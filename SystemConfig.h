#pragma once
class CSystemConfig
{
private:
	CSystemConfig();
	CSystemConfig(CSystemConfig& other) = delete;
	CSystemConfig& operator= (CSystemConfig& other) = delete;

public:
	virtual ~CSystemConfig();

public:
	static CSystemConfig* Instance() { return _instance; }

public:
	void LoadConfig();
	void SaveConfig();


private:
	static CSystemConfig* _instance;

public:
	int Type;
	int UsbIdx;
	int VideoW;
	int VideoH;
	int GrabX;
	int GrabY;
	int GrabW;
	int GrabH;
	int VideoViewCalAngle;
	int VideoViewShowAngle;
	int SnapViewCalAngle;
	int SnapViewShowAngle;
	float LineThick;
	int LineColorR;
	int LineColorG;
	int LineColorB;
	float BoxThick;
	int BoxColorR;
	int BoxColorG;
	int BoxColorB;
	float MixtureOffset;
	float ViewScale;
	int Debug;
	int VideoMark;
	int MixtureX;
	int MixtureY;
	int MixtureW;
	int MixtureH;
	float ValidRectWidth;
	int ValidRectColorR;
	int ValidRectColorG;
	int ValidRectColorB;
	float RadiusWidth;
	int CircleCenterColorR;
	int CircleCenterColorG;
	int CircleCenterColorB;
	int CircleCenterSize;
};

