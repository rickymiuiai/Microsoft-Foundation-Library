#include "stdafx.h"
#include "AlxView.h"

#include "SystemConfig.h"

#define R2A (180.0f / 3.1415926f)
#define A2R (3.1415926f / 180.0f)



// CLine
CLine::CLine()
{
	memset(&start, 0, sizeof(POINTF));
	memset(&end, 0, sizeof(POINTF));
}

CLine::CLine(POINTF s, POINTF e)
{
	start = s;
	end = e;
}

CLine::~CLine()
{

}

float CLine::LengthSquare()
{
	float fx = end.x - start.x;
	float fy = end.y - start.y;
	float fd = fx * fx + fy * fy;

	return fd;
}

float CLine::Length()
{
	float l2 = LengthSquare();

	return sqrt(l2);
}

// CIntersection
CIntersection::CIntersection() : 
    m_bHitIntersection(false)
  , m_nIdxLine1(-1)
  , m_nIdxLine2(-1)
  , m_fAngle1(0.0f)
  , m_fAngle2(0.0f)
  , m_intersection()
  , m_txtPoint1()
  , m_txtPoint2()
{

}

CIntersection::CIntersection(int idx1, int idx2, float angle, POINTF isp) : 
	m_bHitIntersection(false)
  , m_nIdxLine1(idx1)
  , m_nIdxLine2(idx2)
  , m_fAngle1(angle)
  , m_fAngle2(180 - angle)
  , m_intersection(isp)
  , m_txtPoint1()
  , m_txtPoint2()
{

}

CIntersection::CIntersection(int idx1, int idx2, float angle, POINTF isp, POINTF p1, POINTF p2) :
	m_bHitIntersection(false)
  , m_nIdxLine1(idx1)
  , m_nIdxLine2(idx2)
  , m_fAngle1(angle)
  , m_fAngle2(180 - angle)
  , m_intersection(isp)
  , m_txtPoint1(p1)
  , m_txtPoint2(p2)
{

}

CIntersection::~CIntersection()
{

}

bool CIntersection::ContainLine(int idx)
{
	return ((m_nIdxLine1 == idx) || (m_nIdxLine2 == idx));
}

// CAlxView

IMPLEMENT_DYNAMIC(CAlxView, CStatic)


CAlxView::CAlxView() : 
	m_IsValidRectSelected(false)
  , m_IsCalRadius(false)
  , m_bRender(FALSE)
  , m_bLineDrawing(FALSE)
  , m_bNormalSize(TRUE)
  , m_bZoom(FALSE)
  , m_bMixtureRending(FALSE)
  , m_bHasMixture(FALSE)
  , m_bCalCrossAngle(FALSE)
  , m_bRendingAngle(TRUE)
  , m_nDrag(0)
  , m_nLineIdx(-1)
  , m_nLineHandle(0)
  , m_nViewIdx(GRAB_DIV_VIEW)
  , m_nRadiusAlgorithm(0)
  , m_ValidRectSelectPhase(ImgValidRectSelectPhase::Disable)
  , m_VertexesSelectPhase(TriangleVertexesSelectPhase::Disable)
  , m_ValidRectLeft(0)
  , m_ValidRectTop(0)
  , m_ValidRectRight(0)
  , m_ValidRectBottom(0)
  , m_ImgRectScale(1)
#ifdef _RADIUS_ALGORITHM_1_
  , m_AirwayRadius1(0)
  , m_CenterX1(0)
  , m_CenterY1(0)
#endif
#ifdef _RADIUS_ALGORITHM_2_
  , m_AirwayRadius2(0)
  , m_CenterX2(0)
  , m_CenterY2(0)
#endif
  , m_pVideoCapture(nullptr)
  , m_pView(nullptr)
  , m_graphics(nullptr)
  , m_thickPen(nullptr)
  , m_thinPen(nullptr)
  , m_ValidRectPen(nullptr)
  , m_Font(L"Arial", 14)
  , m_sb(Gdiplus::Color(255, 0, 255, 0))
  , m_sbCircleCenter(Gdiplus::Color(255, 255, 0, 0))
  , m_rcTarget(0, 0, 1920, 1080)
  , m_rcDcMem(0, 0, 1920, 1080)
  , m_rcMixture(0, 0, 1920, 1080)
  , m_bitmapMemory()
  , m_bitmapMemView()
  , m_dcMemory()
  , m_dcMemView()
  , m_lines()
  , m_intersections()
  , m_zoomPoint(0, 0)
{
	m_rcDcMem.left = 0;
	m_rcDcMem.top = 0;
	m_rcDcMem.right = CSystemConfig::Instance()->GrabW;
	m_rcDcMem.bottom = CSystemConfig::Instance()->GrabH;

	m_IsValidRectSelected = false;
	m_ValidRectSelectPhase = ImgValidRectSelectPhase::Disable;
	m_VertexesSelectPhase = TriangleVertexesSelectPhase::Disable;

	m_CenterX1 = m_CenterX2 = m_CenterY1 = m_CenterY2 = 0.0f;
}


CAlxView::~CAlxView()
{
	Uninit();
}

void CAlxView::FullScreen(BOOL fs, BOOL mix)
{
	m_bNormalSize = !fs;
	m_bMixtureRending = mix;
}

void CAlxView::Init(int sw /* = 1920*/, int sh /* = 1080*/)
{
	m_thickPen = new Gdiplus::Pen(Gdiplus::Color(
		CSystemConfig::Instance()->LineColorR, 
		CSystemConfig::Instance()->LineColorG, 
		CSystemConfig::Instance()->LineColorB), 
		CSystemConfig::Instance()->LineThick);

	m_sb.SetColor(Gdiplus::Color(255, 
		CSystemConfig::Instance()->LineColorR, 
		CSystemConfig::Instance()->LineColorG, 
		CSystemConfig::Instance()->LineColorB));

	m_thinPen = new Gdiplus::Pen(Gdiplus::Color(
		CSystemConfig::Instance()->BoxColorR, 
		CSystemConfig::Instance()->BoxColorG, 
		CSystemConfig::Instance()->BoxColorB), 
		CSystemConfig::Instance()->BoxThick);

	m_ValidRectPen = new Gdiplus::Pen(Gdiplus::Color(
		CSystemConfig::Instance()->ValidRectColorR,
		CSystemConfig::Instance()->ValidRectColorG,
		CSystemConfig::Instance()->ValidRectColorB),
		CSystemConfig::Instance()->ValidRectWidth);

	m_sbCircleCenter.SetColor(Gdiplus::Color(255,
		CSystemConfig::Instance()->CircleCenterColorR,
		CSystemConfig::Instance()->CircleCenterColorG,
		CSystemConfig::Instance()->CircleCenterColorB));

	// Create Memory DC
	CClientDC dc(this);
	m_dcMemory.CreateCompatibleDC(&dc);
	m_bitmapMemory.CreateCompatibleBitmap(&dc, m_rcDcMem.Width(), m_rcDcMem.Height());
	SelectObject(m_dcMemory, m_bitmapMemory);
	SetStretchBltMode(m_dcMemory.GetSafeHdc(), STRETCH_HALFTONE);

	m_dcMemView.CreateCompatibleDC(&dc);
	m_bitmapMemView.CreateCompatibleBitmap(&dc, sw, sh);
	SelectObject(m_dcMemView, m_bitmapMemView);
	SetStretchBltMode(m_dcMemView.GetSafeHdc(), STRETCH_HALFTONE);

	m_dcMemView.SetTextColor(RGB(255, 0, 0));
	m_dcMemView.SetBkMode(TRANSPARENT);

	m_graphics = new Gdiplus::Graphics(m_dcMemory.GetSafeHdc());
	m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	m_strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
}

void CAlxView::Uninit()
{
	m_bitmapMemory.DeleteObject();
	m_dcMemory.DeleteDC();

	m_bitmapMemView.DeleteObject();
	m_dcMemView.DeleteDC();

	if (m_thickPen != nullptr)
	{
		delete m_thickPen;
		m_thickPen = nullptr;
	}

	if (m_thinPen != nullptr)
	{
		delete m_thinPen;
		m_thinPen = nullptr;
	}

	if (m_ValidRectPen != nullptr)
	{
		delete m_ValidRectPen;
		m_ValidRectPen = nullptr;
	}

	if (m_graphics != nullptr)
	{
		delete m_graphics;
		m_graphics = nullptr;
	}

	m_pVideoCapture = nullptr;
}

void CAlxView::Clear()
{
	m_lines.clear();
	m_intersections.clear();
	m_nDrag = 0;
	m_nLineIdx = -1;
	m_nLineHandle = 0;
	m_bLineDrawing = FALSE;

	Invalidate();
}

void CAlxView::Rending()
{
	m_bRender = TRUE;
	Invalidate();
}

void CAlxView::ForceReleaseRMbtn()
{
	m_bZoom = FALSE;
}

bool CAlxView::IsValidRectSelected()
{
	return m_IsValidRectSelected;
}

void CAlxView::ShowValidRect()
{
	if (m_IsValidRectSelected)
	{
		m_ValidRectSelectPhase = ImgValidRectSelectPhase::Finish;
	}
}

void CAlxView::SkipValidRectSelect()
{
	m_ValidRectSelectPhase = ImgValidRectSelectPhase::Finish;

	m_IsCalRadius = false;
}

void CAlxView::SetImgRectScale(float scale)
{
	float h = (m_ValidRectBottom - m_ValidRectTop) * m_rcDcMem.Height();
	m_ImgRectScale = scale / h;
}

void CAlxView::StartSelectValidRect()
{
	m_ValidRectSelectPhase = ImgValidRectSelectPhase::TLSelecting;

	m_IsCalRadius = false;
}

void CAlxView::CloseSelectValidRect()
{
	m_ValidRectSelectPhase = ImgValidRectSelectPhase::Disable;
}

void CAlxView::StartSelectTriangleVertexes()
{
	if (m_IsValidRectSelected)
	{
		m_TriangleVertexes.clear();
		m_VertexesSelectPhase = TriangleVertexesSelectPhase::Selecting1;
	}

	m_IsCalRadius = false;
}

void CAlxView::CloseSelectTriangleVertexes()
{
	m_VertexesSelectPhase = TriangleVertexesSelectPhase::Disable;
}

void CAlxView::CalculateAirwayRadius()
{
#ifdef _RADIUS_ALGORITHM_1_
	CalculateRadius1();
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
	CalculateRadius2();
#endif // _RADIUS_ALGORITHM_2_

#ifndef _DISCARD_CODE_
	CalculateRadius3();
	CalculateRadius4();
#endif // !_DISCARD_CODE_
	
	m_IsCalRadius = true;

	m_ValidRectSelectPhase = ImgValidRectSelectPhase::Disable;
	m_VertexesSelectPhase = TriangleVertexesSelectPhase::Disable;
}

void CAlxView::CalculateAirwayRadius(int id)
{
	m_nRadiusAlgorithm = id;
	if (id == 0)
	{
#ifdef _RADIUS_ALGORITHM_1_
		CalculateRadius1();
#endif // _RADIUS_ALGORITHM_1_
	}
	else if (1 == id)
	{
#ifdef _RADIUS_ALGORITHM_2_
		CalculateRadius2();
#endif // _RADIUS_ALGORITHM_2_
	}
	else
	{
#ifdef _RADIUS_ALGORITHM_1_
		CalculateRadius1();
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
		CalculateRadius2();
#endif // _RADIUS_ALGORITHM_2_
	}

	m_IsCalRadius = true;

	m_ValidRectSelectPhase = ImgValidRectSelectPhase::Disable;
	m_VertexesSelectPhase = TriangleVertexesSelectPhase::Disable;
}

float CAlxView::AirwayRadius()
{
	float radius = 0;

	if (m_nRadiusAlgorithm == 0)
	{
#ifdef _RADIUS_ALGORITHM_1_
		radius = m_AirwayRadius1 * m_ImgRectScale;
#endif // _RADIUS_ALGORITHM_1_
	}
	else if (1 == m_nRadiusAlgorithm)
	{
#ifdef _RADIUS_ALGORITHM_2_
		radius = m_AirwayRadius2 * m_ImgRectScale;
#endif // _RADIUS_ALGORITHM_2_
	}
	else
	{
#ifdef _RADIUS_ALGORITHM_1_
		radius = m_AirwayRadius1 * m_ImgRectScale;
		printf("1 %f %f %f\n", m_AirwayRadius1, m_ImgRectScale, radius);
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
		radius = m_AirwayRadius2 * m_ImgRectScale;
		printf("2 %f %f %f\n", m_AirwayRadius2, m_ImgRectScale, radius);
#endif // _RADIUS_ALGORITHM_2_
	}

	return radius;
}

// helper
int CAlxView::PickLine(CPoint point, int idx, int r /* = 5 */)
{
	int ret = 0;

	point.x -= m_rcTarget.left;
	point.y -= m_rcTarget.top;

	if ((abs(point.x - m_lines[idx].start.x * m_rcTarget.Width()) < r) && (abs(point.y - m_lines[idx].start.y * m_rcTarget.Height()) < r))
	{
		ret = 1;
	}
	else if ((abs(point.x - m_lines[idx].end.x * m_rcTarget.Width()) < r) && (abs(point.y - m_lines[idx].end.y * m_rcTarget.Height()) < r))
	{
		ret = 2;
	}
	else
	{
		ret = 0;
	}


	return ret;
}

CRect CAlxView::CalculateTargetRect()
{
	CRect rc;
	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_rcDcMem.Width() <= rcClient.Width() && m_rcDcMem.Height() <= rcClient.Height())
	{
		rc.left = 0;
		rc.top = (rcClient.Height() - m_rcDcMem.Height()) / 2;
		rc.right = rc.left + m_rcDcMem.Width();
		rc.bottom = rc.top + m_rcDcMem.Height();
	}
	else
	{
		int w = rcClient.Width();
		int h = (int)(((float)m_rcDcMem.Height() / (float)m_rcDcMem.Width()) * w);

		if (h > rcClient.Height())
		{
			h = rcClient.Height();
			w = (int)(((float)m_rcDcMem.Width() / (float)m_rcDcMem.Height()) * h);
		}

		rc.left = 0;
		rc.top = (rcClient.Height() - h) / 2;
		rc.right = rc.left + w;
		rc.bottom = rc.top + h;
	}

	if (m_bMixtureRending)
	{
		m_rcMixture.top = rc.top;
		//m_rcMixture.left = (int)(rc.Width() * CSystemConfig::Instance()->MixtureOffset);
		//m_rcMixture.left = (int)(rcClient.Width() * CSystemConfig::Instance()->MixtureOffset);
		if (rcClient.Width() - rc.Width() <= 200)
		{
			m_rcMixture.left = (int)(rcClient.Width() * CSystemConfig::Instance()->MixtureOffset);
		}
		else
		{
			m_rcMixture.left = (int)(rc.Width());
		}
		
		m_rcMixture.right = rcClient.right;
		m_rcMixture.bottom = rc.top + (int)((float)m_rcDcMem.Height() / (float)m_rcDcMem.Width() * (float)m_rcMixture.Width());
	}

// 	printf("[%d, %d, %d, %d][%d, %d, %d, %d]\n",
// 		rcClient.left, rcClient.top, rcClient.right, rcClient.bottom,
// 		rc.left, rc.top, rc.right, rc.bottom);

	return rc;
}

bool CAlxView::IsCross2Lines(CLine& line1, CLine& line2)
{
	//快速排斥实验
	if (((line1.end.x > line1.start.x ? line1.end.x : line1.start.x) < (line2.start.x < line2.end.x ? line2.start.x : line2.end.x)) ||
		((line1.end.y > line1.start.y ? line1.end.y : line1.start.y) < (line2.start.y < line2.end.y ? line2.start.y : line2.end.y)) ||
		((line2.end.x > line2.start.x ? line2.end.x : line2.start.x) < (line1.start.x < line1.end.x ? line1.start.x : line1.end.x)) ||
		((line2.end.y > line2.start.y ? line2.end.y : line2.start.y) < (line1.start.y < line1.end.y ? line1.start.y : line1.end.y)))
	{
		return false;
	}

	//跨立实验
	if (
		((
		((line1.start.x - line2.start.x) * (line2.end.y - line2.start.y) - (line1.start.y - line2.start.y) * (line2.end.x - line2.start.x)) *
		((line1.end.x - line2.start.x) * (line2.end.y - line2.start.y) - (line1.end.y - line2.start.y) * (line2.end.x - line2.start.x))
		) > 0)
		||
		((
		((line2.start.x - line1.start.x) * (line1.end.y - line1.start.y) - (line2.start.y - line1.start.y) * (line1.end.x - line1.start.x)) *
		((line2.end.x - line1.start.x) * (line1.end.y - line1.start.y) - (line2.end.y - line1.start.y) * (line1.end.x - line1.start.x))
		) > 0)
	   )
	{
		return false;
	}

	return true;
}

POINTF CAlxView::CalculateIntersection(CLine& line1, CLine& line2)
{
	POINTF p;

	POINTF a;
	a.x = line1.end.x - line1.start.x;
	a.y = line1.end.y - line1.start.y;
	POINTF b;
	b.x = line2.start.x - line2.end.x;
	b.y = line2.start.y - line2.end.y;
	POINTF c;
	c.x = line1.start.x - line2.start.x;
	c.y = line1.start.y - line2.start.y;

	float fDenominator = a.y * b.x - a.x * b.y;

	if (fDenominator > 0.0000001f || fDenominator < -0.0000001f)
	{
		float fReciprocal = 1.0f / fDenominator;
		float na = (b.y * c.x - b.x * c.y) * fReciprocal;

		p.x = a.x * na + line1.start.x;
		p.y = a.y * na + line1.start.y;

		if (p.x < 0.0f || p.y < 0.0f)
		{
			p.x = 0.0f;
			p.y = 0.0f;
		}
	}

	return p;
}


float CAlxView::CalculateAngle(CLine& line1, CLine& line2)
{
	float v1 = (line1.end.x - line1.start.x) * m_rcTarget.Width();
	float v2 = (line1.end.y - line1.start.y) * m_rcTarget.Height();
	float v3 = (line2.end.x - line2.start.x) * m_rcTarget.Width();
	float v4 = (line2.end.y - line2.start.y) * m_rcTarget.Height();

	float fAngle0 = (v1 * v3 + v2 * v4) / ((sqrt(v1 * v1 + v2 * v2)) * (sqrt(v3 * v3 + v4 * v4)));

	return acos(fAngle0) * R2A;
}

float CAlxView::CalculateAngle(CLine& line1, CLine& line2, POINTF isp)
{
	float v1 = (line1.end.x - isp.x) * m_rcTarget.Width();
	float v2 = (line1.end.y - isp.y) * m_rcTarget.Height();
	float v3 = (line2.end.x - isp.x) * m_rcTarget.Width();
	float v4 = (line2.end.y - isp.y) * m_rcTarget.Height();

	float fAngle0 = (v1 * v3 + v2 * v4) / ((sqrt(v1 * v1 + v2 * v2)) * (sqrt(v3 * v3 + v4 * v4)));

	return acos(fAngle0) * R2A;
}


void CAlxView::CalculateTxtPoint(CLine& line1, CLine& line2, POINTF& isp, POINTF& tp1, POINTF& tp2)
{
	float fcx1 = line1.end.x - isp.x;
	float fcy1 = line1.end.y - isp.y;
	float fd1 = sqrt(fcx1 * fcx1 + fcy1 * fcy1);

	float fcx2 = line2.end.x - isp.x;
	float fcy2 = line2.end.y - isp.y;
	float fd2 = sqrt(fcx2 * fcx2 + fcy2 * fcy2);

	float fcx3 = line1.start.x - isp.x;
	float fcy3 = line1.start.y - isp.y;
	float fd3 = sqrt(fcx3 * fcx3 + fcy3 * fcy3);

	float fr = 0.0f;

	POINTF pt;

	if (fd1 < fd2)
	{
		fr = fd1 / fd2;

		pt.x = isp.x + fcx2 * fr;
		pt.y = isp.y + fcy2 * fr;

		pt.x = (pt.x + line1.end.x) / 2.0f;
		pt.y = (pt.y + line1.end.y) / 2.0f;
	}
	else
	{
		fr = fd2 / fd1;

		pt.x = isp.x + fcx1 * fr;
		pt.y = isp.y + fcy1 * fr;

		pt.x = (pt.x + line2.end.x) / 2.0f;
		pt.y = (pt.y + line2.end.y) / 2.0f;
	}

	tp1.x = abs(pt.x + isp.x) / 2.0f;
	tp1.y = abs(pt.y + isp.y) / 2.0f;
	
	fr = 0.0f;

	if (fd3 < fd2)
	{
		fr = fd3 / fd2;

		pt.x = isp.x + fcx2 * fr;
		pt.y = isp.y + fcy2 * fr;

		pt.x = (pt.x + line1.start.x) / 2.0f;
		pt.y = (pt.y + line1.start.y) / 2.0f;
	}
	else
	{
		fr = fd2 / fd3;

		pt.x = isp.x + fcx3 * fr;
		pt.y = isp.y + fcy3 * fr;

		pt.x = (pt.x + line2.end.x) / 2.0f;
		pt.y = (pt.y + line2.end.y) / 2.0f;
	}
	
	tp2.x = abs(pt.x + isp.x) / 2.0f;
	tp2.y = abs(pt.y + isp.y) / 2.0f;
}

void CAlxView::FindCrossLines(int idx)
{
	if (m_bCalCrossAngle)
	{
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			if (i != idx)
			{
				if (IsCross2Lines(m_lines[i], m_lines[idx]))
				{
					POINTF isp = CalculateIntersection(m_lines[i], m_lines[idx]);
					float angle = CalculateAngle(m_lines[i], m_lines[idx], isp);

					POINTF p1;
					POINTF p2;
					CalculateTxtPoint(m_lines[i], m_lines[idx], isp, p1, p2);

					m_intersections.push_back(CIntersection(i, idx, angle, isp, p1, p2));
				}
			}
		}
	}
	
}

void CAlxView::UpdateCrossLines(int idx)
{
	if (m_bCalCrossAngle)
	{
		std::list<CIntersection>::iterator itor = m_intersections.begin();

		// 删除相关的交点
		while (itor != m_intersections.end())
		{
			if (itor->ContainLine(idx))
			{
				itor = m_intersections.erase(itor);
			}
			else
			{
				++itor;
			}
		}

		// 重新计算交点
		FindCrossLines(idx);
	}
}

void CAlxView::HitLineCross(CPoint point, int r /* = 10 */)
{
	std::list<CIntersection>::iterator itor;
	for (itor = m_intersections.begin(); itor != m_intersections.end(); ++itor)
	{
		point.x -= m_rcTarget.left;
		point.y -= m_rcTarget.top;

		if ((abs(point.x - itor->m_intersection.x * m_rcTarget.Width()) < r) && (abs(point.y - itor->m_intersection.y * m_rcTarget.Height()) < r))
		{
			itor->m_bHitIntersection = true;
		}
		else
		{
			itor->m_bHitIntersection = false;
		}
	}
}


#ifdef _RADIUS_ALGORITHM_1_
void CAlxView::CalculateRadius1()
{
	double pbx = m_TriangleVertexes[0].x * m_rcDcMem.Width();
	double pby = m_TriangleVertexes[0].y * m_rcDcMem.Height();
	double pax = m_TriangleVertexes[1].x * m_rcDcMem.Width();
	double pay = m_TriangleVertexes[1].y * m_rcDcMem.Height();
	double pcx = m_TriangleVertexes[2].x * m_rcDcMem.Width();
	double pcy = m_TriangleVertexes[2].y * m_rcDcMem.Height();

	double abx = pax - pbx;
	double aby = pay - pby;
	double cax = pcx - pax;
	double cay = pcy - pay;
	double bcx = pbx - pcx;
	double bcy = pby - pcy;
	double lc = sqrt(((abx * abx) + (aby * aby)));
	double lb = sqrt(((cax * cax) + (cay * cay)));;
	double la = sqrt(((bcx * bcx) + (bcy * bcy)));;

	//cosA=(b2+c2-a2)/2bc
	double cosA = (((lb * lb) + (lc * lc) - (la * la)) / (2 * lb * lc));

	double la2 = la / 2;

	// (cos1/2A)2=(1+cosA)/2
	double cosA2 = sqrt((1 + cosA) / 2);

	// 5、AD=AC/cos1/2A
	double radius = (lb / cosA2) / 2;

	// 弧长半径计算弧度
	// A=2*ARC SIN((L/2)/R)度
	// chord
	double vlcrr = (lc / 2.0f) / radius;
	if ((1.0f - vlcrr) > 0.0001f)
	{
		double vlarr = (la / 2.0f) / radius;
		if ((1.0f - vlarr) > 0.0001f)
		{
			double aboa = (2.0f * asin(vlcrr)) * R2A;

			double lbd = 0.0f;
			double lad = 0.0f;

			double aboc = (2.0f * asin(vlarr)) * R2A;
			double acbo = (180.0f - aboc) / 2.0f;
			if (aboa <= 90.0f)
			{
				double abdo = 180.0f - aboa - acbo;

				lbd = (radius / sin(abdo * A2R)) * sin(aboa * A2R);
				lad = radius - ((radius / sin(abdo * A2R)) * sin(acbo * A2R));
			}
			else
			{
				double abod = 180.0f - aboa;
				double abdo = 180.0f - abod - acbo;

				lbd = (radius / sin(abdo * A2R)) * sin(abod * A2R);
				lad = radius + ((radius / sin(abdo * A2R)) * sin(acbo * A2R));
			}

			double vcbx = pcx - pbx;
			double vcby = pcy - pby;
			double vcbxn = vcbx / la;
			double vcbyn = vcby / la;

			double pdx = pbx + vcbxn * lbd;
			double pdy = pby + vcbyn * lbd;

			double vdax = pdx - pax;
			double vday = pdy - pay;
			double vdaxn = vdax / lad;
			double vdayn = vday / lad;

			m_AirwayRadius1 = static_cast<float>(radius);
			m_CenterX1 = static_cast<float>(pax + vdaxn * radius);
			m_CenterY1 = static_cast<float>(pay + vdayn * radius);
		}
		else
		{
			m_CenterX1 = static_cast<float>((pbx + pcx) / 2.0f);
			m_CenterY1 = static_cast<float>((pby + pcy) / 2.0f);
		}

	}
	else
	{
		m_CenterX1 = static_cast<float>((pbx + pax) / 2.0f);
		m_CenterY1 = static_cast<float>((pby + pay) / 2.0f);
	}


	printf("CalculateRadius1 %f  [%f, %f]\n", radius, m_CenterX1, m_CenterY1);
}
#endif // _RADIUS_ALGORITHM_1_

#ifdef _RADIUS_ALGORITHM_2_
void CAlxView::CalculateRadius2()
{
	double centerX = 0;
	double centerY = 0;
	double radius = 0;

	double x1 = m_TriangleVertexes[0].x * m_rcDcMem.Width();
	double y1 = m_TriangleVertexes[0].y * m_rcDcMem.Height();
	double x2 = m_TriangleVertexes[1].x * m_rcDcMem.Width();
	double y2 = m_TriangleVertexes[1].y * m_rcDcMem.Height();
	double x3 = m_TriangleVertexes[2].x * m_rcDcMem.Width();
	double y3 = m_TriangleVertexes[2].y * m_rcDcMem.Height();

	double a = x1 - x2;
	double b = y1 - y2;
	double c = x1 - x3;
	double d = y1 - y3;
	double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) / 2.0;
	double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) / 2.0;
	double det = b * c - a * d;

	if (fabs(det) > 1e-9)
	{
		centerX = -(d * e - b * f) / det;
		centerY = -(a * f - c * e) / det;
		radius = hypot(x1 - centerX, y1 - centerY);
	}

	m_AirwayRadius2 = static_cast<float>(radius);
	m_CenterX2 = static_cast<float>(centerX);
	m_CenterY2 = static_cast<float>(centerY);

	printf("CalculateRadius2 %f  [%f, %f]\n", radius, m_CenterX2, m_CenterY2);
}
#endif // _RADIUS_ALGORITHM_2_

#ifndef _DISCARD_CODE_
void CAlxView::CalculateRadius3()
{
	double pbx = m_TriangleVertexes[0].x * m_rcDcMem.Width();
	double pby = m_TriangleVertexes[0].y * m_rcDcMem.Height();
	double pax = m_TriangleVertexes[1].x * m_rcDcMem.Width();
	double pay = m_TriangleVertexes[1].y * m_rcDcMem.Height();
	double pcx = m_TriangleVertexes[2].x * m_rcDcMem.Width();
	double pcy = m_TriangleVertexes[2].y * m_rcDcMem.Height();

	double abx = pax - pbx;
	double aby = pay - pby;
	double cax = pcx - pax;
	double cay = pcy - pay;
	double bcx = pbx - pcx;
	double bcy = pby - pcy;
	double lc = sqrt(((abx * abx) + (aby * aby)));
	double lb = sqrt(((cax * cax) + (cay * cay)));;
	double la = sqrt(((bcx * bcx) + (bcy * bcy)));;

	//cosA=(b2+c2-a2)/2bc
	double cosA = (((lb * lb) + (lc * lc) - (la * la)) / (2 * lb * lc));

	double sinA = sin(acos(cosA));

	double radius = la / sinA / 2;

	printf("CalculateRadius3 %f\n", radius);
}

void CAlxView::CalculateRadius4()
{
	float centerX = 0;
	float centerY = 0;
	float radius = 0;

	double x1 = m_TriangleVertexes[0].x * m_rcDcMem.Width();
	double y1 = m_TriangleVertexes[0].y * m_rcDcMem.Height();
	double x2 = m_TriangleVertexes[1].x * m_rcDcMem.Width();
	double y2 = m_TriangleVertexes[1].y * m_rcDcMem.Height();
	double x3 = m_TriangleVertexes[2].x * m_rcDcMem.Width();
	double y3 = m_TriangleVertexes[2].y * m_rcDcMem.Height();

	double a = 2 * (x2 - x1);
	double b = 2 * (y2 - y1);
	double c = x2*x2 + y2*y2 - x1*x1 - y1*y1;
	double d = 2 * (x3 - x2);
	double e = 2 * (y3 - y2);
	double f = x3*x3 + y3*y3 - x2*x2 - y2*y2;
	centerX = (b*f - e*c) / (b*d - e*a);
	centerY = (d*c - a*f) / (b*d - e*a);

	//x=((C1*B2)-(C2*B1))/((A1*B2)-(A2*B1))；
	//y = ((A1*C2) - (A2*C1)) / ((A1*B2) - (A2*B1))
	centerX = ((c*e) - (f*b)) / ((a*e) - (d*b));
	centerY = ((a*f) - (d*c)) / ((a*e) - (d*b));
	radius = sqrt((centerX - x1)*(centerX - x1) + (centerY - y1)*(centerY - y1));



	printf("CalculateRadius4 %f\n", radius);
}
#endif // !_DISCARD_CODE_



BEGIN_MESSAGE_MAP(CAlxView, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAlxView 消息处理程序
void CAlxView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bRender)
	{
		bool isEnableDrawLine = true;
		if (m_ValidRectSelectPhase != ImgValidRectSelectPhase::Disable && m_ValidRectSelectPhase != ImgValidRectSelectPhase::Finish)
		{
			if (m_ValidRectSelectPhase == ImgValidRectSelectPhase::TLSelecting)
			{
				float x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				float y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				m_ValidRectLeft = m_ValidRectRight = x;
				m_ValidRectTop = m_ValidRectBottom = y;

				m_ValidRectSelectPhase = ImgValidRectSelectPhase::BRSelecting;
			}
			isEnableDrawLine = false;
		}

		if (m_VertexesSelectPhase != TriangleVertexesSelectPhase::Disable && m_VertexesSelectPhase != TriangleVertexesSelectPhase::Finish)
		{
			isEnableDrawLine = false;
		}

		if (isEnableDrawLine)
		{
			if (m_bLineDrawing)
			{
				if (m_lines.size() > 0)
				{
					size_t idx = m_lines.size() - 1;

					m_lines[idx].end.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
					m_lines[idx].end.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

					// 如果画的线过短，小于10像素，删除
					if (m_lines[idx].LengthSquare() < 0.0003f)
					{
						m_lines.pop_back();
					}
					else
					{
						// 计算焦点
						FindCrossLines((int)idx);
					}
				}

				m_bLineDrawing = FALSE;
			}
			else
			{
				RECT rc;
				GetClientRect(&rc);

				int w = rc.right - rc.left;
				int h = rc.bottom - rc.top;

				for (unsigned int i = 0; i < m_lines.size(); ++i)
				{
					m_nDrag = PickLine(point, i, 5);
					if (m_nDrag > 0)
					{
						m_nLineIdx = i;
						break;
					}
					else
					{
						m_nLineIdx = -1;
					}
				}

				if (m_nDrag <= 0)
				{
					POINTF p;

					p.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
					p.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();
					m_lines.push_back(CLine(p, p));

					m_bLineDrawing = TRUE;
				}
			}
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}


void CAlxView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(rc);
	printf("[%d, %d][%d, %d][%f, %f]\n", rc.Width(), rc.Height(), point.x, point.y,
		((((float)point.x / (float)rc.Width())) * 1920.0f), ((((float)point.y / (float)rc.Height())) * 1080.0f));

	if (m_bRender)
	{
		bool isEnableDrawLine = true;
		if (m_ValidRectSelectPhase != ImgValidRectSelectPhase::Disable && m_ValidRectSelectPhase != ImgValidRectSelectPhase::Finish)
		{
			if (m_ValidRectSelectPhase == ImgValidRectSelectPhase::BRSelecting)
			{
				m_ValidRectRight = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_ValidRectBottom = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				m_IsValidRectSelected = true;
				m_ValidRectSelectPhase = ImgValidRectSelectPhase::Finish;

				::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMG_VALID_RECT_SELECT_FINISH, m_nViewIdx, 0);
			}
			isEnableDrawLine = false;
		}

		if (m_VertexesSelectPhase != TriangleVertexesSelectPhase::Disable && m_VertexesSelectPhase != TriangleVertexesSelectPhase::Finish)
		{
			isEnableDrawLine = false;
			float x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
			float y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();
			

			POINTF p;
			p.x = x;
			p.y = y;
			m_TriangleVertexes.push_back(p);

			if (m_VertexesSelectPhase == TriangleVertexesSelectPhase::Selecting1)
			{
				m_VertexesSelectPhase = TriangleVertexesSelectPhase::Selecting2;
			}
			else if (m_VertexesSelectPhase == TriangleVertexesSelectPhase::Selecting2)
			{
				m_VertexesSelectPhase = TriangleVertexesSelectPhase::Selecting3;
			}
			else
			{
				::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_TRIANGLE_VERTEXES_SELECT_FINISH, m_nViewIdx, 0);
				m_VertexesSelectPhase = TriangleVertexesSelectPhase::Finish;
			}
		}

		if (isEnableDrawLine)
		{
			if (1 == m_nDrag)
			{
				m_lines[m_nLineIdx].start.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_lines[m_nLineIdx].start.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				// 如果画的线过短，小于10像素，删除
				if (m_lines[m_nLineIdx].LengthSquare() < 0.0002f)
				{
					m_lines.erase(m_lines.begin() + m_nLineIdx);
				}
				else
				{
					// 计算焦点
					UpdateCrossLines(m_nLineIdx);
				}

				m_nDrag = 0;
			}
			else if (2 == m_nDrag)
			{
				m_lines[m_nLineIdx].end.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_lines[m_nLineIdx].end.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				// 如果画的线过短，小于10像素，删除
				if (m_lines[m_nLineIdx].LengthSquare() < 0.0002f)
				{
					m_lines.erase(m_lines.begin() + m_nLineIdx);
				}
				else
				{
					// 计算焦点
					UpdateCrossLines(m_nLineIdx);
				}

				m_nDrag = 0;
			}
		}
		
	}

	CStatic::OnLButtonUp(nFlags, point);
}


void CAlxView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	if (m_bNormalSize)
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_DB_CLICK, m_nViewIdx, VIEW_MAX_SIZE);
	}
	else
	{
		if (m_bMixtureRending && m_rcMixture.PtInRect(point))
		{
			::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_DB_CLICK, (m_nViewIdx + 1) % 2, VIEW_MAX_SIZE);
		}
		else
		{
			::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_DB_CLICK, m_nViewIdx, VIEW_NORMAL_SIZE);
		}
	}
// 	m_bNormalSize = !m_bNormalSize;
	if (m_bLineDrawing)
	{
		size_t idx = m_lines.size() - 1;

		if ((m_lines[idx].end.x - m_lines[idx].start.x < 0.000000001f) && (m_lines[idx].end.y - m_lines[idx].start.y < 0.000000001f))
		{
			m_lines.pop_back();
		}

		m_bLineDrawing = FALSE;
	}
	m_nDrag = 0;
	m_nLineHandle = 0;

	CStatic::OnLButtonDblClk(nFlags, point);
}

void CAlxView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (FALSE == m_bZoom)
	{
		m_bZoom = TRUE;
		m_zoomPoint = point;
	}
	CStatic::OnRButtonDown(nFlags, point);
}


void CAlxView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bZoom)
	{
		m_bZoom = FALSE;
		int distance = point.x - m_zoomPoint.x;
		int direction = ((distance > 0) ? 1 : 0);
		distance = abs(distance);

		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_RESIZE, direction, distance);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_RESIZE_STOP, 0, 0);
	}
	CStatic::OnRButtonUp(nFlags, point);
}

void CAlxView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bRender)
	{
		bool isEnableDrawLine = true;
		if (m_ValidRectSelectPhase != ImgValidRectSelectPhase::Disable && m_ValidRectSelectPhase != ImgValidRectSelectPhase::Finish)
		{
			if (m_ValidRectSelectPhase == ImgValidRectSelectPhase::BRSelecting)
			{
				m_ValidRectRight = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_ValidRectBottom = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();
			}
			isEnableDrawLine = false;
		}

		if (isEnableDrawLine)
		{
			if (m_bLineDrawing)
			{
				if (m_lines.size() > 0)
				{
					size_t idx = m_lines.size() - 1;

					m_lines[idx].end.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
					m_lines[idx].end.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

					UpdateCrossLines((int)idx);
				}
			}
			else
			{
				if (m_lines.size() > 0)
				{
					RECT rc;
					GetClientRect(&rc);
					int w = rc.right - rc.left;
					int h = rc.bottom - rc.top;

					for (unsigned int i = 0; i < m_lines.size(); ++i)
					{
						m_nLineHandle = PickLine(point, i, 5);
						if (m_nLineHandle > 0)
						{
							m_nLineIdx = i;
							break;
						}
					}
				}
			}

			if (1 == m_nDrag)
			{
				m_lines[m_nLineIdx].start.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_lines[m_nLineIdx].start.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				UpdateCrossLines(m_nLineIdx);
			}
			else if (2 == m_nDrag)
			{
				m_lines[m_nLineIdx].end.x = (float)(point.x - m_rcTarget.left) / (float)m_rcTarget.Width();
				m_lines[m_nLineIdx].end.y = (float)(point.y - m_rcTarget.top) / (float)m_rcTarget.Height();

				UpdateCrossLines(m_nLineIdx);
			}

			if (m_bCalCrossAngle)
			{
				HitLineCross(point);
			}
		}

		Invalidate();
	}

	if (m_bZoom)
	{
		int distance = point.x - m_zoomPoint.x;
		int direction = ((distance > 0) ? 1 : 0);
		distance = abs(distance);

		if (distance > 5)
		{
			m_zoomPoint = point;
			::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_VIEW_RESIZE, direction, distance);
		}

		CRect rc;
		GetClientRect(rc);

		if (point.x <= 0 || point.x >= rc.Width() || point.y <= 0 || point.y >= rc.Height())
		{
			m_bZoom = FALSE;
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CAlxView::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	m_rcTarget = CalculateTargetRect();
}
