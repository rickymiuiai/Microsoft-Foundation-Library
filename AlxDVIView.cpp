// AlxDVIView.cpp : 实现文件
//

#include "stdafx.h"
#include "AlxDVIView.h"
#include "CommonFunction.h"
#include "DataPersistence.h"
#include "SystemConfig.h"

#pragma comment(lib, "../GDIPlus/Lib/GdiPlus.lib")

// CAlxDVIView
IMPLEMENT_DYNAMIC(CAlxDVIView, CStatic)

CAlxDVIView::CAlxDVIView() : 
	CAlxView()
  , m_bGrab(FALSE)
  , m_nDebug(0)
  , m_timer()
  , m_Recorder()
{
	m_nViewIdx = GRAB_DIV_VIEW;

	m_nDebug = CSystemConfig::Instance()->Debug;

	m_bCalCrossAngle = (CSystemConfig::Instance()->VideoViewCalAngle != 0);
	m_bRendingAngle = (CSystemConfig::Instance()->VideoViewShowAngle != 0);

	m_IsMarkVisible = (CSystemConfig::Instance()->VideoMark == 1);
}

CAlxDVIView::~CAlxDVIView()
{
	
}

void CAlxDVIView::Init(int sw /* = 1920*/, int sh /* = 1080*/)
{
	CAlxView::Init(sw, sh);

	m_Recorder.InitRecorder(m_dcMemory.GetSafeHdc(), m_bitmapMemory, m_rcDcMem.Width(), m_rcDcMem.Height());

	m_pVideoCapture->RegisterCallBack(RecvNewFrameCallback);
}

void CAlxDVIView::Uninit()
{
	Stop();

	CAlxView::Uninit();
}

BOOL CAlxDVIView::Start()
{
	if (FALSE == m_bGrab)
	{
		m_bGrab = (m_pVideoCapture->OpenVideoCapture(CSystemConfig::Instance()->UsbIdx)) ? TRUE : FALSE;

		if (1 == m_nDebug)
		{
			if (m_bGrab)
			{
				printf("CAlxDVIView::Start()>>okCaptureTo Success !\n");
			}
			else
			{
				printf("CAlxDVIView::Start()>>okCaptureTo Failed !\n");
			}
		}

		m_nDrag = 0;
		m_nLineHandle = 0;
		m_nLineIdx = -1;
		m_bRender = TRUE;

		std::string strFile = GetDateTime_x1() + ".mp4";
		std::string fullPath = CDataPersistence::getInstance()->GetFullFilePath(strFile);

		if (m_Recorder.StartVideoRecord(fullPath, 25.0))
		{
			m_timer.CreateTimer(this, 40, TimerProc);
			CDataPersistence::getInstance()->UPdateNewFileRecord(strFile, 1);
		}
	}
	
	return m_bGrab;
}

BOOL CAlxDVIView::Stop()
{
	if (m_bGrab)
	{
		m_pVideoCapture->CloseVideoCapture();
		m_bGrab = FALSE;
	}

	m_bRender = FALSE;
	m_nDrag = 0;
	m_nLineHandle = 0;
	m_nLineIdx = -1;
	Invalidate();

	m_timer.StopTimer();
	m_Recorder.StopVideoRecord();

	return !m_bGrab;
}

BOOL CAlxDVIView::isGrabbing()
{
	return m_bGrab;
}

BOOL CAlxDVIView::hasSnap()
{
	return m_bHasMixture;
}

void CAlxDVIView::Snap()
{
	m_bHasMixture = TRUE;

	std::string strName = CDataPersistence::getInstance()->Generate1ImgName();
	std::string fullPath = CDataPersistence::getInstance()->GenerateImgFullName("");

	m_pVideoCapture->SaveImage(fullPath);
	m_Recorder.Snap(fullPath);
}

CDC* CAlxDVIView::RenderMemoryDC()
{
	if (m_bGrab)
	{
		RECT rect;
		GetClientRect(&rect);

		m_dcMemory.StretchBlt(0, 0, m_pVideoCapture->Width(), m_pVideoCapture->Height(),
			m_pVideoCapture->CurrentFrameCDC(),
			0, 0, m_pVideoCapture->Width(), m_pVideoCapture->Height(),
			SRCCOPY);

		if (m_ValidRectSelectPhase != ImgValidRectSelectPhase::Disable)
		{
			int x1 = static_cast<int>(m_ValidRectLeft * m_rcDcMem.Width());
			int y1 = static_cast<int>(m_ValidRectTop * m_rcDcMem.Height());
			int x2 = static_cast<int>(m_ValidRectRight* m_rcDcMem.Width());
			int y2 = static_cast<int>(m_ValidRectBottom * m_rcDcMem.Height());

			m_graphics->DrawRectangle(m_ValidRectPen, x1, y1, (x2 - x1), (y2 - y1));
		}

		if (m_VertexesSelectPhase != TriangleVertexesSelectPhase::Disable)
		{
			for (unsigned int i = 0; i < m_TriangleVertexes.size(); ++i)
			{
				int x = static_cast<int>(m_TriangleVertexes[i].x * m_rcDcMem.Width());
				int y = static_cast<int>(m_TriangleVertexes[i].y * m_rcDcMem.Height());
				int w = 16;
				int h = 16;
				x = x - (w / 2);
				y = y - (h / 2);
				m_graphics->FillEllipse(&m_sb, x, y, w, h);
			}
		}

		if (m_IsCalRadius)
		{
			int xt = 0;
			int yt = 0;
			int wt = CSystemConfig::Instance()->CircleCenterSize;
			int ht = CSystemConfig::Instance()->CircleCenterSize;

			if (0 == m_nRadiusAlgorithm)
			{
#ifdef _RADIUS_ALGORITHM_1_
				xt = (int)m_CenterX1;
				yt = (int)m_CenterY1;
				xt = xt - (wt / 2);
				yt = yt - (ht / 2);
				m_graphics->FillEllipse(&m_sbCircleCenter, xt, yt, wt, ht);
#endif // _RADIUS_ALGORITHM_1_
			}
			else if (1 == m_nRadiusAlgorithm)
			{
#ifdef _RADIUS_ALGORITHM_2_
				xt = (int)m_CenterX2;
				yt = (int)m_CenterY2;
				xt = xt - (wt / 2);
				yt = yt - (ht / 2);
				m_graphics->FillEllipse(&m_sbCircleCenter, xt, yt, wt, ht);
#endif // _RADIUS_ALGORITHM_2_
			}
			else
			{
#ifdef _RADIUS_ALGORITHM_1_
				xt = (int)m_CenterX1;
				yt = (int)m_CenterY1;
				xt = xt - (wt / 2);
				yt = yt - (ht / 2);
				m_graphics->FillEllipse(&m_sb, xt, yt, wt, ht);
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
				xt = (int)m_CenterX2;
				yt = (int)m_CenterY2;
				xt = xt - (wt / 2);
				yt = yt - (ht / 2);
				m_graphics->FillEllipse(&m_sbCircleCenter, xt, yt, wt, ht);
#endif // _RADIUS_ALGORITHM_2_
			}
		}

		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			int x1 = (int)(m_lines[i].start.x * m_rcDcMem.Width());
			int y1 = (int)(m_lines[i].start.y * m_rcDcMem.Height());
			int x2 = (int)(m_lines[i].end.x * m_rcDcMem.Width());
			int y2 = (int)(m_lines[i].end.y * m_rcDcMem.Height());
			m_graphics->DrawLine(m_thickPen, x1, y1, x2, y2);

			if ((1 == m_nLineHandle || 1 == m_nDrag) && m_nLineIdx == i)
			{
				m_graphics->DrawRectangle(m_thinPen, x1 - 5, y1 - 5, 10, 10);
			}

			if ((2 == m_nLineHandle || 2 == m_nDrag) && m_nLineIdx == i)
			{
				m_graphics->DrawRectangle(m_thinPen, x2 - 5, y2 - 5, 10, 10);
			}
		}

		if (m_bCalCrossAngle)
		{
			WCHAR szAngle[16] = { 0 };
			Gdiplus::PointF origin;
			std::list<CIntersection>::iterator itor;
			for (itor = m_intersections.begin(); itor != m_intersections.end(); ++itor)
			{
				if (m_bRendingAngle || itor->m_bHitIntersection)
				{
					origin.X = itor->m_txtPoint1.x * m_rcDcMem.Width();
					origin.Y = itor->m_txtPoint1.y * m_rcDcMem.Height();
					memset(szAngle, 0, sizeof(WCHAR)* 16);
					swprintf_s(szAngle, 16, L"%.2f°", itor->m_fAngle1);
					m_graphics->DrawString(szAngle, (INT)wcslen(szAngle), &m_Font, origin, &m_strFormat, &m_sb);


					memset(szAngle, 0, sizeof(WCHAR)* 16);
					swprintf_s(szAngle, 16, L"%.2f°", itor->m_fAngle2);
					origin.X = itor->m_txtPoint2.x * m_rcDcMem.Width();
					origin.Y = itor->m_txtPoint2.y * m_rcDcMem.Height();
					m_graphics->DrawString(szAngle, (INT)wcslen(szAngle), &m_Font, origin, &m_strFormat, &m_sb);
				}
			}
		}

		return &m_dcMemory;
	}
	else
	{
		return nullptr;
	}
}

void CAlxDVIView::TimerProc(void* pUser)
{
	CAlxDVIView* pThis = static_cast<CAlxDVIView*>(pUser);

	if (pThis != nullptr)
	{
		pThis->m_Recorder.WriteFrame();
	}
}

void CAlxDVIView::RecvNewFrameCallback()
{
	AfxGetApp()->GetMainWnd()->InvalidateRect(NULL, FALSE);
	AfxGetApp()->GetMainWnd()->UpdateWindow();
}

BEGIN_MESSAGE_MAP(CAlxDVIView, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CAlxDVIView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	RECT rect;
	GetClientRect(&rect);

	m_dcMemView.FillSolidRect(&rect, RGB(0, 0, 0));

	if (m_bRender)
	{
		RenderMemoryDC();

		m_dcMemView.StretchBlt(m_rcTarget.left, m_rcTarget.top, m_rcTarget.Width(), m_rcTarget.Height(),
			&m_dcMemory,
			m_rcDcMem.left, m_rcDcMem.top, m_rcDcMem.Width(), m_rcDcMem.Height(),
			SRCCOPY);
	}

	if (m_bMixtureRending)
	{
		CDC* img = m_pView->RenderMemoryDC();

		m_dcMemView.StretchBlt(m_rcMixture.left, m_rcMixture.top, m_rcMixture.Width(), m_rcMixture.Height(),
			img,
			m_rcDcMem.left, m_rcDcMem.top, m_rcDcMem.Width(), m_rcDcMem.Height(),
			SRCCOPY);
	}

	if (m_IsMarkVisible)
	{
		CRect rc(0, 0, 24, 24);
		m_dcMemView.DrawText(_T("●"), &rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
	}

	dc.BitBlt(0, 0, rect.right, rect.bottom, &m_dcMemView, 0, 0, SRCCOPY);

	m_Recorder.UpdateFrame();
}


