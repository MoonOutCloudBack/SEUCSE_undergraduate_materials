#pragma once
#include<atlimage.h>
#include"GeoTransform.h"
#include"Enhancement.h"

#ifndef CIMAGEDATA_H
#define CIMAGEDATA_H

template<class T_DATA>
class CGeoTransform;

// 用来存储图像数据的类
// 支持根据路径读写图像，支持显示图像
template<class T_DATA>
class CImageData
{
	template<class T_DATA>
	friend class CGeoTransform;			// 几何变换类是友元类
	template<class T_DATA>
	friend class CEnhancement;			// 图像增强类是友元类
	template<class T_DATA>
	friend class CSegment;				// 图像分割类是友元类
protected:
	unsigned long width;				// 图像的宽，单位是像素
	unsigned long height;				// 图像的高，单位是像素
	T_DATA* pixelData;					// 像素数据

	void syncPixel(CImage*);			// 将像素同步到CImage中

public:
	CImageData(){pixelData=NULL;}		// 默认构造函数
	CImageData(int, int);				// 构造一个黑色画布
	virtual ~CImageData(){}				// 析构函数

	bool copy(T_DATA*,int,int);			// 拷贝图像信息

	int getWidth() const {return width;}
	int getHeight() const {return height;}

	bool load(LPCTSTR path);			// 根据文件路径载入图像
	bool save(LPCTSTR path);			// 把图像保存到对应路径的文件
	bool show(CDC* pDC,int,int);		// 显示图像

};

#endif // CIMAGEDATA_H
/*
我到底要写出什么？
首先是，CImageData类，大体实现功能就好。
然后是，GeoTransform类，首先写一个affine变换，输入是width、height和矩阵参数，输出是【每一个像素映射到的横纵坐标】。
在affine后，需要进行插值。怎么插值呢？每一个像素映射到的横纵坐标，根据这个取值，就得到了。

*/