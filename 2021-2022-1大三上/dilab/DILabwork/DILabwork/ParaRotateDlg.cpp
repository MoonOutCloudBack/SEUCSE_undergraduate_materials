// ParaRotateDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "ParaRotateDlg.h"
#include "afxdialogex.h"


// ParaRotateDlg 对话框

IMPLEMENT_DYNAMIC(ParaRotateDlg, CDialogEx)

ParaRotateDlg::ParaRotateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARA_ROTATE, pParent)
{
	goon=false;
}

ParaRotateDlg::~ParaRotateDlg()
{
}

void ParaRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParaRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParaRotateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParaRotateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ParaRotateDlg 消息处理程序


void ParaRotateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	CString temp;
	GetDlgItemText(IDC_ROTATEVALUE,temp);
	para1=_ttof(temp);
	CDialogEx::OnOK();
}


void ParaRotateDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}
