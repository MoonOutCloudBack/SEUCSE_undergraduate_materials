// LoadPathDlg.cpp: 实现文件
//

#include "pch.h"
#include "DILabwork.h"
#include "LoadPathDlg.h"
#include "afxdialogex.h"


// LoadPathDlg 对话框

IMPLEMENT_DYNAMIC(LoadPathDlg, CDialogEx)

LoadPathDlg::LoadPathDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOAD_PATH, pParent)
	, m_path_init(_T(""))
{
	goon=false;
	// path_init.SetWindowText(_T("../barbara.bmp"));
	m_path_init="../knee.raw";
}

LoadPathDlg::~LoadPathDlg()
{
}

void LoadPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_LOAD_PATH, path_init);
	DDX_Text(pDX, IDC_LOAD_PATH, m_path_init);
}


BEGIN_MESSAGE_MAP(LoadPathDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoadPathDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoadPathDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// LoadPathDlg 消息处理程序


void LoadPathDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=true;
	GetDlgItemText(IDC_LOAD_PATH,path);
	CDialogEx::OnOK();
}


void LoadPathDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	goon=false;
	CDialogEx::OnCancel();
}


