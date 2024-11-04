#pragma once
#include "afxcmn.h"
#include "PatientInfo.h"
#include "PatientInfoDlg.h"
#include "afxwin.h"
#include "ATLComTime.h"


// CPatientListDlg �Ի���

class CPatientListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatientListDlg)

public:
	CPatientListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPatientListDlg();

// �Ի�������
	enum { IDD = IDD_PATIENTLIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
public:
	virtual BOOL OnInitDialog();// DDX/DDV ֧��
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
	int          m_nGender;  // �Ա�
	CString      m_strId;  // ����ID
	CString      m_strName;  // ��������
	CString      m_strDiagnosis;  // ���
	CString      m_strImgRes;  // Ӱ����
	CListCtrl    m_ctrlPatientsList;  // ����б�
	CListBox     m_ctrlImgList;  // ��ͼ�б�
	CListBox     m_ctrlVideoList;  // ¼���б�
	COleDateTime m_odtStartDate;
	COleDateTime m_odtEndDate;

private:
	int                       m_nCurrSelectIdx;
	CPatientInfoDlg           m_patientInfoDlg;
	std::vector<CPatientInfo> m_patients;
	
};
