#pragma once


// ParaScalingDlg 对话框

class ParaScalingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParaScalingDlg)

public:
	ParaScalingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParaScalingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARA_SCALING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool goon;
	double para1;
};
