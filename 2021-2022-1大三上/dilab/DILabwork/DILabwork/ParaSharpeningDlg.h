#pragma once


// ParaSharpeningDlg 对话框

class ParaSharpeningDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParaSharpeningDlg)

public:
	ParaSharpeningDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParaSharpeningDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARA_SHARPENING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool goon;
	double para;
};
