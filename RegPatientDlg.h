#pragma once

#include <string>

// CRegPatientDlg 对话框

class CRegPatientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegPatientDlg)

public:
	CRegPatientDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegPatientDlg();

// 对话框数据
	enum { IDD = IDD_REGPATIENT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 患者年龄
	UINT    m_unAge;
	// 性别
	int     m_nGender;
	// 登记日期
	CString m_strDate;
	// 诊断结果
	CString m_strDiagnosis;
	// 患者ID
	CString m_strPatientId;
	// 影像结果
	CString m_strImgRes;
	// 患者姓名
	CString m_strName;
	
};
