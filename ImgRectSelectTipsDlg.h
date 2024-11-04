#pragma once


// CImgRectSelectTipsDlg 对话框

class CImgRectSelectTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImgRectSelectTipsDlg)

public:
	CImgRectSelectTipsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImgRectSelectTipsDlg();

	void EnableSkip(bool isEnable);

// 对话框数据
	enum { IDD = IDD_IMGRECTSELECTTIPS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSkipBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	bool m_IsEnableSkipBtn;
public:
	virtual BOOL OnInitDialog();
};
