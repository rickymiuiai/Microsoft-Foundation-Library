#pragma once
#include "IVideoCapture.h"

class CVideoCaptureBase : public IVideoCapture
{
public:
	CVideoCaptureBase();
	virtual ~CVideoCaptureBase();

public:
	virtual void RegisterCallBack(pfRecvNewFrame callback) override { m_pfRecvNewFrame = callback; }
	virtual CDC* CurrentFrameCDC() const override { return m_pCDC; }

protected:
	bool           m_IsRunning;
	bool           m_IsSnap;
	CDC*           m_pCDC;
	pfRecvNewFrame m_pfRecvNewFrame;
	std::string    m_SnapName;
};

