#pragma once


// CImgRectScaleDlg �Ի���

class CImgRectScaleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImgRectScaleDlg)

public:
	CImgRectScaleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImgRectScaleDlg();

// �Ի�������
	enum { IDD = IDD_IMGRECTSCALE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	float ScaleValue();
	void UpdateScaleData();

public:
//	float m_Scale;
	virtual BOOL OnInitDialog();
	float m_Scale;
};
