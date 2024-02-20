#pragma once


// ParaGrayWindowDlg 对话框

class ParaGrayWindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParaGrayWindowDlg)

public:
	ParaGrayWindowDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParaGrayWindowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARA_GRAY_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool goon;
	double para1,para2;
};
