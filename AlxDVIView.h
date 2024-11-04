#pragma once
#include "AlxView.h"
#include "AlxHrTimer.h"
#include "VideoRecorder.h"

// CAlxDVIView
class CAlxDVIView : public CAlxView
{
	DECLARE_DYNAMIC(CAlxDVIView)
public:
	CAlxDVIView();
	virtual ~CAlxDVIView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	void Init(int sw = 1920, int sh = 1080) override;
	void Uninit() override;
	CDC* RenderMemoryDC() override;
	BOOL Start();
	BOOL Stop();
	BOOL isGrabbing();
	BOOL hasSnap();
	void Snap() override;

public:
	static void TimerProc(void* pUser);
	static void RecvNewFrameCallback();

private:
	bool                      m_IsMarkVisible;
	BOOL                      m_bGrab;
	int                       m_nDebug;
	CAlxHrTimerEx             m_timer;
	CVideoRecorder            m_Recorder;
};


