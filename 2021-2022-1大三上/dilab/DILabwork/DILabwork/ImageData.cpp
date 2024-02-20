#include<atlimage.h>
#include<iostream>
#include<cstring>
#include"pch.h"
#include"ImageData.h"
using namespace std;

// CImageData类的实现

// 构造一个黑色画布
template<class T_DATA> CImageData<T_DATA>::
CImageData(int w, int h):
	width(w),height(h),pixelData(new T_DATA[w*h]){
	memset(pixelData,0,sizeof(pixelData));
}

// 拷贝图像信息
template<class T_DATA> bool CImageData<T_DATA>::
copy(T_DATA* data,int w,int h){
	if(pixelData)delete[] pixelData;
	pixelData=new T_DATA[w*h];
	memcpy(pixelData,data,sizeof(T_DATA)*w*h);
	width=w,height=h;
	return true;
}

// 将像素同步到CImage中
template<class T_DATA> void CImageData<T_DATA>::
syncPixel(CImage* cimage){
	cimage->Create(width,-1*long long(height),8*sizeof(T_DATA));

	// 调色板
	RGBQUAD* pLUT = new RGBQUAD[256];
	for (int i = 0; i < 256; ++i) {
		(pLUT + i)->rgbRed = (pLUT + i)->rgbGreen = (pLUT + i)->rgbBlue = i;	
		(pLUT + i)->rgbReserved = 0;
	}
	cimage->SetColorTable(0, 256, pLUT);
	delete[] pLUT;

	// 像素赋值
	byte* pDataSrc = (byte*)cimage->GetBits(); // 获取指向图像数据的指针
	int pitchSrc = cimage->GetPitch(); // 获取每行图像占用的字节数 +：top-down；-：bottom-up DIB 
	int bitCountSrc = cimage->GetBPP()/8;  // 获取每个像素占用的字节数

	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++){
			*(pDataSrc+pitchSrc*i+j) = pixelData[i*width+j];
			// *(pDataSrc+pitchSrc*i+j*bitCountSrc) = pixelData[i*width+j];
		}

	/*for(int i=0;i<height;++i)
		for(int j=0;j<width;++j)
			cimage->SetPixel(j,height-1-i,
				RGB(pixelData[i*width+j],
					pixelData[i*width+j],
					pixelData[i*width+j]));*/


}

// 根据文件路径载入图像
template<class T_DATA> bool CImageData<T_DATA>::
load(LPCTSTR path){
	if(!PathFileExists(path)) return false; // 文件不存在，操作失败
	//AfxMessageBox(_T("至少图像是存在的"));

	// 载入图片，记录尺寸
	CImage cimage;
	cimage.Load(path);
	width=cimage.GetWidth();
	height=cimage.GetHeight();

	// 像素数据的内存管理
	if(pixelData)delete[] pixelData; // 收回内存
	pixelData=new T_DATA[width*height]; // 申请内存

	byte* pDataSrc = (byte*)cimage.GetBits(); // 获取指向图像数据的指针
	int pitchSrc = cimage.GetPitch(); // 获取每行图像占用的字节数 +：top-down；-：bottom-up DIB 
	int bitCountSrc = cimage.GetBPP()/8;  // 获取每个像素占用的字节数

	/*byte tmpR,tmpG,tmpB,avg;
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++){
			tmpR = *(pDataSrc+pitchSrc*j+i*bitCountSrc);
			tmpG = *(pDataSrc+pitchSrc*j+i*bitCountSrc+1);
			tmpB = *(pDataSrc+pitchSrc*j+i*bitCountSrc+2);
			avg = (byte)(tmpR+tmpG+tmpB)/3;
			*(pixelData+pitchDst*j+i*bitCountDst) = avg;
		}*/

	// 取像素的红色数据，得到灰度值
	byte tmpR,tmpG,tmpB,avg;
	for(int i=0;i<height;++i)
		for(int j=0;j<width;++j){
			tmpR = *(pDataSrc+pitchSrc*i+j*bitCountSrc);
			pixelData[i*width+j]=tmpR;
			// pixelData[i*width+j]=255;
			// GetRValue(cimage.GetPixel(j,height-1-i))
		}

	return true; // 操作成功
}

// 把图像保存到对应路径的文件
template<class T_DATA> bool CImageData<T_DATA>::
save(LPCTSTR path){
	CImage cimage;
	syncPixel(&cimage); // cimage同步像素数据
	cimage.Save(path); // 存储到文件
	// 如果没法根据文件名判断类型，就默认存储成bmp类型
	return true; // 操作成功
}

// 显示图像
template<class T_DATA> bool CImageData<T_DATA>::
show(CDC* pDC,int x,int y){
	CImage cimage;
	syncPixel(&cimage); // cimage同步像素数据
	// cimage.Load(_T("test.bmp"));
	cimage.Draw(pDC->GetSafeHdc(),x,y,min(width,570),min(height,570),
		0,0,width,height);

	return true;
}



/*
参考资料：

CImage 访问像素点
https://bbs.csdn.net/topics/330056240

Cimage类的介绍及使用
https://blog.csdn.net/u013472838/article/details/80519525

MFC显示图片
https://blog.csdn.net/lynn1258/article/details/54376311

CImage 设置任意像素的RGB值
https://blog.csdn.net/u011450490/article/details/53638942

使用 CImage 类将彩色图像转换成8位灰色图像
https://blog.csdn.net/hero_myself/article/details/45152453

*/