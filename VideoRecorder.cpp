#include "stdafx.h"
#include "VideoRecorder.h"


CVideoRecorder::CVideoRecorder() : 
    m_IsSnap(false)
  , m_Read(0)
  , m_Write(1)
  , m_Width(0)
  , m_Height(0)
  , m_LineBytes(0)
  , m_BmpSize(0)
  , m_hdc(NULL)
  , m_hBitmap(NULL)
  , m_lpBitMap(nullptr)
  , m_SnapName("")
  , m_BitmapInfo()
  , m_videoWriter()
{
}


CVideoRecorder::~CVideoRecorder()
{
	StopVideoRecord();

	if (m_lpBitMap != nullptr)
	{
		delete[] m_lpBitMap;
		m_lpBitMap = nullptr;
	}
}

bool CVideoRecorder::InitRecorder(HDC hdc, HBITMAP hBitmap, int w, int h)
{
	if (m_Width != w || m_Height != h)
	{
		m_Width = w;
		m_Height = h;

		for (int i = 0; i < 8; ++i)
		{
			m_Frames[i].release();
			m_Frames[i].create(h, w, CV_8UC3);
		}
	}

	if (m_lpBitMap != nullptr)
	{
		delete[] m_lpBitMap;
		m_lpBitMap = nullptr;
	}

	m_hdc = hdc;
	m_hBitmap = hBitmap;

	m_BitmapInfo.bmiHeader.biSize = sizeof(m_BitmapInfo.bmiHeader);
	m_BitmapInfo.bmiHeader.biWidth = m_Width;
	m_BitmapInfo.bmiHeader.biHeight = m_Height;
	m_BitmapInfo.bmiHeader.biPlanes = 1;
	m_BitmapInfo.bmiHeader.biBitCount = 24;
	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_BitmapInfo.bmiHeader.biSizeImage = 0;
	m_BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	m_BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	m_BitmapInfo.bmiHeader.biClrUsed = 0;
	m_BitmapInfo.bmiHeader.biClrImportant = 0;

	m_LineBytes = ((m_Width * m_BitmapInfo.bmiHeader.biBitCount + 31) / 32) * 4;
	m_BmpSize = m_LineBytes * m_Height;
	m_lpBitMap = new char[m_BmpSize];

	return (m_lpBitMap != nullptr);
}


bool CVideoRecorder::StartVideoRecord(std::string filename, double fps /* = 40 */, VIDEO_FORMAT vf /* = VC_VF_MP4 */)
{
	if (m_videoWriter.isOpened())
	{
		m_videoWriter.release();
	}

	m_videoWriter.open(filename.c_str(), FourCC(vf), fps, cv::Size(m_Width, m_Height), true);

	return m_videoWriter.isOpened();
}

bool CVideoRecorder::StopVideoRecord()
{
	bool ret = false;

	if (m_videoWriter.isOpened())
	{
		m_videoWriter.release();
		ret = true;
	}

	return ret;
}

void CVideoRecorder::UpdateFrame()
{
	if (m_videoWriter.isOpened())
	{
		GetDIBits(m_hdc, m_hBitmap, 0, m_Height, m_lpBitMap, &m_BitmapInfo, DIB_RGB_COLORS);

		for (int i = 0; i < m_Height; ++i)
		{
			int srcIdx = (m_Height - i - 1) * m_LineBytes;
			int desIdx = i * m_Width * 3;

			memcpy(&((m_Frames[m_Write]).data[desIdx]), &m_lpBitMap[srcIdx], m_Width * 3);
		}

		m_Read = (m_Read + 1) % 8;

		if ((m_Read - m_Write != 1) && (m_Write - m_Read != 7))
		{
			m_Write = (m_Write + 1) % 8;
		}
	}
}

void CVideoRecorder::WriteFrame()
{
	if (m_IsSnap)
	{
		int idx = m_Read;
		m_videoWriter.write(m_Frames[idx]);
		cv::imwrite(m_SnapName, m_Frames[idx]);
		m_IsSnap = false;
	}
	else
	{
		m_videoWriter.write(m_Frames[m_Read]);
	}
}

void CVideoRecorder::Snap(std::string snapName)
{
	m_IsSnap = true;
	m_SnapName = snapName + ".png";
}

int CVideoRecorder::FourCC(VIDEO_FORMAT vf)
{
	int code = 0;

	switch (vf)
	{
		case VC_VF_MP4:
		{
			code = m_videoWriter.fourcc('m', 'p', '4', 'v');
			break;
		}
		default:
		{
			code = m_videoWriter.fourcc('m', 'p', '4', 'v');
			break;
		}
	}

	return code;
}
