#include "stdafx.h"
#include "UsbVideoCapture.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_core320d.lib")
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_videoio320d.lib")
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_imgcodecs320d.lib")
#else
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_core320.lib")
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_videoio320.lib")
#pragma comment(lib, "../OpenCV3.2/x64/lib/opencv_imgcodecs320.lib")
#endif // _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_core320_d.lib")
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_videoio320_d.lib")
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_imgcodecs320_d.lib")
#else
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_core320.lib")
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_videoio320.lib")
#pragma comment(lib, "../OpenCV3.2/x86/lib/opencv_imgcodecs320.lib")
#endif // _DEBUG
#endif // _WIN64

CUsbVideoCapture::CUsbVideoCapture() : 
	CVideoCaptureBase()
  , m_hThread(INVALID_HANDLE_VALUE)
  , m_Rect(0, 0, 1920, 1080)
  , m_FrameMat()
  , m_Mat()
  , m_VideoCapture()
  , m_Converter()
{
}


CUsbVideoCapture::~CUsbVideoCapture()
{
	UnInit();
}

bool CUsbVideoCapture::Init(CDC* pCDC, int x, int y, int w, int h)
{
	UnInit();

	m_Mat.create(h, w, CV_8UC3);

	m_Rect.x = x;
	m_Rect.y = y;
	m_Rect.width = w;
	m_Rect.height = h;

	m_pCDC = new CDC();
	m_pCDC->CreateCompatibleDC(pCDC);

	m_Converter.CreateCompatibleConverter(pCDC, m_pCDC, w, h);

	m_pCDC->SetStretchBltMode(STRETCH_HALFTONE);

	return true;
}

bool CUsbVideoCapture::UnInit()
{
	CloseVideoCapture();

	m_Converter.Release();

	if (m_pCDC != NULL)
	{
		m_pCDC->DeleteDC();

		delete m_pCDC;
		m_pCDC = nullptr;
	}

	return true;
}

bool CUsbVideoCapture::OpenVideoCapture(int id)
{
	m_IsSnap = false;

	if (false == m_IsRunning)
	{
		m_VideoCapture.open(id);

		if (m_VideoCapture.isOpened())
		{
			m_VideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
			m_VideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

			m_IsRunning = true;
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, WorkThread, this, 0, NULL);

			if (m_hThread == INVALID_HANDLE_VALUE)
			{
				m_IsRunning = false;
				m_VideoCapture.release();
			}
		}
	}

	return m_IsRunning;
}

bool CUsbVideoCapture::CloseVideoCapture()
{
	if (m_IsRunning)
	{
		m_IsRunning = false;

		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 1000))
		{
			TerminateThread(m_hThread, 0);
		}

		m_hThread = INVALID_HANDLE_VALUE;

		m_VideoCapture.release();
	}
	return true;
}

bool CUsbVideoCapture::SaveImage(std::string imgname)
{
	m_IsSnap = true;
	m_SnapName = imgname + "_o.png";
	return true;
}

void CUsbVideoCapture::VideoCapture()
{
	while (m_IsRunning)
	{
		m_VideoCapture >> (m_FrameMat);

		m_FrameMat(m_Rect).copyTo(m_Mat);

		if (m_IsSnap)
		{
			cv::imwrite(m_SnapName.c_str(), m_Mat);
			m_IsSnap = false;
		}

		m_Converter.ConvertMat2CDC(&m_Mat);

		if (m_pfRecvNewFrame != nullptr) m_pfRecvNewFrame();
	}
	
}


unsigned int CUsbVideoCapture::WorkThread(void* pParam)
{
	CUsbVideoCapture* pThis = static_cast<CUsbVideoCapture*>(pParam);

	if (pThis != nullptr)
	{
		pThis->VideoCapture();
	}

	return 0;
}
