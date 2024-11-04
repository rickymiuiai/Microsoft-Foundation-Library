#pragma once
#include "afxwin.h"

#include "Common.h"
#include "../GDIPlus/Includes/GdiPlus.h"

#include <vector>
#include <list>

#include "IVideoCapture.h"

// algorithm
#define _DISCARD_CODE_
#define _RADIUS_ALGORITHM_1_
#define _RADIUS_ALGORITHM_2_

class CLine
{
public:
	CLine();
	CLine(POINTF s, POINTF e);
	virtual ~CLine();

public:
	float LengthSquare();
	float Length();

public:
	POINTF start;
	POINTF end;
};

enum class ImgValidRectSelectPhase
{
	Disable,
	TLSelecting,
	BRSelecting,
	Finish
};

enum class TriangleVertexesSelectPhase
{
	Disable,
	Selecting1,
	Selecting2,
	Selecting3,
	Finish
};

class CIntersection
{
public:
	CIntersection();
	CIntersection(int idx1, int idx2, float angle, POINTF isp);
	CIntersection(int idx1, int idx2, float angle, POINTF isp, POINTF p1, POINTF p2);
	virtual ~CIntersection();

public:
	bool ContainLine(int idx);

public:
	bool   m_bHitIntersection;
	int    m_nIdxLine1;
	int    m_nIdxLine2;
	float  m_fAngle1;
	float  m_fAngle2;
	POINTF m_intersection;
	POINTF m_txtPoint1;
	POINTF m_txtPoint2;
};

class CAlxView : public CStatic
{
	DECLARE_DYNAMIC(CAlxView)
public:
	CAlxView();
	virtual ~CAlxView();

public:
	void RegistView(CAlxView* pView) { m_pView = pView; }
	void RegistVideoCapture(IVideoCapture* pVideoCapture) { m_pVideoCapture = pVideoCapture; }
	void FullScreen(BOOL fs, BOOL mix);
	virtual void Init(int sw = 1920, int sh = 1080);
	virtual void Uninit();
	void Clear();
	void Rending();
	void ForceReleaseRMbtn();
	virtual void Snap() = 0;
	virtual CDC* RenderMemoryDC() = 0;

	bool IsValidRectSelected();
	void ShowValidRect();
	void SkipValidRectSelect();
	void SetImgRectScale(float scale);
	void StartSelectValidRect();
	void CloseSelectValidRect();
	void StartSelectTriangleVertexes();
	void CloseSelectTriangleVertexes();
	void CalculateAirwayRadius();
	void CalculateAirwayRadius(int id);
	float AirwayRadius();
	
protected:
	int PickLine(CPoint point, int idx, int r = 5);
	CRect CalculateTargetRect();
	bool IsCross2Lines(CLine& line1, CLine& line2);
	POINTF CalculateIntersection(CLine& line1, CLine& line2);
	float CalculateAngle(CLine& line1, CLine& line2);
	float CalculateAngle(CLine& line1, CLine& line2, POINTF isp);
	void CalculateTxtPoint(CLine& line1, CLine& line2, POINTF& isp, POINTF& tp1, POINTF& tp2);
	void FindCrossLines(int idx);
	void UpdateCrossLines(int idx);
	void HitLineCross(CPoint point, int r = 10);

#ifdef _RADIUS_ALGORITHM_1_
	void CalculateRadius1();
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
	void CalculateRadius2();
#endif // _RADIUS_ALGORITHM_2_
	
#ifndef _DISCARD_CODE_
	void CalculateRadius3();
	void CalculateRadius4();
#endif // !_DISCARD_CODE_

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	bool                     m_IsValidRectSelected;
	bool                     m_IsCalRadius;
	BOOL                     m_bRender;
	BOOL                     m_bLineDrawing;
	BOOL                     m_bNormalSize;
	BOOL                     m_bZoom;
	BOOL                     m_bMixtureRending;
	BOOL                     m_bHasMixture;
	BOOL                     m_bCalCrossAngle;
	BOOL                     m_bRendingAngle;
	int                      m_nDrag;
	int                      m_nLineIdx;
	int                      m_nLineHandle;
	int                      m_nViewIdx;
	int                      m_nRadiusAlgorithm;
	ImgValidRectSelectPhase     m_ValidRectSelectPhase;
	TriangleVertexesSelectPhase m_VertexesSelectPhase;
	float                    m_ValidRectLeft;
	float                    m_ValidRectTop;
	float                    m_ValidRectRight;
	float                    m_ValidRectBottom;
	float                    m_ImgRectScale;
#ifdef _RADIUS_ALGORITHM_1_
	float                    m_AirwayRadius1;
	float                    m_CenterX1;
	float                    m_CenterY1;
#endif // _RADIUS_ALGORITHM_1_
#ifdef _RADIUS_ALGORITHM_2_
	float                    m_AirwayRadius2;
	float                    m_CenterX2;
	float                    m_CenterY2;
#endif // _RADIUS_ALGORITHM_2_
	IVideoCapture*           m_pVideoCapture;
	CAlxView*                m_pView;
	Gdiplus::Graphics*       m_graphics;
	Gdiplus::Pen*            m_thickPen;
	Gdiplus::Pen*            m_thinPen;
	Gdiplus::Pen*            m_ValidRectPen;
	Gdiplus::Font            m_Font;
	Gdiplus::SolidBrush      m_sb;
	Gdiplus::SolidBrush      m_sbCircleCenter;
	Gdiplus::StringFormat    m_strFormat;
	CRect                    m_rcTarget;
	CRect                    m_rcDcMem;
	CRect                    m_rcMixture;
	CBitmap                  m_bitmapMemory;
	CBitmap                  m_bitmapMemView;
	CDC                      m_dcMemory;
	CDC                      m_dcMemView;
	std::vector<CLine>       m_lines;
	std::list<CIntersection> m_intersections;
	std::vector<POINTF> m_TriangleVertexes;
	
public:
	CPoint                   m_zoomPoint;
};

