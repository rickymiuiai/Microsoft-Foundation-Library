#pragma once
#include "afxcmn.h"
#include "PatientInfo.h"
#include "PatientInfoDlg.h"
#include "afxwin.h"
#include "ATLComTime.h"


// CPatientListDlg 对话框

class CPatientListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatientListDlg)

public:
	CPatientListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPatientListDlg();

// 对话框数据
	enum { IDD = IDD_PATIENTLIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
public:
	virtual BOOL OnInitDialog();// DDX/DDV 支持
	virtual BOOL DestroyWindow();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQueryBtn();
	afx_msg void OnClickPatientList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkPatientList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkImgList();
	afx_msg void OnDblclkVideoList();
	afx_msg void OnBnClickedClearfilterBtn();
	afx_msg void OnClickedDateCheck();
	afx_msg void OnBnClickedOpenfolderBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	void ClearDlg(BOOL bSelect);

private:
	void ShowExtendINfo();
	void ClearImgList();
	void ClearVideoList();
	std::string GetGender();

public:
	BOOL         m_bDateFilter;
	int          m_nGender;  // 性别
	CString      m_strId;  // 患者ID
	CString      m_strName;  // 患者姓名
	CString      m_strDiagnosis;  // 诊断
	CString      m_strImgRes;  // 影响结果
	CListCtrl    m_ctrlPatientsList;  // 结果列表
	CListBox     m_ctrlImgList;  // 截图列表
	CListBox     m_ctrlVideoList;  // 录像列表
	COleDateTime m_odtStartDate;
	COleDateTime m_odtEndDate;

private:
	int                       m_nCurrSelectIdx;
	CPatientInfoDlg           m_patientInfoDlg;
	std::vector<CPatientInfo> m_patients;
	
};
