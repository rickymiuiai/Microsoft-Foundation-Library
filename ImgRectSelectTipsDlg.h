#pragma once


// CImgRectSelectTipsDlg �Ի���

class CImgRectSelectTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImgRectSelectTipsDlg)

public:
	CImgRectSelectTipsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImgRectSelectTipsDlg();

	void EnableSkip(bool isEnable);

// �Ի�������
	enum { IDD = IDD_IMGRECTSELECTTIPS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
