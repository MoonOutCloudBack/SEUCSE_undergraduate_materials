// ParaScalingDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "ParaScalingDlg.h"
#include "afxdialogex.h"


// ParaScalingDlg 对话框

IMPLEMENT_DYNAMIC(ParaScalingDlg, CDialogEx)

ParaScalingDlg::ParaScalingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARA_SCALING, pParent)
{
	goon=false;
}

ParaScalingDlg::~ParaScalingDlg()
{
}

void ParaScalingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ParaScalingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParaScalingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ParaScalingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ParaScalingDlg 消息处理程序


void ParaScalingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	CString temp;
	GetDlgItemText(IDC_SCALINGVALUE,temp);
	para1=_ttof(temp);
	CDialogEx::OnOK();
}


void ParaScalingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}
