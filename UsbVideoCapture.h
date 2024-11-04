#pragma once
#include "VideoCaptureBase.h"
#include "Mat2CDCConverter.h"

class CUsbVideoCapture : public CVideoCaptureBase
{
public:
	CUsbVideoCapture();
	virtual ~CUsbVideoCapture();

public:
	bool Init(CDC* pCDC, int x, int y, int w, int h) override;
	bool UnInit() override;
	bool OpenVideoCapture(int id) override;
	bool CloseVideoCapture() override;
	bool SaveImage(std::string imgname) override;
	int  Width() const override { return m_Rect.width; }
	int  Height() const override { return m_Rect.height; }

private:
	static unsigned int __stdcall WorkThread(void* pParam);

private:
	void VideoCapture();

private:
	HANDLE            m_hThread;
	cv::Rect          m_Rect;
	cv::Mat           m_FrameMat;
	cv::Mat           m_Mat;
	cv::VideoCapture  m_VideoCapture;
	CMat2CDCConverter m_Converter;
};

