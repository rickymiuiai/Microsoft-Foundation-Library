#pragma once

#include <string>

// CRegPatientDlg �Ի���

class CRegPatientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegPatientDlg)

public:
	CRegPatientDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegPatientDlg();

// �Ի�������
	enum { IDD = IDD_REGPATIENT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	void ClearDlg();

private:
	double GetDateTimeD();
	std::string GetGender();
	std::string GetPatientPath(std::string strId);

	void Counter(std::string strPatientId);

public:
	// ��������
	UINT    m_unAge;
	// �Ա�
	int     m_nGender;
	// �Ǽ�����
	CString m_strDate;
	// ��Ͻ��
	CString m_strDiagnosis;
	// ����ID
	CString m_strPatientId;
	// Ӱ����
	CString m_strImgRes;
	// ��������
	CString m_strName;
	
};
