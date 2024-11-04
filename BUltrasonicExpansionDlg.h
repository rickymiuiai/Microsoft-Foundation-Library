
// BUltrasonicExpansionDlg.h : 头文件
//

#pragma once

#include "afxwin.h"
#include "AlxDVIView.h"
#include "AlxImgView.h"
#include "RegPatientDlg.h"
#include "PatientListDlg.h"
#include "VideoCaptureFactory.h"
#include "SystemConfig.h"

#include "ImgRectSelectTipsDlg.h"
#include "ImgRectScaleDlg.h"


// CBUltrasonicExpansionDlg 对话框
class CBUltrasonicExpansionDlg : public CDialogEx
{
// 构造
public:
	CBUltrasonicExpansionDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CBUltrasonicExpansionDlg();

// 对话框数据
	enum { IDD = IDD_BULTRASONICEXPANSION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStartBtn();
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnBnClickedClearBtn();
	afx_msg void OnBnClickedClearImgBtn();
	afx_msg void OnBnClickedSnapBtn();
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnBnClickedQueryBtn();
	afx_msg void OnBnClickedOpenfolderBtn();
	afx_msg void OnBnClickedAirwayradiusBtn();
	afx_msg void OnSelchangeRadiusAlgorithmCombo();
	afx_msg LRESULT OnViewDBClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewResize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewResizeStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePatientInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImgValidRectSelectFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTriangleVertexesSelectFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImgRectSelectTipsDlgClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImgRectScaleDlgClose(WPARAM wParam, LPARAM lParam);
	

private:
	void ShowHideCtrl(BOOL bShow = TRUE);
	void ShowViewFullScreen(int fs = 0);
	void Counter();

public:
	CAlxDVIView          m_imgDVI;
	CAlxImgView          m_imgSnap;

private:
	int                  m_nsw;
	int                  m_nsh;
	int                  m_nNrmViewW;
	int                  m_nNrmViewH;
	int                  m_nViewIdx;
	int                  m_nCalRadiusViewIdx;
	int                  m_nRadiusAlgorithm;
	float                m_fRate;
	float                m_fImgRate;
	DWORD                m_dwStyle;
	DWORD                m_dwExStyle;
	BOOL                 m_bPlaying;
	CRegPatientDlg*      m_pRegPatientDlg;
	CPatientListDlg*     m_pPatientListDlg;
	CImgRectSelectTipsDlg* m_pRectSelectTipsDlg;
	CImgRectScaleDlg*    m_pRectScaleDlg;
	CRect                m_rcClient;
	CRect                m_rcDesktop;
	CVideoCaptureFactory m_VideoCaptureFactory;

};
