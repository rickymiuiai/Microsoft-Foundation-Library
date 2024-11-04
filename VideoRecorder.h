#pragma once
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#include "../OpenCV3.2/include/opencv2/opencv.hpp"
#pragma warning(default:4819)
#pragma warning(default:4996)

#include "VideoRecordFormat.h"

class CVideoRecorder
{
public:
	CVideoRecorder();
	virtual ~CVideoRecorder();

public:
	bool InitRecorder(HDC hdc, HBITMAP hBitmap, int w, int h);
	bool StartVideoRecord(std::string filename, double fps = 40, VIDEO_FORMAT vf = VC_VF_MP4);
	bool StopVideoRecord();
	void UpdateFrame();
	void Snap(std::string snapName);
	void WriteFrame();

private:
	int FourCC(VIDEO_FORMAT vf);

private:
	bool            m_IsSnap;
	int             m_Read;
	int             m_Write;
	int             m_Width;
	int             m_Height;
	int             m_LineBytes;
	int             m_BmpSize;
	HDC             m_hdc;
	HBITMAP         m_hBitmap;
	char*           m_lpBitMap;
	std::string     m_SnapName;
	cv::VideoWriter m_videoWriter;
	BITMAPINFO      m_BitmapInfo;
	cv::Mat         m_Frames[8];
};

