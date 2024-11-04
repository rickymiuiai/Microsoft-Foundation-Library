#pragma once
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#include "../OpenCV3.2/include/opencv2/opencv.hpp"
#pragma warning(default:4819)
#pragma warning(default:4996)
#include "../GDIPlus/Includes/GdiPlus.h"

class CMat2CDCConverter
{
public:
	CMat2CDCConverter();
	virtual ~CMat2CDCConverter();

public:
	bool CreateCompatibleConverter(CDC* clientdc, CDC* cdc, int w, int h);
	void Release();
	bool ConvertMat2CDC(cv::Mat* mat);
	
private:
	CDC*               m_pCDC;
	Gdiplus::Graphics* m_pGraphics;
	int                m_width;
	int                m_height;
	unsigned char*     m_pBuffer;
	BITMAPINFO*        m_bitmapInfo;
	BITMAPINFOHEADER*  m_bitmapInfoHeader;
	CBitmap            m_bitmap;

};

