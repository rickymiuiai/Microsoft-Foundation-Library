#include "stdafx.h"
#include "CommonFunction.h"

#include <io.h>
#include <direct.h>


bool AlexCreateDirectory(char* pPath)
{
	bool bRet = false;

	if (pPath != NULL)
	{
		size_t nPathLen = strlen(pPath);

		if (nPathLen > 0)
		{
			size_t nBufSize = nPathLen + 8;
			char* pBuf = new char[nBufSize];

			if (pBuf != NULL)
			{
				memset(pBuf, 0, nBufSize);

				for (size_t i = 0; i < nPathLen; ++i)
				{
					pBuf[i] = pPath[i];

					if ('\\' == pBuf[i] || '/' == pBuf[i])
					{
						if (_access(pBuf, 0) != 0)
						{
							if (_mkdir(pBuf) != 0)
							{
								return false;
							}
						}
					}
				}

				if (_access(pBuf, 0) != 0)
				{
					if (_mkdir(pBuf) == 0)
					{
						bRet = true;
					}
				}
				else
				{
					bRet = true;
				}
			}
		}
	}

	return bRet;
}

std::string GetDateTime()
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	char szBuf[32] = { 0 };
	sprintf_s(szBuf, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return std::string(szBuf);
}

std::string GetDateTime_x1()
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	char szBuf[32] = { 0 };
	sprintf_s(szBuf, "%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return std::string(szBuf);
}

std::string AlxGetCurrentPath()
{
	char szPath[MAX_PATH] = { 0 };
	char szDrive[_MAX_DRIVE] = { 0 };
	char szDir[_MAX_DIR] = { 0 };
	char szFname[_MAX_FNAME] = { 0 };
	char szExt[_MAX_EXT] = { 0 };

	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	_splitpath_s(szPath, szDrive, szDir, szFname, szExt);

	memset(szPath, 0, MAX_PATH);
	sprintf_s(szPath, MAX_PATH, "%s%s", szDrive, szDir);

	return szPath;
}

bool DateTimeIsValid(CString strDate, CString startDate, CString endDate)
{
	bool isValid = false;

	COleDateTime oleDate;
	COleDateTime startOleDate;
	COleDateTime endOleDate;

	oleDate.ParseDateTime(strDate);
	startOleDate.ParseDateTime(startDate);
	endOleDate.ParseDateTime(endDate);

	isValid = (oleDate >= startOleDate) && (oleDate <= endOleDate);

	return isValid;
}

bool CompareDateTime(CString strEarlier, CString strLater)
{
	bool ret = true;

	if ((!strEarlier.IsEmpty()) && (!strLater.IsEmpty()))
	{
		COleDateTime odtEarlier;
		COleDateTime odtLater;

		odtEarlier.ParseDateTime(strEarlier);
		odtLater.ParseDateTime(strLater);

		ret = odtEarlier <= odtLater;
	}
	
	return ret;
}
