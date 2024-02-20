// ParaGrayWindowDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "ParaGrayWindowDlg.h"
#include "afxdialogex.h"


// ParaGrayWindowDlg 对话框

IMPLEMENT_DYNAMIC(ParaGrayWindowDlg, CDialogEx)

ParaGrayWindowDlg::ParaGrayWindowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARA_GRAY_WINDOW, pParent)
{
	goon=false;
}

ParaGrayWindowDlg::~ParaGrayWindowDlg()
{
}

void ParaGrayWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParaGrayWindowDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParaGrayWindowDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParaGrayWindowDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ParaGrayWindowDlg 消息处理程序


void ParaGrayWindowDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	CString temp;
	GetDlgItemText(IDC_GRAYWINDOW_LEVEL,temp);
	para1=_ttof(temp);
	GetDlgItemText(IDC_GRAYWINDOW_WIDTH,temp);
	para2=_ttof(temp);
	CDialogEx::OnOK();
}


void ParaGrayWindowDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}
