#pragma once
#include <vector>

#include "../GDIPlus/Includes/GdiPlus.h"

#include "Common.h"
#include "AlxView.h"
// CAlxImgView


class CAlxImgView : public CAlxView
{
	DECLARE_DYNAMIC(CAlxImgView)

public:
	CAlxImgView();
	virtual ~CAlxImgView();

public:
	void Init(int sw = 1920, int sh = 1080) override;
	void Uninit() override;
	CDC* RenderMemoryDC() override;
	void Snap() override;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	CBitmap            m_bitmapBuffer;
	CDC                m_dcBuffer;
};


