#pragma once
#include "stdafx.h"

#include <string>
#include <list>
#include <vector>

class CPatientInfo
{
public:
	CPatientInfo();
	CPatientInfo(const char* szId, const char* szName, const char* szGender, unsigned int age);
	CPatientInfo(std::string strId, std::string strName, std::string strGender, unsigned int age, std::string diagnosis = "", std::string imgRes = "", double fDate = 0.0f, std::string strPath = "");
	virtual ~CPatientInfo();

public:
	CString GetDateTimeString(CString def = _T(""));
	bool HasAttachment();

public:
	unsigned int             m_nAge;
	double                   m_fDate;
	std::string              m_strPath;
	std::string              m_strId;
	std::string              m_strName;
	std::string              m_strGender;
	std::string              m_strDiagnosis;
	std::string              m_strImgResult;
	std::vector<std::string> m_imgs;
	std::vector<std::string> m_videos;
};

