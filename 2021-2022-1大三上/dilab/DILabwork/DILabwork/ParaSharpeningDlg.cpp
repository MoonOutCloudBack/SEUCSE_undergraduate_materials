// ParaSharpeningDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "ParaSharpeningDlg.h"
#include "afxdialogex.h"


// ParaSharpeningDlg 对话框

IMPLEMENT_DYNAMIC(ParaSharpeningDlg, CDialogEx)

ParaSharpeningDlg::ParaSharpeningDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARA_SHARPENING, pParent)
{

}

ParaSharpeningDlg::~ParaSharpeningDlg()
{
}

void ParaSharpeningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParaSharpeningDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParaSharpeningDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParaSharpeningDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ParaSharpeningDlg 消息处理程序


void ParaSharpeningDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	CString temp;
	GetDlgItemText(IDC_SHAEPENINGVALUE,temp);
	para=_ttof(temp);
	CDialogEx::OnOK();
}


void ParaSharpeningDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}
