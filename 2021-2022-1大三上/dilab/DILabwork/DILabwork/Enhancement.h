#pragma once
#include<atlimage.h>
#include<algorithm>
#include"ImageData.h"

#ifndef CENHANCEMENT_H
#define CENHANCEMENT_H

using namespace std;

template<class T_DATA>
class CImageData;

// 用来进行图像增强的类
template<class T_DATA>
class CEnhancement
{
public:
	static CImageData<T_DATA> laplacian(CImageData<T_DATA>&, double);		// 拉普拉斯锐化
	static CImageData<T_DATA> medianFilter(CImageData<T_DATA>&);			// 中值滤波器
private:
	static CImageData<T_DATA> conv(CImageData<T_DATA>&, int, double*);		// 卷积操作
};

#endif // CENHANCEMENT_H


