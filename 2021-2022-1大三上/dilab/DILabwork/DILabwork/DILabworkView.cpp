
// DILabworkView.cpp: CDILabworkView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DILabwork.h"
#endif

#include "DILabworkDoc.h"
#include "DILabworkView.h"
#include"ImageData.h"
#include"ImageData.cpp"
#include"GeoTransform.h"
#include"GeoTransform.cpp"
#include"MedicalImageData.h"
#include"MedicalImageData.cpp"
#include"Enhancement.h"
#include"Enhancement.cpp"
#include"Segment.h"
#include"Segment.cpp"

#include"ParaTranslateDlg.h"
#include"ParaRotateDlg.h"
#include"ParaScalingDlg.h"
#include"LoadPathDlg.h"
#include"ParaGrayWindowDlg.h"
#include"ParaSharpeningDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDILabworkView

IMPLEMENT_DYNCREATE(CDILabworkView, CScrollView)

BEGIN_MESSAGE_MAP(CDILabworkView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDILabworkView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TRANSLATE, &CDILabworkView::OnTranslate)
	ON_COMMAND(ID_ROTATE, &CDILabworkView::OnRotate)
	ON_COMMAND(ID_SCALING, &CDILabworkView::OnScaling)
	ON_COMMAND(ID_FILE_OPEN, &CDILabworkView::OnFileOpen)
	ON_COMMAND(ID_OPEN_MEDICAL, &CDILabworkView::OnOpenMedical)
	ON_COMMAND(ID_CHANGE_GREY_WINDOW, &CDILabworkView::OnChangeGreyWindow)
	ON_COMMAND(ID_DENOISING, &CDILabworkView::OnDenoising)
	ON_COMMAND(ID_SHARPENING, &CDILabworkView::OnSharpening)
	ON_COMMAND(ID_IMAGE_SEGMENT, &CDILabworkView::OnImageSegment)
END_MESSAGE_MAP()

// CDILabworkView 构造/析构

CDILabworkView::CDILabworkView() noexcept
{
	src=NULL;result=NULL;medical=NULL;
	xMedical=yMedical=levelMedical=widthMedical=-1;

}

CDILabworkView::~CDILabworkView()
{
	
}

void CDILabworkView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(1800,1800)); // 偷懒设置成最大值了
	CScrollView::OnInitialUpdate();
}

BOOL CDILabworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CDILabworkView 绘图

void CDILabworkView::OnDraw(CDC* pDC)
{
	CDILabworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if(src) src->show(pDC,0,0);
	if(result) result->show(pDC,600,0);
	if(medical) medical->show(pDC,levelMedical,widthMedical);
}

// CDILabworkView 打印

void CDILabworkView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDILabworkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDILabworkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDILabworkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDILabworkView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDILabworkView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDILabworkView 诊断

#ifdef _DEBUG
void CDILabworkView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDILabworkView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDILabworkDoc* CDILabworkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDILabworkDoc)));
	return (CDILabworkDoc*)m_pDocument;
}
#endif //_DEBUG


// CDILabworkView 消息处理程序

void CDILabworkView::OnTranslate()
{
	if(!src)return;
	// assert(src);
	if(result)delete result;result=NULL;
	result=new CImageData<byte>;

	ParaTranslateDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	CGeoTransform<byte> gt;
	CImageData<byte>* temp=&gt.translate(*src,dlg.para1,dlg.para2);
	memcpy(result,temp,sizeof(CImageData<byte>));
	Invalidate();
}


void CDILabworkView::OnRotate()
{
	if(!src)return;
	if(result)delete result;result=NULL;
	result=new CImageData<byte>;

	ParaRotateDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	CGeoTransform<byte> gt;
	CImageData<byte>* temp=&gt.rotate(*src,dlg.para1/180.0*3.1415926);
	memcpy(result,temp,sizeof(CImageData<byte>));
	Invalidate();
}


void CDILabworkView::OnScaling()
{
	if(!src)return;
	if(result)delete result;result=NULL;
	result=new CImageData<byte>;

	ParaScalingDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	CGeoTransform<byte> gt;
	CImageData<byte>* temp=&gt.scaling(*src,dlg.para1);
	memcpy(result,temp,sizeof(CImageData<byte>));
	Invalidate();
}


void CDILabworkView::OnFileOpen()
{
	LoadPathDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	if(src)delete src;src=NULL;
	if(result)delete result;result=NULL;

	src=new CImageData<byte>;
	if(!src->load((LPCTSTR)(dlg.path))) {
		AfxMessageBox(_T("源图像加载错误！"));
		if(src)delete src;src=NULL;
	}

	Invalidate();
}


void CDILabworkView::OnOpenMedical()
{
	LoadPathDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	if(src)delete src;src=NULL;
	if(result)delete result;result=NULL;
	if(medical)delete medical;medical=NULL;

	medical=new CMedicalImageData<unsigned long>;
	if(!medical->loadRaw(dlg.path)) {
		AfxMessageBox(_T("源图像加载错误！"));
		if(medical){delete medical;medical=NULL;return;}
	}
	
	xMedical=yMedical=0;
	levelMedical=2048, widthMedical=4096;
	Invalidate();
}


void CDILabworkView::OnChangeGreyWindow()
{
	ParaGrayWindowDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	levelMedical=dlg.para1;
	widthMedical=dlg.para2;
	Invalidate();
}


void CDILabworkView::OnDenoising()
{
	if(!medical)return;
	CImageData<unsigned long> *medisrc=medical;

	CEnhancement<unsigned long> eh;
	CImageData<unsigned long>* temp=&eh.medianFilter(*medisrc);
	memcpy(medical,temp,sizeof(CImageData<unsigned long>));
	Invalidate();
}


void CDILabworkView::OnSharpening()
{
	if(!medical)return;
	CImageData<unsigned long> *medisrc=medical;

	ParaSharpeningDlg dlg;
	dlg.DoModal();
	if(!dlg.goon)return;

	CEnhancement<unsigned long> eh;
	CImageData<unsigned long>* temp=&eh.laplacian(*medisrc,dlg.para);
	memcpy(medical,temp,sizeof(CImageData<unsigned long>));
	Invalidate();
}


void CDILabworkView::OnImageSegment()
{
	if(src)delete src;
	src=new CImageData<byte>;
	if(!src->load(_T("../seed.bmp"))) {
		AfxMessageBox(_T("源图像加载错误！"));
		if(src)delete src;src=NULL;
		return;
	}
	if(result)delete result;result=NULL;
	result=new CImageData<byte>;

	CSegment<byte> seg(6.009);
	CImageData<byte>* temp=&seg.twoClassSegment(*src);
	memcpy(result,temp,sizeof(CImageData<byte>));
	result->save(_T("../seed_result.bmp"));
	Invalidate();
}
