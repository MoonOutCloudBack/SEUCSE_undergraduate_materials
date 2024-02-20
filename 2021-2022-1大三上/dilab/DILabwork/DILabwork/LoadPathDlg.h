#pragma once


// LoadPathDlg 对话框

class LoadPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoadPathDlg)

public:
	LoadPathDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LoadPathDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_PATH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool goon;
	CString path;

	// 是因为我想给src img的路径赋一个初值啦
	CString m_path_init;
};
