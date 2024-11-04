// PatientListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "PatientListDlg.h"
#include "afxdialogex.h"
#include "DataPersistence.h"
#include "Common.h"


// CPatientListDlg 对话框

IMPLEMENT_DYNAMIC(CPatientListDlg, CDialogEx)

CPatientListDlg::CPatientListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatientListDlg::IDD, pParent)
	, m_bDateFilter(FALSE)
	, m_nGender(0)
	, m_strId(_T(""))
	, m_strName(_T(""))
	, m_strDiagnosis(_T(""))
	, m_strImgRes(_T(""))
	, m_odtStartDate(COleDateTime::GetCurrentTime())
	, m_odtEndDate(COleDateTime::GetCurrentTime())
	, m_nCurrSelectIdx(-1)
	, m_patientInfoDlg()
	, m_patients()
{

}

CPatientListDlg::~CPatientListDlg()
{
	
}

void CPatientListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID_EDIT, m_strId);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	DDX_Control(pDX, IDC_PATIENT_LIST, m_ctrlPatientsList);
	DDX_Text(pDX, IDC_DIAGNOSIS_EDIT, m_strDiagnosis);
	DDX_Radio(pDX, IDC_GENDER_RADIO, m_nGender);
	DDX_Control(pDX, IDC_IMG_LIST, m_ctrlImgList);
	DDX_Text(pDX, IDC_IMGRES_EDIT, m_strImgRes);
	DDX_Control(pDX, IDC_VIDEO_LIST, m_ctrlVideoList);
	DDX_Check(pDX, IDC_DATE_CHECK, m_bDateFilter);
	DDX_DateTimeCtrl(pDX, IDC_START_DATEPICKER, m_odtStartDate);
	DDX_DateTimeCtrl(pDX, IDC_END_DATEPICKER, m_odtEndDate);
}


BEGIN_MESSAGE_MAP(CPatientListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_QUERY_BTN, &CPatientListDlg::OnBnClickedQueryBtn)
	ON_NOTIFY(NM_CLICK, IDC_PATIENT_LIST, &CPatientListDlg::OnClickPatientList)
	ON_NOTIFY(NM_DBLCLK, IDC_PATIENT_LIST, &CPatientListDlg::OnDblclkPatientList)
	ON_LBN_DBLCLK(IDC_IMG_LIST, &CPatientListDlg::OnDblclkImgList)
	ON_LBN_DBLCLK(IDC_VIDEO_LIST, &CPatientListDlg::OnDblclkVideoList)
	ON_BN_CLICKED(IDC_CLEARFILTER_BTN, &CPatientListDlg::OnBnClickedClearfilterBtn)
	ON_BN_CLICKED(IDC_DATE_CHECK, &CPatientListDlg::OnClickedDateCheck)
	ON_BN_CLICKED(IDC_OPENFOLDER_BTN, &CPatientListDlg::OnBnClickedOpenfolderBtn)
	ON_BN_CLICKED(IDOK, &CPatientListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPatientListDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CPatientListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowText(_T("患者记录"));
// 	m_ctrlPatientsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrlPatientsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT);
	// 	m_ctrlPatientsList.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_ctrlPatientsList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 36);
	m_ctrlPatientsList.InsertColumn(1, _T("患者ID"), LVCFMT_LEFT, 90);
	m_ctrlPatientsList.InsertColumn(2, _T("姓名"), LVCFMT_LEFT, 100);
	m_ctrlPatientsList.InsertColumn(3, _T("性别"), LVCFMT_LEFT, 36);
	m_ctrlPatientsList.InsertColumn(4, _T("年龄"), LVCFMT_LEFT, 36);
	m_ctrlPatientsList.InsertColumn(5, _T("日期"), LVCFMT_LEFT, 130);

	((CButton*)(GetDlgItem(IDC_DATE_CHECK)))->SetCheck(BST_UNCHECKED);
	GetDlgItem(IDC_START_DATEPICKER)->EnableWindow(FALSE);
	GetDlgItem(IDC_END_DATEPICKER)->EnableWindow(FALSE);

	static_cast<CEdit*>(GetDlgItem(IDC_COPYRIGHT_EDIT))->SetWindowText(_T("Copyright © 2021 中日友好医院&国家呼吸医学中心 All Rights Reserved."));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

BOOL CPatientListDlg::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	
	return CDialogEx::DestroyWindow();
}
// CPatientListDlg 消息处理程序


void CPatientListDlg::OnBnClickedQueryBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_patients.clear();

	m_ctrlPatientsList.DeleteAllItems();
	ClearImgList();
	ClearVideoList();

	m_strDiagnosis = _T("");
	m_strImgRes = _T("");

	UpdateData(FALSE);

	USES_CONVERSION;

	double d1 = 0.0f;
	double d2 = 0.0f;
	if (m_bDateFilter)
	{
		d1 = m_odtStartDate.m_dt;
		d2 = m_odtEndDate.m_dt;
	}

	CDataPersistence::getInstance()->QueryPatients(m_patients, W2A(m_strId), W2A(m_strName), GetGender(), d1, d2);

	CString strIdx = _T("");
	CString strAge = _T("");
// 	for (int j = 0; j < 20; ++j)
	for (unsigned int i = 0; i < m_patients.size(); ++i)
	{
		strIdx.Format(_T("%d"), i + 1);
		strAge.Format(_T("%d"), m_patients[i].m_nAge);
		m_ctrlPatientsList.InsertItem(i, strIdx);
		m_ctrlPatientsList.SetItemText(i, 1, CString(m_patients[i].m_strId.c_str()));
		m_ctrlPatientsList.SetItemText(i, 2, CString(m_patients[i].m_strName.c_str()));
		m_ctrlPatientsList.SetItemText(i, 3, CString(m_patients[i].m_strGender.c_str()));
		m_ctrlPatientsList.SetItemText(i, 4, strAge);
		m_ctrlPatientsList.SetItemText(i, 5, m_patients[i].GetDateTimeString());
	}
}

void CPatientListDlg::OnBnClickedClearfilterBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strId = "";
	m_strName = "";
	m_nGender = 0;
	m_odtStartDate.m_dt = 43466.0f;

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_odtEndDate.SetDateTime(st.wYear, st.wMonth, st.wDay, 23, 59, 59);

	UpdateData(FALSE);
}

void CPatientListDlg::OnClickPatientList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	m_nCurrSelectIdx = pNMItemActivate->iItem;

	ShowExtendINfo();
}

void CPatientListDlg::OnDblclkPatientList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	m_nCurrSelectIdx = pNMItemActivate->iItem;

	ShowExtendINfo();

	if (m_nCurrSelectIdx >= 0)
	{		
		m_patientInfoDlg.SetPatientInfo(m_patients[m_nCurrSelectIdx]);
		if (IDOK == m_patientInfoDlg.DoModal())
		{
			m_patients[m_nCurrSelectIdx] = m_patientInfoDlg.m_patientInfo;

			CString strIdx = _T("");
			CString strAge = _T("");
			// 	for (int j = 0; j < 20; ++j)
			strIdx.Format(_T("%d"), m_nCurrSelectIdx);
			strAge.Format(_T("%d"), m_patients[m_nCurrSelectIdx].m_nAge);
			m_ctrlPatientsList.SetItemText(m_nCurrSelectIdx, 1, CString(m_patients[m_nCurrSelectIdx].m_strId.c_str()));
			m_ctrlPatientsList.SetItemText(m_nCurrSelectIdx, 2, CString(m_patients[m_nCurrSelectIdx].m_strName.c_str()));
			m_ctrlPatientsList.SetItemText(m_nCurrSelectIdx, 3, CString(m_patients[m_nCurrSelectIdx].m_strGender.c_str()));
			m_ctrlPatientsList.SetItemText(m_nCurrSelectIdx, 4, strAge);
			m_ctrlPatientsList.SetItemText(m_nCurrSelectIdx, 5, m_patients[m_nCurrSelectIdx].GetDateTimeString());

			ShowExtendINfo();
		}
	}
}

void CPatientListDlg::OnDblclkImgList()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurrSelectIdx >= 0)
	{
		std::string strPath = CDataPersistence::getInstance()->m_strRootPath + m_patients[m_nCurrSelectIdx].m_strPath;

		CString strFile = _T("");
		m_ctrlImgList.GetText(m_ctrlImgList.GetCurSel(), strFile);

		ShellExecute(NULL, _T("open"), CString(strPath.c_str()) + strFile, NULL, NULL, SW_SHOW);
	}
}


void CPatientListDlg::OnDblclkVideoList()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurrSelectIdx >= 0)
	{
		std::string strPath = CDataPersistence::getInstance()->m_strRootPath + m_patients[m_nCurrSelectIdx].m_strPath;

		CString strFile = _T("");
		m_ctrlVideoList.GetText(m_ctrlVideoList.GetCurSel(), strFile);

		ShellExecute(NULL, _T("open"), CString(strPath.c_str()) + strFile, NULL, NULL, SW_SHOW);
	}
}

void CPatientListDlg::OnClickedDateCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bDateFilter)
	{
		GetDlgItem(IDC_START_DATEPICKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_END_DATEPICKER)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_START_DATEPICKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_END_DATEPICKER)->EnableWindow(FALSE);
	}
}


void CPatientListDlg::OnBnClickedOpenfolderBtn()
{
	// TODO:  在此添加控件通知处理程序代码
// 	system("@start F:\\WallPaper");
	std::string strPath = CDataPersistence::getInstance()->m_strRootPath + "Archives\\";
	
	if (m_nCurrSelectIdx >= 0)
	{
		strPath = CDataPersistence::getInstance()->m_strRootPath + m_patients[m_nCurrSelectIdx].m_strPath;
	}

	ShellExecute(NULL, _T("explore"), CString(strPath.c_str()), NULL, NULL, SW_SHOWNORMAL);
}


void CPatientListDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurrSelectIdx >= 0)
	{
		CDataPersistence::getInstance()->SetCurrentPatient(m_patients[m_nCurrSelectIdx]);
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_UPDATE_PATIENT_INFO, 0, 0);
	}

	CDialogEx::OnOK();
}


void CPatientListDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CPatientListDlg::ClearDlg(BOOL bSelect)
{
	OnBnClickedClearfilterBtn();

	ClearImgList();
	ClearVideoList();

	m_strDiagnosis = _T("");
	m_strImgRes = _T("");
	m_ctrlPatientsList.DeleteAllItems();

	m_patients.clear();
	m_nCurrSelectIdx = -1;

	UpdateData(FALSE);

	GetDlgItem(IDOK)->EnableWindow(bSelect);
}

void CPatientListDlg::ShowExtendINfo()
{
	ClearImgList();
	ClearVideoList();

	if (m_nCurrSelectIdx >= 0)
	{
		USES_CONVERSION;
		m_strDiagnosis = CString(m_patients[m_nCurrSelectIdx].m_strDiagnosis.c_str());
		m_strImgRes = CString(m_patients[m_nCurrSelectIdx].m_strImgResult.c_str());

		for (unsigned int i = 0; i < m_patients[m_nCurrSelectIdx].m_imgs.size(); ++i)
		{
			m_ctrlImgList.AddString(CString(m_patients[m_nCurrSelectIdx].m_imgs[i].c_str()));
		}

		for (unsigned int i = 0; i < m_patients[m_nCurrSelectIdx].m_videos.size(); ++i)
		{
			m_ctrlVideoList.AddString(CString(m_patients[m_nCurrSelectIdx].m_videos[i].c_str()));
		}
	}
	else
	{
		m_strDiagnosis = _T("");
		m_strImgRes = _T("");
	}

	UpdateData(FALSE);
}

void CPatientListDlg::ClearImgList()
{
	int count = m_ctrlImgList.GetCount();
	for (int i = 0; i < count; ++i)
	{
		m_ctrlImgList.DeleteString(0);
	}
	count = m_ctrlImgList.GetCount();
}

void CPatientListDlg::ClearVideoList()
{
	int count = m_ctrlVideoList.GetCount();
	for (int i = 0; i < count; ++i)
	{
		m_ctrlVideoList.DeleteString(0);
	}
}

std::string CPatientListDlg::GetGender()
{
	std::string strGender = "";

	switch (m_nGender)
	{
		case 1:
		{
			strGender = "男";
			break;
		}
		case 2:
		{
			strGender = "女";
			break;
		}
		default:
		{
			strGender = "";
			break;
		}
	}

	return strGender;
}
