#include "stdafx.h"
#include "Mat2CDCConverter.h"


CMat2CDCConverter::CMat2CDCConverter() : 
	m_pCDC(nullptr)
  , m_pGraphics(nullptr)
  , m_width(0)
  , m_height(0)
  , m_pBuffer(nullptr)
  , m_bitmapInfo(nullptr)
  , m_bitmapInfoHeader(nullptr)
  , m_bitmap()
{
}


CMat2CDCConverter::~CMat2CDCConverter()
{
	if (m_pGraphics != nullptr)
	{
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}

	if (m_pBuffer != nullptr)
	{
		delete[] m_pBuffer;
		m_pBuffer = nullptr;
	}
}

bool CMat2CDCConverter::CreateCompatibleConverter(CDC* clientdc, CDC* cdc, int w, int h)
{
	bool ret = false;

	if (m_pGraphics != nullptr)
	{
		m_pGraphics->ReleaseHDC(m_pCDC->GetSafeHdc());
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}

	m_pCDC = cdc;
	m_width = w;
	m_height = h;

	if (m_bitmap.CreateCompatibleBitmap(clientdc, w, h))
	{
		m_pCDC->SelectObject(m_bitmap);

		m_pGraphics = new Gdiplus::Graphics(m_pCDC->GetSafeHdc());

		m_pBuffer = new unsigned char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)* 256];
		m_bitmapInfo = (BITMAPINFO*)m_pBuffer;
		m_bitmapInfoHeader = &(m_bitmapInfo->bmiHeader);

		memset(m_bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));

		m_bitmapInfoHeader->biSize = sizeof(*m_bitmapInfoHeader);
		m_bitmapInfoHeader->biWidth = w;
		m_bitmapInfoHeader->biHeight = -h;
		m_bitmapInfoHeader->biPlanes = 1;
		m_bitmapInfoHeader->biCompression = BI_RGB;
		m_bitmapInfoHeader->biBitCount = 8 * 3;

		if (m_pGraphics != nullptr)
		{
			m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			ret = true;
		}
	}

	return ret;
}

void CMat2CDCConverter::Release()
{
	if (m_pGraphics != nullptr)
	{
		m_pGraphics->ReleaseHDC(m_pCDC->GetSafeHdc());
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}

	m_bitmap.DeleteObject();

	if (m_pBuffer != nullptr)
	{
		delete[] m_pBuffer;
		m_pBuffer = nullptr;
	}

	m_bitmapInfo = nullptr;
	m_bitmapInfoHeader = nullptr;
}

bool CMat2CDCConverter::ConvertMat2CDC(cv::Mat* mat)
{
	bool ret = false;

	if (mat != nullptr)
	{
		::StretchDIBits(m_pCDC->GetSafeHdc(), 0, 0, m_width, m_height, 0, 0, mat->cols, mat->rows, mat->data, (BITMAPINFO*)m_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		ret = true;
	}

	return ret;
}
