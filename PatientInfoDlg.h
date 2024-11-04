#pragma once
#include "afxwin.h"
#include "PatientInfo.h"


// CPatientInfoDlg �Ի���

class CPatientInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatientInfoDlg)

public:
	CPatientInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPatientInfoDlg();

// �Ի�������
	enum { IDD = IDD_PATIENTINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	void SetPatientInfo(CPatientInfo& info);

private:
	int  GenderIdx(std::string str);
	std::string GetGender();
	void ClearImageListBox();
	void ClearVideoListBox();

public:
	int      m_nAge;
	int      m_nGender;
	CString  m_strDiagnosis;
	CString  m_strId;
	CListBox m_ctrlImgList;
	CString  m_strImgRes;
	CString  m_strName;
	CString  m_strDate;
	CListBox m_ctrlVideoList;
	CPatientInfo m_patientInfo;
	
	
public:
	afx_msg void OnDblclkImgList();
	afx_msg void OnDblclkVideoList();
};
