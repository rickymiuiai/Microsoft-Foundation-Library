#include "stdafx.h"
#include "VideoCaptureBase.h"


CVideoCaptureBase::CVideoCaptureBase() : 
	m_IsRunning(false)
  , m_IsSnap(false)
  , m_pCDC(NULL)
  , m_pfRecvNewFrame(nullptr)
  , m_SnapName("")
{
}


CVideoCaptureBase::~CVideoCaptureBase()
{
	if (m_pCDC != NULL)
	{
		delete m_pCDC;
		m_pCDC = NULL;
	}
}

