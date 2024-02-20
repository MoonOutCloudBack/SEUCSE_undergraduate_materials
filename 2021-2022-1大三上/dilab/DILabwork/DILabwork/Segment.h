#pragma once
#include<atlimage.h>
#include<algorithm>
#include"ImageData.h"

#ifndef CSEGMENT_H
#define CSEGMENT_H

using namespace std;

template<class T_DATA>
class CImageData;

// 用来进行图像几何变换的类
template<class T_DATA>
class CSegment{
public:
	CSegment(double);
	CImageData<T_DATA> twoClassSegment(CImageData<T_DATA>&);	// 二分类图像分割
private:
	double conv[5][5];											// 用来提取特征值的卷积核
	double eps;													// 区域增长法的相似性阈值
	double* getFeature(CImageData<T_DATA>&);					// 得到特征值
	CImageData<T_DATA> regionalGrowth(int, int, double*);		// 区域增长法
};
#endif // CSEGMENT_H
