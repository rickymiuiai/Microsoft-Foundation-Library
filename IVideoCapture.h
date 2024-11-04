#pragma once

#include <afxwin.h>
#include <string>

typedef void (*pfRecvNewFrame)();

class IVideoCapture
{
public:
	virtual bool Init(CDC* pCDC, int x, int y, int w, int h) = 0;
	virtual bool UnInit() = 0;
	virtual void RegisterCallBack(pfRecvNewFrame callback) = 0;
	virtual bool OpenVideoCapture(int id) = 0;
	virtual bool CloseVideoCapture() = 0;
	virtual CDC* CurrentFrameCDC() const = 0;
	virtual bool SaveImage(std::string imgname) = 0;
	virtual int  Width() const = 0;
	virtual int  Height() const = 0;
};