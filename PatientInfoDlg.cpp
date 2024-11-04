// PatientInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "PatientInfoDlg.h"
#include "afxdialogex.h"
#include "DataPersistence.h"


// CPatientInfoDlg 对话框

IMPLEMENT_DYNAMIC(CPatientInfoDlg, CDialogEx)

CPatientInfoDlg::CPatientInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatientInfoDlg::IDD, pParent)
	, m_strDiagnosis(_T(""))
	, m_strId(_T(""))
	, m_strImgRes(_T(""))
	, m_nGender(0)
	, m_strName(_T(""))
	, m_strDate(_T(""))
	, m_nAge(0)
	, m_patientInfo()
{

}

CPatientInfoDlg::~CPatientInfoDlg()
{
}

void CPatientInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_AGE_EDIT, m_nAge);
	DDX_Text(pDX, IDC_DIAGNOSIS_EDIT, m_strDiagnosis);
	DDX_Text(pDX, IDC_ID_EDIT, m_strId);
	DDX_Control(pDX, IDC_IMG_LIST, m_ctrlImgList);
	DDX_Text(pDX, IDC_IMGRES_EDIT, m_strImgRes);
	DDX_Radio(pDX, IDC_MALE_RADIO, m_nGender);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	DDX_Control(pDX, IDC_VIDEO_LIST, m_ctrlVideoList);
	DDX_Text(pDX, IDC_DATE_STATIC, m_strDate);
	DDX_Text(pDX, IDC_AGE_EDIT, m_nAge);
}


BEGIN_MESSAGE_MAP(CPatientInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPatientInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPatientInfoDlg::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_IMG_LIST, &CPatientInfoDlg::OnDblclkImgList)
	ON_LBN_DBLCLK(IDC_VIDEO_LIST, &CPatientInfoDlg::OnDblclkVideoList)
END_MESSAGE_MAP()


// CPatientInfoDlg 消息处理程序
BOOL CPatientInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowText(_T("修改患者信息"));

	ClearImageListBox();
	ClearVideoListBox();

	m_strId = CString(m_patientInfo.m_strId.c_str());
	m_strName = CString(m_patientInfo.m_strName.c_str());
	m_nAge = m_patientInfo.m_nAge;
	m_nGender = GenderIdx(m_patientInfo.m_strGender);
	m_strDiagnosis = CString(m_patientInfo.m_strDiagnosis.c_str());
	m_strImgRes = CString(m_patientInfo.m_strImgResult.c_str());
	m_strDate = m_patientInfo.GetDateTimeString();

	for (unsigned int i = 0; i < m_patientInfo.m_imgs.size(); ++i)
	{
		m_ctrlImgList.AddString(CString(m_patientInfo.m_imgs[i].c_str()));
	}

	for (unsigned int i = 0; i < m_patientInfo.m_videos.size(); ++i)
	{
		m_ctrlVideoList.AddString(CString(m_patientInfo.m_videos[i].c_str()));
	}

	static_cast<CEdit*>(GetDlgItem(IDC_COPYRIGHT_EDIT))->SetWindowText(_T("Copyright © 2021 中日友好医院&国家呼吸医学中心 All Rights Reserved."));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CPatientInfoDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	USES_CONVERSION;

	m_patientInfo.m_strId = W2A(m_strId);
	m_patientInfo.m_strName = W2A(m_strName);
	m_patientInfo.m_strGender = GetGender();
	m_patientInfo.m_nAge = m_nAge;
	m_patientInfo.m_strDiagnosis = W2A(m_strDiagnosis);
	m_patientInfo.m_strImgResult = W2A(m_strImgRes);

	if (CDataPersistence::getInstance()->UpdatePatient(m_patientInfo))
	{
		CDialogEx::OnOK();
	}
	else
	{
		AfxMessageBox(_T("修改失败！"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
	}
}


void CPatientInfoDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CPatientInfoDlg::SetPatientInfo(CPatientInfo& info)
{
	m_patientInfo = info;
}

int CPatientInfoDlg::GenderIdx(std::string str)
{
	int ret = -1;

	if (str == "男")
	{
		ret = 0;
	}
	else if (str == "女")
	{
		ret = 1;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

std::string CPatientInfoDlg::GetGender()
{
	std::string strGender = "";

	switch (m_nGender)
	{
		case 0:
		{
			strGender = "男";
			break;
		}
		case 1:
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

void CPatientInfoDlg::ClearImageListBox()
{
	int count = m_ctrlImgList.GetCount();

	for (int i = 0; i < count; ++i)
	{
		m_ctrlImgList.DeleteString(0);
	}
}

void CPatientInfoDlg::ClearVideoListBox()
{
	int count = m_ctrlVideoList.GetCount();

	for (int i = 0; i < count; ++i)
	{
		m_ctrlVideoList.DeleteString(0);
	}
}

void CPatientInfoDlg::OnDblclkImgList()
{
	// TODO:  在此添加控件通知处理程序代码
	std::string strPath = CDataPersistence::getInstance()->m_strRootPath + m_patientInfo.m_strPath;

	CString strFile = _T("");
	m_ctrlImgList.GetText(m_ctrlImgList.GetCurSel(), strFile);

	ShellExecute(NULL, _T("open"), CString(strPath.c_str()) + strFile, NULL, NULL, SW_SHOW);
}


void CPatientInfoDlg::OnDblclkVideoList()
{
	// TODO:  在此添加控件通知处理程序代码
	std::string strPath = CDataPersistence::getInstance()->m_strRootPath + m_patientInfo.m_strPath;

	CString strFile = _T("");
	m_ctrlVideoList.GetText(m_ctrlVideoList.GetCurSel(), strFile);

	ShellExecute(NULL, _T("open"), CString(strPath.c_str()) + strFile, NULL, NULL, SW_SHOW);
}
