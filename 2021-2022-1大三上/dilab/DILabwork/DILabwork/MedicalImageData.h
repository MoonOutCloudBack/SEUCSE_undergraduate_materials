#pragma once
#include "ImageData.h"

#ifndef CMEDICALIMAGEDATA_H
#define CMEDICALIMAGEDATA_H

template<class T_DATA>
class CMedicalImageData : public CImageData<T_DATA>
{
public:
	bool loadRaw(CString);						// 读取医学图像数据
	void greyWindowSync(CImage*,double,double);	// 调整灰度窗
	void show(CDC*,double,double);				// 显示图像
};

#endif
