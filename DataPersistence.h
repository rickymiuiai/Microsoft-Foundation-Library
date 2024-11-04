#pragma once

#include "Persistence/DBMiddleWare.h"

enum DP_PATIENT_STATUS
{
	DP_UN_REG,
	DP_NEW_REG,
	DP_MERGE_INFO,
	DP_EXIST_INFO,
	DP_COUNT
};

class CDataPersistence
{
public:
	virtual ~CDataPersistence();

	static CDataPersistence* getInstance()
	{
		static CDataPersistence _instance;
		return &_instance;
	}

private:
	CDataPersistence();

public:
	int  RegNewPatient(CPatientInfo& info);
	bool MergeUpdateInfo(CPatientInfo& info);
	bool SelectPatientInfo(std::string strId);
	int  QueryPatients(std::vector<CPatientInfo>& patients, std::string strId = "", std::string strName = "", std::string strGender = "", double beginDate = 0.0f, double endDate = 0.0f, bool bFuzzy = true);
	bool UpdatePatient(CPatientInfo& info);
	void SetCurrentPatient(CPatientInfo& info);
	void UPdateNewFileRecord(std::string strFile, int type);
	bool IsSave();
	bool IsValid();
	void MoveAttachment(CPatientInfo& src, CPatientInfo& des);
	std::string Generate1ImgName();
	std::string GenerateImgFullName(std::string ext);
	std::string GetFullFilePath(std::string strFile);
	std::string GetFullPath();

private:
	void WriteArchivesFile(CPatientInfo& info);

public:
	DP_PATIENT_STATUS        m_emStatus;
	bool                     m_bInfoValid;
	std::string              m_strRootPath;
	std::string              m_strImgName;
	std::vector<std::string> m_tmpImgs;
	std::vector<std::string> m_tmpVideos;
	CDBMiddleWare            m_dbMw;
	CPatientInfo             m_currentPatient;
	
};

