#pragma once


class CAlxHrTimerEx
{
	typedef void(*TIMERCALLBACK)(void*);
public:
	CAlxHrTimerEx();
	virtual ~CAlxHrTimerEx();

public:
	int CreateTimer(void* pUser, DWORD dwDelay, TIMERCALLBACK pcb);
	void StopTimer();

public:
	static void CALLBACK TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

private:
	void ClearUp();

private:
	HANDLE        m_hTimerQueue;
	HANDLE        m_hTimer;
	void*         m_pUser;
	TIMERCALLBACK m_pCb;
};

