#include "pch.h"
#include "MedicalImageData.h"
#include <string> 
#include <fstream> 
#include <iostream> 

// 读取raw文件
template<class T_DATA> bool CMedicalImageData<T_DATA>::
loadRaw(CString path){
	ifstream raw(path, ios::in|ios::binary);
	if (!raw.is_open()) return false;

	raw.read((char*)&width, sizeof(unsigned long));
	raw.read((char*)&height, sizeof(unsigned long));

	//CString temp_value = _T("");
	//temp_value.Format(_T("width,height=%u,%u"), width, height);//固定格式
	//AfxMessageBox(temp_value);

	if(pixelData!=NULL)delete pixelData;
	pixelData=new T_DATA[width*height];
	for(int i=0;i<width*height;++i){
		raw.read((char*)&pixelData[i], sizeof(unsigned short));
		pixelData[i]%=4096;
	}

	raw.close();
	return true;
}

// 带有灰度窗功能的 像素数据同步
template<class T_DATA> void CMedicalImageData<T_DATA>::
greyWindowSync(CImage* cimage, double wlevel, double wwidth){
	
	int tsize=570;
	cimage->Create(width,-1*long long(height),8*sizeof(byte));

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

	// 灰度窗
	wlevel-=wwidth/2;
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++){
			byte value=0;
			if(pixelData[i*width+j]<=wlevel)value=0;
			else if(pixelData[i*width+j]>=wlevel+wwidth)value=255;
			else value=floor((pixelData[i*width+j]-wlevel)/wwidth*256);
			*(pDataSrc+pitchSrc*i+j) = value;
		}
}

// 显示局部图像
// int参数为图像局部的左上角左边（大小为570*570），double参数为灰度窗参数
template<class T_DATA> void CMedicalImageData<T_DATA>::
show(CDC* pDC, double wlevel, double wwidth){
	
	CImage cimage;
	greyWindowSync(&cimage, wlevel, wwidth); // cimage同步像素数据

	cimage.Draw(pDC->GetSafeHdc(),0,0,width,height,
		0,0,width,height);
}
