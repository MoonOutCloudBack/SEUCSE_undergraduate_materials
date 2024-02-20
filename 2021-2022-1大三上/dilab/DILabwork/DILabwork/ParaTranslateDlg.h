#pragma once


// ParaTranslateDlg 对话框

class ParaTranslateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParaTranslateDlg)

public:
	ParaTranslateDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParaTranslateDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARA_TRANSLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

	DECLARE_MESSAGE_MAP()

public:
	void OnBnClickedOk();
	void OnBnClickedCancel();

	bool goon;
	double para1;
	double para2;
};
