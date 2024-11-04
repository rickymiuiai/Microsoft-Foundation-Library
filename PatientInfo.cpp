#include "stdafx.h"
#include "PatientInfo.h"


CPatientInfo::CPatientInfo() : 
    m_nAge(0)
  , m_fDate(0.0f)
  , m_strPath("")
  , m_strId("")
  , m_strName("")
  , m_strGender("")
  , m_strDiagnosis("")
  , m_strImgResult("")
  , m_imgs()
  , m_videos()
{
	
}

CPatientInfo::CPatientInfo(const char* szId, const char* szName, const char* szGender, unsigned int age) : 
	m_nAge(age)
  , m_fDate(0.0f)
  , m_strPath("")
  , m_strId("")
  , m_strName("")
  , m_strGender("")
  , m_strDiagnosis("")
  , m_strImgResult("")
  , m_imgs()
  , m_videos()
{
	if (szId != nullptr)     m_strId = szId;
	if (szName != nullptr)   m_strName = szName;
	if (szGender != nullptr) m_strGender = szGender;
}

CPatientInfo::CPatientInfo(std::string strId, std::string strName, std::string strGender, unsigned int age, std::string diagnosis /* = "" */, std::string imgRes /* = "" */, double fDate /* = 0.0f */, std::string strPath /* = "" */) :
    m_nAge(age)
  , m_fDate(fDate)
  , m_strPath(strPath)
  , m_strId(strId)
  , m_strName(strName)
  , m_strGender(strGender)
  , m_strDiagnosis(diagnosis)
  , m_strImgResult(imgRes)
  , m_imgs()
  , m_videos()
{
	
}


CPatientInfo::~CPatientInfo()
{
}

CString CPatientInfo::GetDateTimeString(CString def /* = _T("") */)
{
	CString str = def;

	if (m_fDate > 0.0f)
	{
		COleDateTime odt1(m_fDate);
		str = odt1.Format(_T("%Y-%m-%d %H:%M:%S"));
	}

	return str;
}

bool CPatientInfo::HasAttachment()
{
	return (m_videos.size() > 0) || (m_imgs.size() > 0);
}
