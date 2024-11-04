#pragma once

#include "OkiVideoCapture.h"
#include "UsbVideoCapture.h"

enum VideoCaptureType
{
	Oki_Dvi,
	Usb
};

class CVideoCaptureFactory
{
public:
	CVideoCaptureFactory();
	virtual ~CVideoCaptureFactory();

public:
	IVideoCapture* CreateVideoCapture(VideoCaptureType type);
	IVideoCapture* VideoCapture() const { return m_pVideoCapture; }
	
private:
	IVideoCapture*    m_pVideoCapture;
	COkiVideoCapture* m_pOkiVideoCapture;
	CUsbVideoCapture* m_pUsbVideoCapture;
};

