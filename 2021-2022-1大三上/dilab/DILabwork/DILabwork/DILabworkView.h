
// DILabworkView.h: CDILabworkView 类的接口
//

#pragma once
#include"ImageData.h"
#include"DILabworkDoc.h"
#include"MedicalImageData.h"

class CDILabworkView : public CScrollView
{
protected: // 仅从序列化创建
	CDILabworkView() noexcept;
	DECLARE_DYNCREATE(CDILabworkView)
private:
	CImageData<unsigned char> *src=NULL;
	CImageData<unsigned char> *result=NULL;
	CMedicalImageData<unsigned long> *medical=NULL;
	int xMedical, yMedical;
	double levelMedical, widthMedical;

// 特性
public:
	CDILabworkDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDILabworkView();
	virtual void OnInitialUpdate();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnTranslate();
	afx_msg void OnRotate();
	afx_msg void OnScaling();
	afx_msg void OnFileOpen();
	afx_msg void OnOpenMedical();
	afx_msg void OnChangeGreyWindow();
	afx_msg void OnDenoising();
	afx_msg void OnSharpening();
	afx_msg void OnImageSegment();
};

#ifndef _DEBUG  // DILabworkView.cpp 中的调试版本
inline CDILabworkDoc* CDILabworkView::GetDocument() const
   { return reinterpret_cast<CDILabworkDoc*>(m_pDocument); }
#endif

