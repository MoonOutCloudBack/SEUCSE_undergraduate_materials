// ParaTranslateDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "ParaTranslateDlg.h"
#include "afxdialogex.h"
#include "DILabworkView.h"


// ParaTranslateDlg 对话框

IMPLEMENT_DYNAMIC(ParaTranslateDlg, CDialogEx)

ParaTranslateDlg::ParaTranslateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARA_TRANSLATE, pParent)
{
	goon=false;
}

ParaTranslateDlg::~ParaTranslateDlg()
{
}

void ParaTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParaTranslateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParaTranslateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParaTranslateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ParaTranslateDlg 消息处理程序


void ParaTranslateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	CString temp;
	GetDlgItemText(IDC_XSPACE,temp);
	para1=_ttof(temp);
	GetDlgItemText(IDC_YSPACE,temp);
	para2=_ttof(temp);
	CDialogEx::OnOK();
}


void ParaTranslateDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}
