#include <algorithm>
#include <cstring>
#include <vector>
#include "pch.h"
#include "Enhancement.h"

using namespace std;

// 拉普拉斯锐化
template<class T_DATA>
CImageData<T_DATA> CEnhancement<T_DATA>::
laplacian(CImageData<T_DATA>& src, double k)
{
	double* kernel=new double[3*3];
	
	kernel[0]=-k; kernel[1]=-k;    kernel[2]=-k;
	kernel[3]=-k; kernel[4]=8*k+1; kernel[5]=-k;
	kernel[6]=-k; kernel[7]=-k;    kernel[8]=-k;
/*
	kernel[0]=0; kernel[1]=0;  kernel[2]=0;
	kernel[3]=0; kernel[4]=1; kernel[5]=0;
	kernel[6]=0; kernel[7]=0;  kernel[8]=0;
*/
	CImageData<T_DATA> result=conv(src, 3, kernel);
	delete kernel;
	return result;
}

// 中值滤波器
template<class T_DATA>
CImageData<T_DATA> CEnhancement<T_DATA>::
medianFilter(CImageData<T_DATA>& src)
{
	int w=src.width, h=src.height;
	int r=1;
	T_DATA* srcdata=src.pixelData, *dstdata=new T_DATA[w*h];
	memset(dstdata, 0, sizeof(dstdata));

	// （魔改的）中值滤波
	T_DATA neighbor[9+2];
	for(int i=r;i<h-r;++i)
		for(int j=r;j<w-r;++j){
			double temp=0;
			neighbor[9]=neighbor[10]=srcdata[i*w+j];
			for(int i0=-r;i0<=r;++i0)
				for(int j0=-r;j0<=r;++j0)
					neighbor[(r+i0)*3+(r+j0)]=srcdata[(i+i0)*w+(j+j0)];
			sort(neighbor,neighbor+11);
			temp=(neighbor[4]+neighbor[5]+neighbor[6])/3.0;
			dstdata[i*w+j] = ceil(temp);
		}

	for(int r0=0;r0<r;++r0){
		for(int j=0;j<w;++j) dstdata[r0*w+j]=srcdata[r0*w+j], dstdata[(h-1-r0)*w+j]=srcdata[(h-1-r0)*w+j];
		for(int i=0;i<h;++i) dstdata[i*w+r0]=srcdata[i*w+r0], dstdata[i*w+w-1-r0]=srcdata[i*w+w-1-r0];
	}

	// 赋值，ok啦！
	CImageData<T_DATA> result;
	result.copy(dstdata,w,h);
	delete[] dstdata;
	return result;
}

// 卷积操作
template<class T_DATA>
CImageData<T_DATA> CEnhancement<T_DATA>::
conv(CImageData<T_DATA>& src, int size, double* kernel)
{
	int w=src.width, h=src.height;
	int r=(size-1)>>1;
	T_DATA* srcdata=src.pixelData, *dstdata=new T_DATA[w*h];
	memset(dstdata, 0, sizeof(dstdata));

	// 卷积！
	for(int i=r;i<h-r;++i)
		for(int j=r;j<w-r;++j){
			double temp=0;
			for(int i0=-r;i0<=r;++i0)
				for(int j0=-r;j0<=r;++j0)
					temp += kernel[(i0+r)*size+(j0+r)]*srcdata[(i+i0)*w+(j+j0)];
			if(temp<0) dstdata[i*w+j]=0;
			else dstdata[i*w+j] = ceil(temp);
		}

	for(int r0=0;r0<r;++r0){
		for(int j=0;j<w;++j) dstdata[r0*w+j]=srcdata[r0*w+j], dstdata[(h-1-r0)*w+j]=srcdata[(h-1-r0)*w+j];
		for(int i=0;i<h;++i) dstdata[i*w+r0]=srcdata[i*w+r0], dstdata[i*w+w-1-r0]=srcdata[i*w+w-1-r0];
	}
	
	// 赋值，ok啦！
	CImageData<T_DATA> result;
	result.copy(dstdata,w,h);
	delete[] dstdata;
	return result;
}
