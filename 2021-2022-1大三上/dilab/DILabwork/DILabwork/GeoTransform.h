#pragma once
#include<atlimage.h>
#include<algorithm>
#include"ImageData.h"

#ifndef CGEOTRANSFORM_H
#define CGEOTRANSFORM_H

using namespace std;

template<class T_DATA>
class CImageData;

// 用来进行图像几何变换的类
template<class T_DATA>
class CGeoTransform
{
public:
	static CImageData<T_DATA> rotate(CImageData<T_DATA>&, double);				// 旋转
	static CImageData<T_DATA> scaling(CImageData<T_DATA>&, double);				// 缩放
	static CImageData<T_DATA> translate(CImageData<T_DATA>&, double, double);	// 平移
private:
	static CImageData<T_DATA> generalGeoTrans(CImageData<T_DATA>&, double[3][4]);	// 所有几何变换
	static pair<double,double>* affine(int, int, double[3][4]);						// 仿射变换，得到坐标
	static T_DATA* interpolate(pair<double,double>*, CImageData<T_DATA>&);			// 插值，得到灰度值

	static void printContent(CImageData<T_DATA>&);									// 为了debug
};

#endif // CGEOTRANSFORM_H