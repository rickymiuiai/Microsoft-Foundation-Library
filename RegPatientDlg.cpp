// RegPatientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "RegPatientDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include "DataPersistence.h"
#include "CommonFunction.h"


// CRegPatientDlg 对话框

IMPLEMENT_DYNAMIC(CRegPatientDlg, CDialogEx)

CRegPatientDlg::CRegPatientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegPatientDlg::IDD, pParent)
	, m_unAge(0)
	, m_nGender(0)
	, m_strDate(_T(""))
	, m_strDiagnosis(_T(""))
	, m_strPatientId(_T(""))
	, m_strImgRes(_T(""))
	, m_strName(_T(""))
{

}

CRegPatientDlg::~CRegPatientDlg()
{
}

void CRegPatientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AGE_EDIT, m_unAge);
	DDV_MinMaxUInt(pDX, m_unAge, 0, 150);
	DDX_Text(pDX, IDC_DATE_STATIC, m_strDate);
	DDX_Text(pDX, IDC_DIAGNOSIS_EDIT, m_strDiagnosis);
	DDX_Text(pDX, IDC_ID_EDIT, m_strPatientId);
	DDX_Text(pDX, IDC_IMGRES_EDIT, m_strImgRes);
	DDX_Radio(pDX, IDC_MALE_RADIO, m_nGender);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
}


BEGIN_MESSAGE_MAP(CRegPatientDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegPatientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegPatientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRegPatientDlg 消息处理程序
BOOL CRegPatientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(_T("患者登记"));

	static_cast<CEdit*>(GetDlgItem(IDC_COPYRIGHT_EDIT))->SetWindowText(_T("Copyright © 2021 中日友好医院&国家呼吸医学中心 All Rights Reserved."));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CRegPatientDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_strPatientId.IsEmpty())
	{
		AfxMessageBox(_T("患者ID不能为空！"), MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
	}
	else
	{
		USES_CONVERSION;
		std::string strId = W2A(m_strPatientId);
		CPatientInfo info(strId, W2A(m_strName), GetGender(), m_unAge, W2A(m_strDiagnosis), W2A(m_strImgRes), GetDateTimeD(), GetPatientPath(strId));
		
		int ret = CDataPersistence::getInstance()->RegNewPatient(info);

		switch (ret)
		{
			case 0:
			{
				Counter(strId);
				::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_UPDATE_PATIENT_INFO, 0, 0);
				CDialogEx::OnOK();
				break;
			}
			case 1:
			{
				int btn = AfxMessageBox(_T("患者信息已经存在！是否合并更新？\r\n选择<YES>合并；\r\n选择<NO>使用原有患者信息；\r\n选择<CANCEL>修改后再登记"), MB_YESNOCANCEL | MB_SYSTEMMODAL | MB_ICONQUESTION);
				if (IDYES == btn)
				{
					// 合并更新
					if (CDataPersistence::getInstance()->MergeUpdateInfo(info))
					{
						::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_UPDATE_PATIENT_INFO, 0, 0);
						CDialogEx::OnOK();
					}
					else
					{
						AfxMessageBox(_T("登记失败！"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
					}

				}
				else if (IDNO == btn)
				{
					// 直接使用原有患者信息
					if (CDataPersistence::getInstance()->SelectPatientInfo(info.m_strId))
					{
						::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_UPDATE_PATIENT_INFO, 0, 0);
						CDialogEx::OnOK();
					}
					else
					{
						AfxMessageBox(_T("登记失败！"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
					}
				}
				else
				{
					// None
				}
				break;
			}
			case 2:
			{
				int btn = AfxMessageBox(_T("患者ID已存在！"), MB_OK | MB_SYSTEMMODAL | MB_ICONQUESTION);
				break;
			}
			default:
			{
				AfxMessageBox(_T("登记失败！"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
				break;
			}
		}
	}
	
	
}


void CRegPatientDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CRegPatientDlg::ClearDlg()
{
	m_strPatientId = _T("");
	m_strName = _T("");
	m_strDate = _T("");
	m_strDiagnosis = _T("");
	m_strImgRes = _T("");
	m_nGender = 0;
	m_unAge = 0;
	UpdateData(FALSE);
}

double CRegPatientDlg::GetDateTimeD()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dt(st);

	return dt.m_dt;
}

std::string CRegPatientDlg::GetGender()
{
	std::string strGender = "";

	if (0 == m_nGender)
	{
		strGender = "男";
	}
	else if (1 == m_nGender)
	{
		strGender = "女";
	}

	return strGender;
}

std::string CRegPatientDlg::GetPatientPath(std::string strId)
{
	std::string strPath = "";

	std::string subPath = "Archives\\" + strId + "\\";
	std::string fullPath = CDataPersistence::getInstance()->m_strRootPath + subPath;

	if (AlexCreateDirectory(const_cast<char*>(fullPath.c_str())))
	{
		strPath = subPath;
	}

	return strPath;
}

void CRegPatientDlg::Counter(std::string strPatientId)
{
	FILE* fpCounter = nullptr;
	FILE* fpCounterInfo = nullptr;

	fopen_s(&fpCounter, "CounterPatient.txt", "r");
	fopen_s(&fpCounterInfo, "CounterPatientInfo.txt", "a+");

	int counter = 1;
	char buf[128] = { 0 };

	if (fpCounter != nullptr)
	{
		fseek(fpCounter, 0, SEEK_SET);
		fread(buf, 1, 128, fpCounter);
		counter = atoi(buf);

		++counter;
		fclose(fpCounter);
	}

	fopen_s(&fpCounter, "CounterPatient.txt", "w");
	if (fpCounter != nullptr)
	{
		memset(buf, 0, 128);
		sprintf_s(buf, "%d", counter);
		fwrite(buf, 1, strlen(buf), fpCounter);
		fclose(fpCounter);
	}

	if (fpCounterInfo != nullptr)
	{
		memset(buf, 0, 128);

		SYSTEMTIME st;

		GetLocalTime(&st);
		sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d 登记患者 [%s] 计数 [%d]\n", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, 
			strPatientId.c_str(), counter);
		fwrite(buf, 1, strlen(buf), fpCounterInfo);
		fclose(fpCounterInfo);
	}
}


