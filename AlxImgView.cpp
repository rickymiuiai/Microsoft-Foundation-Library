// AlxImgView.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "AlxImgView.h"
#include "AlxDVIView.h"
#include "SystemConfig.h"


// CAlxImgView

IMPLEMENT_DYNAMIC(CAlxImgView, CStatic)

CAlxImgView::CAlxImgView() :
	CAlxView()
  , m_bitmapBuffer()
  , m_dcBuffer()
{
	m_nViewIdx = SNAP_IMG_VIEW;
	m_bHasMixture = TRUE;

	m_bCalCrossAngle = (CSystemConfig::Instance()->SnapViewCalAngle != 0);
	m_bRendingAngle = (CSystemConfig::Instance()->SnapViewShowAngle != 0);

}

CAlxImgView::~CAlxImgView()
{
}

void CAlxImgView::Init(int sw /* = 1920*/, int sh /* = 1080*/)
{
	CAlxView::Init(sw, sh);

	CClientDC dc(this);
	m_dcBuffer.CreateCompatibleDC(&dc);
	m_bitmapBuffer.CreateCompatibleBitmap(&dc, m_rcDcMem.Width(), m_rcDcMem.Height());
	SelectObject(m_dcBuffer, m_bitmapBuffer);
	SetStretchBltMode(m_dcBuffer.GetSafeHdc(), STRETCH_HALFTONE);
}

void CAlxImgView::Uninit()
{
	m_bitmapBuffer.DeleteObject();
	m_dcBuffer.DeleteDC();

	CAlxView::Uninit();
}

CDC* CAlxImgView::RenderMemoryDC()
{
	RECT rect;
	GetClientRect(&rect);

	m_dcMemory.StretchBlt(0, 0, m_pVideoCapture->Width(), m_pVideoCapture->Height(),
		&m_dcBuffer,
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
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
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

void CAlxImgView::Snap()
{
	m_bRender = TRUE;
	m_dcBuffer.StretchBlt(0, 0, m_pVideoCapture->Width(), m_pVideoCapture->Height(),
		m_pVideoCapture->CurrentFrameCDC(),
		0, 0, m_pVideoCapture->Width(), m_pVideoCapture->Height(),
		SRCCOPY);
}


BEGIN_MESSAGE_MAP(CAlxImgView, CStatic)
 	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAlxImgView 消息处理程序
void CAlxImgView::OnPaint()
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

		if (img != nullptr)
		{
			m_dcMemView.StretchBlt(m_rcMixture.left, m_rcMixture.top, m_rcMixture.Width(), m_rcMixture.Height(),
				img,
				m_rcDcMem.left, m_rcDcMem.top, m_rcDcMem.Width(), m_rcDcMem.Height(),
				SRCCOPY);
		}
	}

	dc.BitBlt(0, 0, rect.right, rect.bottom, &m_dcMemView, 0, 0, SRCCOPY);
	
}
