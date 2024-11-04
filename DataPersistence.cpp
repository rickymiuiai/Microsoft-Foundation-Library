#include "stdafx.h"
#include "DataPersistence.h"
#include "CommonFunction.h"


CDataPersistence::CDataPersistence() : 
	m_emStatus(DP_UN_REG)
  , m_strRootPath("")
  , m_strImgName("")
  , m_tmpImgs()
  , m_tmpVideos()
  , m_dbMw()
  , m_currentPatient()
{
	m_strRootPath = AlxGetCurrentPath();

	m_bInfoValid = false;
	m_currentPatient.m_strPath = "Archives\\Temp\\";

	std::string fullPath = m_strRootPath + m_currentPatient.m_strPath;
	AlexCreateDirectory(const_cast<char*>(fullPath.c_str()));
}


CDataPersistence::~CDataPersistence()
{
}

int CDataPersistence::RegNewPatient(CPatientInfo& info)
{
	bool isSave = IsSave();

	if (!isSave)
	{
		info.m_imgs = m_currentPatient.m_imgs;
		info.m_videos = m_currentPatient.m_videos;
	}

	int ret = m_dbMw.InsertNewPatient(info);
	if (0 == ret)
	{
		WriteArchivesFile(info);
		if (!isSave)
		{
			// 移动文件
			MoveAttachment(m_currentPatient, info);
		}
		m_currentPatient = info;
		m_bInfoValid = true;
	}

	return ret;
}

bool CDataPersistence::MergeUpdateInfo(CPatientInfo& info)
{
	bool ret = false;

	// 查询数据库中的患者详情
	std::vector<CPatientInfo> patients;
	int count = m_dbMw.QueryPatientInfo(patients, info.m_strId);

	if (count > 0)
	{
		CPatientInfo temp;
		temp = patients[0];

		// 合并
		temp.m_strDiagnosis += "\n" + info.m_strDiagnosis;
		temp.m_strImgResult += "\n" + info.m_strImgResult;

		bool isSave = IsSave();
		if (!isSave)
		{
			temp.m_imgs.insert(temp.m_imgs.begin(), m_currentPatient.m_imgs.begin(), m_currentPatient.m_imgs.end());
			temp.m_videos.insert(temp.m_videos.begin(), m_currentPatient.m_videos.begin(), m_currentPatient.m_videos.end());
		}

		// 更新写入数据库
		ret = m_dbMw.AlterPatientInfo(temp);

		if (ret)
		{
			WriteArchivesFile(temp);
			if (!isSave)
			{
				// 移动文件
				MoveAttachment(m_currentPatient, temp);
			}
			m_currentPatient = temp;
			m_bInfoValid = true;
		}
	}

	return ret;
}

bool CDataPersistence::SelectPatientInfo(std::string strId)
{
	bool ret = false;

	// 查询数据库中的患者详情
	std::vector<CPatientInfo> patients;
	int count = m_dbMw.QueryPatientInfo(patients, strId);

	if (count > 0)
	{
		if (!IsSave())
		{
			CPatientInfo tmp = patients[0];
			tmp.m_imgs.insert(m_currentPatient.m_imgs.begin(), m_currentPatient.m_imgs.begin(), m_currentPatient.m_imgs.end());
			tmp.m_videos.insert(m_currentPatient.m_videos.begin(), m_currentPatient.m_videos.begin(), m_currentPatient.m_videos.end());

			if (m_dbMw.AlterPatientInfo(tmp))
			{
				// 移动文件
				WriteArchivesFile(tmp);
				MoveAttachment(m_currentPatient, tmp);
				m_currentPatient = tmp;
				ret = true;
				m_bInfoValid = true;
			}
		}
		else
		{
			m_currentPatient = patients[0];
			ret = true;
			m_bInfoValid = true;
		}
		
	}

	return ret;
}

int CDataPersistence::QueryPatients(std::vector<CPatientInfo>& patients, std::string strId /* = "" */, std::string strName /* = "" */, std::string strGender /* = "" */, double beginDate /* = 0.0f */, double endDate /* = 0.0f */, bool bFzzy /* = true */)
{
	return m_dbMw.QueryPatientInfo(patients, strId, strName, strGender, beginDate, endDate, bFzzy);
}

bool CDataPersistence::UpdatePatient(CPatientInfo& info)
{
	WriteArchivesFile(info);
	return m_dbMw.AlterPatientInfo(info);
}

void CDataPersistence::SetCurrentPatient(CPatientInfo& info)
{
	bool isSave = IsSave();

	if (isSave)
	{
		m_currentPatient = info;

		m_bInfoValid = true;
	}
	else
	{
		CPatientInfo tmp = info;
		tmp.m_imgs.insert(tmp.m_imgs.begin(), m_currentPatient.m_imgs.begin(), m_currentPatient.m_imgs.end());
		tmp.m_videos.insert(tmp.m_videos.begin(), m_currentPatient.m_videos.begin(), m_currentPatient.m_videos.end());

		if (UpdatePatient(tmp))
		{
			// 移动文件
			MoveAttachment(m_currentPatient, tmp);
			m_currentPatient = tmp;
			m_bInfoValid = true;
		}
	}
	
}

void CDataPersistence::UPdateNewFileRecord(std::string strFile, int type)
{
	switch (type)
	{
		case 0:
		{
			m_currentPatient.m_imgs.push_back(strFile);
			break;
		}
		case 1:
		{
			m_currentPatient.m_videos.push_back(strFile);
			break;
		}
		default:
		{
			break;
		}
	}

	if (m_bInfoValid)
	{
		WriteArchivesFile(m_currentPatient);
		m_dbMw.AlterPatientInfo(m_currentPatient);
	}
}

bool CDataPersistence::IsSave()
{
	bool ret = true;

	ret = !((false == m_bInfoValid) && (m_currentPatient.HasAttachment()));

	return ret;
}

bool CDataPersistence::IsValid()
{
	return m_bInfoValid;
}

void CDataPersistence::MoveAttachment(CPatientInfo& src, CPatientInfo& des)
{
	std::string strSrc = "";
	std::string strDes = "";
	for (unsigned int i = 0; i < src.m_imgs.size(); ++i)
	{
		strSrc = m_strRootPath + src.m_strPath + src.m_imgs[i];
		strDes = m_strRootPath + des.m_strPath + des.m_imgs[i];

		CString cstrSrc(strSrc.c_str());
		CString cstrDes(strDes.c_str());
		MoveFile(cstrSrc, cstrDes);

		cstrSrc.Replace(_T(".png"), _T("_o.jpg"));
		cstrDes.Replace(_T(".png"), _T("_o.jpg"));
		MoveFile(cstrSrc, cstrDes);
	}

	for (unsigned int i = 0; i < src.m_videos.size(); ++i)
	{
		strSrc = m_strRootPath + src.m_strPath + src.m_videos[i];
		strDes = m_strRootPath + des.m_strPath + des.m_videos[i];

		MoveFile(CString(strSrc.c_str()), CString(strDes.c_str()));
	}
}

std::string CDataPersistence::Generate1ImgName()
{
	m_strImgName = GetDateTime_x1();

	return m_strImgName;
}

std::string CDataPersistence::GenerateImgFullName(std::string ext)
{
	std::string fullPath = ""; 

	fullPath = m_strRootPath + m_currentPatient.m_strPath + m_strImgName + ext;
	
	return fullPath;
}

std::string CDataPersistence::GetFullFilePath(std::string strFile)
{
	std::string fullPath = "";

	fullPath = m_strRootPath + m_currentPatient.m_strPath + strFile;
	
	return fullPath;
}

std::string CDataPersistence::GetFullPath()
{
	return m_strRootPath + m_currentPatient.m_strPath;
}

// helper
void CDataPersistence::WriteArchivesFile(CPatientInfo& info)
{
	FILE* fp = NULL;

	std::string file = m_strRootPath + info.m_strPath + info.m_strId + ".txt";
	if (0 == fopen_s(&fp, file.c_str(), "w"))
	{
		if (fp != NULL)
		{
			/*fwrite("", 1, 1, fp);*/
			fprintf_s(fp, "患者信息\n");
			fprintf_s(fp, "-------------------------------------------------------------\n");
			fprintf_s(fp, "患者  ID：%s\n", info.m_strId.c_str());
			fprintf_s(fp, "姓      名：%s\n", info.m_strName.c_str());
			fprintf_s(fp, "性      别：%s\n", info.m_strGender.c_str());
			fprintf_s(fp, "年      龄：%d\n", info.m_nAge);
			fprintf_s(fp, "诊      断：%s\n", info.m_strDiagnosis.c_str());
			fprintf_s(fp, "影像结果：%s\n", info.m_strImgResult.c_str());
			fprintf_s(fp, "-------------------------------------------------------------\n");
			fprintf_s(fp, "附件\n");			
			fprintf_s(fp, "录像\n");
			fprintf_s(fp, "*************************************************************\n");
			for (unsigned int i = 0; i < info.m_videos.size(); ++i)
			{
				fprintf_s(fp, "%s\n", info.m_videos[i].c_str());
			}
			fprintf_s(fp, "*************************************************************\n");
			fprintf_s(fp, "截图\n");
			fprintf_s(fp, "*************************************************************\n");
			for (unsigned int i = 0; i < info.m_imgs.size(); ++i)
			{
				fprintf_s(fp, "%s\n", info.m_imgs[i].c_str());
			}
			fprintf_s(fp, "*************************************************************\n");

			fflush(fp);
			fclose(fp);
			fp = NULL;
		}
	}
}