#pragma once
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#include "../OpenCV3.2/include/opencv2/opencv.hpp"
#pragma warning(default:4819)
#pragma warning(default:4996)

class CCDC2MatConverter
{
public:
	CCDC2MatConverter();
	virtual ~CCDC2MatConverter();

public:
	bool CreateCompatibleConverter(CDC* clientDc, CDC* cdc, int w, int h);
	void Release();
	bool ConvertCDC2Mat(cv::Mat* mat);

private:
	void CreateBitmapInfo();

private:
	CDC*       m_pCDC;
	int        m_width;
	int        m_height;
	int        m_lineBytes;
	char*      m_lpBitMap;
	CBitmap    m_bitmap;
	BITMAPINFO m_bitmapInfo;
	
};

