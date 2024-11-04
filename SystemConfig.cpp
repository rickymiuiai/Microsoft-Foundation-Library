#include "stdafx.h"
#include "SystemConfig.h"

CSystemConfig* CSystemConfig::_instance = new CSystemConfig();


CSystemConfig::CSystemConfig()
{
}


CSystemConfig::~CSystemConfig()
{
}

void CSystemConfig::LoadConfig()
{
	// Video Capture Card
	Type = ::GetPrivateProfileInt(_T("VideoCapture"), _T("Type"), 0, _T(".//SystemConfig.ini"));
	UsbIdx = ::GetPrivateProfileInt(_T("VideoCapture"), _T("UsbIdx"), 0, _T(".//SystemConfig.ini"));
	VideoW = ::GetPrivateProfileInt(_T("VideoCapture"), _T("Width"), 1920, _T(".//SystemConfig.ini"));
	VideoH = ::GetPrivateProfileInt(_T("VideoCapture"), _T("Height"), 1080, _T(".//SystemConfig.ini"));

	// ROI
	GrabX = ::GetPrivateProfileInt(_T("ROI"), _T("X"), 0, _T(".//SystemConfig.ini"));
	GrabY = ::GetPrivateProfileInt(_T("ROI"), _T("Y"), 0, _T(".//SystemConfig.ini"));
	GrabW = ::GetPrivateProfileInt(_T("ROI"), _T("W"), 1920, _T(".//SystemConfig.ini"));
	GrabH = ::GetPrivateProfileInt(_T("ROI"), _T("H"), 1080, _T(".//SystemConfig.ini"));

	// Video View
	VideoViewCalAngle = ::GetPrivateProfileInt(_T("VideoView"), _T("CalCrossAngle"), 1, _T(".//SystemConfig.ini"));
	VideoViewShowAngle = ::GetPrivateProfileInt(_T("VideoView"), _T("ShowCrossAngle"), 1, _T(".//SystemConfig.ini"));
	VideoMark = ::GetPrivateProfileInt(_T("VideoView"), _T("Mark"), 1, _T(".//SystemConfig.ini"));


	// Snap View
	SnapViewCalAngle = ::GetPrivateProfileInt(_T("SnapView"), _T("CalCrossAngle"), 1, _T(".//SystemConfig.ini"));
	SnapViewShowAngle = ::GetPrivateProfileInt(_T("SnapView"), _T("ShowCrossAngle"), 1, _T(".//SystemConfig.ini"));

	// Line
	CString str = _T("");
	::GetPrivateProfileString(_T("Line"), _T("Thick"), _T("1.5"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	LineThick = (float)_ttof(str);
	LineColorR = ::GetPrivateProfileInt(_T("Line"), _T("R"), 0, _T(".//SystemConfig.ini"));
	LineColorG = ::GetPrivateProfileInt(_T("Line"), _T("G"), 0, _T(".//SystemConfig.ini"));
	LineColorB = ::GetPrivateProfileInt(_T("Line"), _T("B"), 0, _T(".//SystemConfig.ini"));

	// Box
	str = _T("");
	::GetPrivateProfileString(_T("Box"), _T("Thick"), _T("1"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	BoxThick = (float)_ttof(str);
	BoxColorR = ::GetPrivateProfileInt(_T("Box"), _T("R"), 0, _T(".//SystemConfig.ini"));
	BoxColorG = ::GetPrivateProfileInt(_T("Box"), _T("G"), 0, _T(".//SystemConfig.ini"));
	BoxColorB = ::GetPrivateProfileInt(_T("Box"), _T("B"), 0, _T(".//SystemConfig.ini"));
	
	// Mixture
	str = _T("");
	::GetPrivateProfileString(_T("Mixture"), _T("Offset"), _T("0.766"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	MixtureOffset = (float)_ttof(str);
	MixtureX = ::GetPrivateProfileInt(_T("Mixture"), _T("X"), 0, _T(".//SystemConfig.ini"));
	MixtureY = ::GetPrivateProfileInt(_T("Mixture"), _T("Y"), 0, _T(".//SystemConfig.ini"));
	MixtureW = ::GetPrivateProfileInt(_T("Mixture"), _T("W"), 1920, _T(".//SystemConfig.ini"));
	MixtureH = ::GetPrivateProfileInt(_T("Mixture"), _T("H"), 1080, _T(".//SystemConfig.ini"));


	// View
	str = _T("");
	::GetPrivateProfileString(_T("View"), _T("Scale"), _T("0.9"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	ViewScale = (float)_ttof(str);

	::GetPrivateProfileString(_T("ValidRect"), _T("Width"), _T("4"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	ValidRectWidth = (float)_ttof(str);
	ValidRectColorR = ::GetPrivateProfileInt(_T("ValidRect"), _T("R"), 0, _T(".//SystemConfig.ini"));
	ValidRectColorG = ::GetPrivateProfileInt(_T("ValidRect"), _T("G"), 0, _T(".//SystemConfig.ini"));
	ValidRectColorB = ::GetPrivateProfileInt(_T("ValidRect"), _T("B"), 0, _T(".//SystemConfig.ini"));

	::GetPrivateProfileString(_T("CircleCenter"), _T("Width"), _T("1.5"), str.GetBuffer(20), 20, _T(".//SystemConfig.ini"));
	RadiusWidth = (float)_ttof(str);
	CircleCenterColorR = ::GetPrivateProfileInt(_T("CircleCenter"), _T("R"), 0, _T(".//SystemConfig.ini"));
	CircleCenterColorG = ::GetPrivateProfileInt(_T("CircleCenter"), _T("G"), 0, _T(".//SystemConfig.ini"));
	CircleCenterColorB = ::GetPrivateProfileInt(_T("CircleCenter"), _T("B"), 0, _T(".//SystemConfig.ini"));
	CircleCenterSize = ::GetPrivateProfileInt(_T("CircleCenter"), _T("Size"), 12, _T(".//SystemConfig.ini"));

	Debug = ::GetPrivateProfileInt(_T("System"), _T("Debug"), 0, _T(".//SystemConfig.ini"));
}

void CSystemConfig::SaveConfig()
{

}
