
// BUltrasonicExpansion.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBUltrasonicExpansionApp: 
// �йش����ʵ�֣������ BUltrasonicExpansion.cpp
//

class CBUltrasonicExpansionApp : public CWinApp
{
public:
	CBUltrasonicExpansionApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
	

private:
	Gdiplus::GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR                    m_GdiplusToken;
};

extern CBUltrasonicExpansionApp theApp;