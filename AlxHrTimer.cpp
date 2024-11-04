#include "stdafx.h"
#include "AlxHrTimer.h"



CAlxHrTimerEx::CAlxHrTimerEx() : 
	m_hTimerQueue(NULL)
  , m_hTimer(NULL)
  , m_pUser(nullptr)
  , m_pCb(nullptr)
{

}

CAlxHrTimerEx::~CAlxHrTimerEx()
{
	ClearUp();
}

int CAlxHrTimerEx::CreateTimer(void* pUser, DWORD dwDelay, TIMERCALLBACK pcb)
{
	int ret = 0;

	if (pUser != nullptr && dwDelay > 0 && pcb != nullptr)
	{
		if (NULL == m_hTimerQueue)
		{
			m_hTimerQueue = CreateTimerQueue();
		}

		if (m_hTimerQueue != NULL)
		{
			if (CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, TimerProc, this, 0, dwDelay, WT_EXECUTEDEFAULT))
			{
				m_pUser = pUser;
				m_pCb = pcb;
				ret = 1;
			}
		}
	}
	

	return ret;
}

void CAlxHrTimerEx::StopTimer()
{
	if (m_hTimer != NULL)
	{
		if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, INVALID_HANDLE_VALUE))
		{
			m_hTimer = NULL;
		}
	}
}

void CAlxHrTimerEx::TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	CAlxHrTimerEx* pThis = static_cast<CAlxHrTimerEx*>(lpParameter);

	if (pThis != nullptr && pThis->m_pCb != nullptr)
	{
		pThis->m_pCb(pThis->m_pUser);
	}
}

// helper
void CAlxHrTimerEx::ClearUp()
{
	StopTimer();

	if (m_hTimerQueue != NULL)
	{
		if (DeleteTimerQueueEx(m_hTimerQueue, NULL))
		{
			m_hTimerQueue = NULL;
		}
	}
}