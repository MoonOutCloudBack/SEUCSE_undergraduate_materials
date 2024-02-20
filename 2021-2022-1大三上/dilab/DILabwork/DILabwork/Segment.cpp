#include <algorithm>
#include <cstring>
#include <stack>
#include <queue>
#include <cmath>
#include <vector>
#include "pch.h"
#include "Segment.h"

using namespace std;

// 构造函数
template<class T_DATA>
CSegment<T_DATA>::
CSegment(double ee){
	double temp[5][5] = {
		{0,		0.005,	0.005,	0.005,	0},
		{0.005,	0.025,	0.05,	0.025,	0.005},
		{0.005,	0.05,	0.64,	0.05,	0.005},
		{0.005,	0.025,	0.05,	0.025,	0.005},
		{0,		0.005,	0.005,	0.005,	0}};
	double sum = 0;
	for(int i=0;i<25;++i) sum+=temp[i/5][i%5];
	assert(abs(sum-1)<0.0001); // check whether sum == 1
	memcpy(this->conv, temp, sizeof(temp));
	this->eps = ee;
}

// 二分类图像分割
template<class T_DATA>
CImageData<T_DATA> CSegment<T_DATA>::
twoClassSegment(CImageData<T_DATA>& src){
	int width = src.width, height = src.height;
	double* feature = getFeature(src); // 得到特征值
	CImageData<T_DATA> result = regionalGrowth(width, height, feature);
	// 利用特征值进行二分类
	delete[] feature;
	return result; // ok啦！
}

// 得到特征值
template<class T_DATA>
double* CSegment<T_DATA>::
getFeature(CImageData<T_DATA>& image){
	// init
	int width = image.width, height = image.height;
	double* feature = new double[width * height];
	T_DATA* origin = image.pixelData;
	memset(feature, 0, sizeof(double) * width * height);

	// get feature
	for(int i=2;i<height-2;++i)
		for(int j=2;j<width-2;++j)
			for(int i0=-2;i0<=2;++i0)
				for(int j0=-2;j0<=2;++j0)
					feature[i*width+j] += conv[i0+2][j0+2] * origin[(i+i0)*width+j+j0];
	for(int i=0;i<2;++i)
		for(int j=0;j<width;++j)
			feature[i*width+j] = origin[i*width+j];
	for(int i=height-2;i<height;++i)
		for(int j=0;j<width;++j)
			feature[i*width+j] = origin[i*width+j];
	for(int j=0;j<2;++j)
		for(int i=0;i<height;++i)
			feature[i*width+j] = origin[i*width+j];
	for(int j=width-2;j<width;++j)
		for(int i=0;i<height;++i)
			feature[i*width+j] = origin[i*width+j];
	return feature;
}

// 区域增长法
template<class T_DATA>
CImageData<T_DATA> CSegment<T_DATA>::
regionalGrowth(int width, int height, double* feature){
	int n = 4;

	// init stack & pick seeds
	queue<pair<int,int> > st;
	int blockw = ceil(double(width)/n), blockh = ceil(double(height)/n);
	for(int ii=0;ii<n;++ii)
		for(int jj=0;jj<n;++jj){
			double mini = 500; int minx = -1, miny = -1;
			for(int i=ii*blockh; i<min((ii+1)*blockh,height); ++i)
				for(int j=jj*blockw; j<min((jj+1)*blockw,width); ++j)
					if(feature[i*width+j]<mini){
						mini = feature[i*width+j];
						minx = i, miny = j;
					}
			st.push(pair<int,int>(minx, miny));
		}

	// regional growth
	bool* visited = new bool[width*height];
	memset(visited, 0, sizeof(bool)*width*height);

	while(!st.empty()){
		pair<int,int> pos = st.front(); st.pop(); // get top
		int x=pos.first, y=pos.second; // get coordinate
		if(visited[x*width+y]) continue; // visited coordinate
		visited[x*width+y] = true; // then we visit it
		// traverse the neighbor
		for(int i=-1;i<=1;++i)
			for(int j=-1;j<=1;++j){
				if(i==0 && j==0) continue; // itself
				if(!(x+i>=0 && x+i<height && y+j>=0 && y+j<width)) continue; // out of bound
				if(visited[(x+i)*width+y+j]) continue; // visited
				if(abs(feature[x*width+y]-feature[(x+i)*width+y+j])<eps){
					st.push(pair<int,int>(x+i, y+j)); // in the same class
				}
			}
	}

	// get result
	T_DATA* resultval = new T_DATA[width*height];
	memset(resultval, 0, sizeof(T_DATA)*width*height);
	for(int i=0;i<height;++i)
		for(int j=0;j<width;++j)
			if(!visited[i*width+j]) // it's white region
				resultval[i*width+j] = 255;
	CImageData<T_DATA> result;
	result.copy(resultval, width, height);

	// clear & return
	delete[] resultval; delete[] visited;
	return result;
}
