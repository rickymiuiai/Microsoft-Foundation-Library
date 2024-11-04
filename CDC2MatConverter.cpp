#include "stdafx.h"
#include "CDC2MatConverter.h"


CCDC2MatConverter::CCDC2MatConverter() : 
	m_pCDC(nullptr)
  , m_width(0)
  , m_height(0)
  , m_lineBytes(0)
  , m_lpBitMap(nullptr)
{
}


CCDC2MatConverter::~CCDC2MatConverter()
{
	if (m_lpBitMap != nullptr)
	{
		delete[] m_lpBitMap;
		m_lpBitMap = nullptr;
	}
}

bool CCDC2MatConverter::CreateCompatibleConverter(CDC* clientDc, CDC* cdc, int w, int h)
{
	bool ret = false;
	m_pCDC   = cdc;
	m_width  = w;
	m_height = h;

	if (m_bitmap.CreateCompatibleBitmap(clientDc, w, h))
	{
		m_pCDC->SelectObject(m_bitmap);

		CreateBitmapInfo();

		m_lineBytes = ((w * m_bitmapInfo.bmiHeader.biBitCount + 31) / 32) * 4;
		int bmpSize = m_lineBytes * h;

		if (m_lpBitMap != nullptr)
		{
			delete[] m_lpBitMap;
			m_lpBitMap = nullptr;
		}

		m_lpBitMap = new char[bmpSize];

		ret = (m_lpBitMap != nullptr);
	}

	return ret;
}

void CCDC2MatConverter::Release()
{
	m_bitmap.DeleteObject();

	if (m_lpBitMap != nullptr)
	{
		delete[] m_lpBitMap;
		m_lpBitMap = nullptr;
	}
}

bool CCDC2MatConverter::ConvertCDC2Mat(cv::Mat* mat)
{
	bool ret = false;

	if (GetDIBits(m_pCDC->GetSafeHdc(), m_bitmap, 0, m_height, m_lpBitMap, &m_bitmapInfo, DIB_RGB_COLORS) != 0)
	{
		for (int i = 0; i < m_height; ++i)
		{
			int srcIdx = (m_height - i - 1) * m_lineBytes;
			int desIdx = i * m_width * 3;
			memcpy(&(mat->data[desIdx]), &m_lpBitMap[srcIdx], m_width * 3);
		}
	}	

	return ret;
}

void CCDC2MatConverter::CreateBitmapInfo()
{
	m_bitmapInfo.bmiHeader.biSize = sizeof(m_bitmapInfo.bmiHeader);
	m_bitmapInfo.bmiHeader.biWidth = m_width;
	m_bitmapInfo.bmiHeader.biHeight = m_height;
	m_bitmapInfo.bmiHeader.biPlanes = 1;
	m_bitmapInfo.bmiHeader.biBitCount = 24;
	m_bitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_bitmapInfo.bmiHeader.biSizeImage = 0;
	m_bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	m_bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	m_bitmapInfo.bmiHeader.biClrUsed = 0;
	m_bitmapInfo.bmiHeader.biClrImportant = 0;
}
