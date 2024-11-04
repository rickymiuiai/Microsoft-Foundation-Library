#pragma once


// CImgRectScaleDlg 对话框

class CImgRectScaleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImgRectScaleDlg)

public:
	CImgRectScaleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImgRectScaleDlg();

// 对话框数据
	enum { IDD = IDD_IMGRECTSCALE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
