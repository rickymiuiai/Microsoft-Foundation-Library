#pragma once
#include "stdafx.h"
#include <string>


bool AlexCreateDirectory(char* pPath);

std::string GetDateTime();
std::string GetDateTime_x1();

std::string AlxGetCurrentPath();

bool DateTimeIsValid(CString strDate, CString startDate, CString endDate);
bool CompareDateTime(CString strEarlier, CString strLater);
