#include "stdafx.h"
#include "VideoCaptureFactory.h"


CVideoCaptureFactory::CVideoCaptureFactory() : 
	m_pVideoCapture(nullptr)
  , m_pOkiVideoCapture(nullptr)
  , m_pUsbVideoCapture(nullptr)
{
}


CVideoCaptureFactory::~CVideoCaptureFactory()
{
	m_pVideoCapture = nullptr;

	if (m_pOkiVideoCapture != nullptr)
	{
		delete m_pOkiVideoCapture;
		m_pOkiVideoCapture = nullptr;
	}

	if (m_pUsbVideoCapture != nullptr)
	{
		delete m_pUsbVideoCapture;
		m_pUsbVideoCapture = nullptr;
	}
}

IVideoCapture* CVideoCaptureFactory::CreateVideoCapture(VideoCaptureType type)
{
	switch (type)
	{
		case Oki_Dvi:
		{
			if (m_pUsbVideoCapture != nullptr)
			{
				delete m_pUsbVideoCapture;
				m_pUsbVideoCapture = nullptr;
			}

			if (m_pOkiVideoCapture == nullptr)
			{
				m_pOkiVideoCapture = new COkiVideoCapture();
			}

			m_pVideoCapture = m_pOkiVideoCapture;
			break;
		}
		case Usb:
		{
			if (m_pOkiVideoCapture != nullptr)
			{
				delete m_pOkiVideoCapture;
				m_pOkiVideoCapture = nullptr;
			}

			if (m_pUsbVideoCapture == nullptr)
			{
				m_pUsbVideoCapture = new CUsbVideoCapture();
			}

			m_pVideoCapture = m_pUsbVideoCapture;
			break;
		}
	}

	return m_pVideoCapture;
}
