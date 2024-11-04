#include "stdafx.h"
#include "OkiVideoCapture.h"

COkiVideoCapture* COkiVideoCapture::This = nullptr;

COkiVideoCapture::COkiVideoCapture() : 
	CVideoCaptureBase()
  , m_hDll(NULL)
  , m_pfokOpenBoard(nullptr)
  , m_pfokCloseBoard(nullptr)
  , m_pfokSetTargetRect(nullptr)
  , m_pfokSetSeqCallback(nullptr)
  , m_pfokCaptureTo(nullptr)
  , m_pfokStopCapture(nullptr)
  , m_pfokSetCaptureParam(nullptr)
  , m_pfokGetTargetInfo(nullptr)
  , m_pfokConvertRect(nullptr)
  , m_pfokSaveImageFile(nullptr)
  , m_nDibSize(1920 * 1080 * 8)
  , m_hBoard(INVALID_HANDLE_VALUE)
  , m_lpDib(nullptr)
  , m_lpBi(nullptr)
  , m_Rect(0, 0, 1920, 1080)
  , m_Bitmap()
  , m_blk()
{
	This = this;

	m_hDll = LoadLibrary(_T(OKI_DLL_NAME));

	if (m_hDll != NULL)
	{
		m_pfokOpenBoard = (pfokOpenBoard)GetProcAddress(m_hDll, "okOpenBoard");
		m_pfokCloseBoard = (pfokCloseBoard)GetProcAddress(m_hDll, "okCloseBoard");
		m_pfokSetTargetRect = (pfokSetTargetRect)GetProcAddress(m_hDll, "okSetTargetRect");
		m_pfokSetSeqCallback = (pfokSetSeqCallback)GetProcAddress(m_hDll, "okSetSeqCallback");
		m_pfokCaptureTo = (pfokCaptureTo)GetProcAddress(m_hDll, "okCaptureTo");
		m_pfokStopCapture = (pfokStopCapture)GetProcAddress(m_hDll, "okStopCapture");
		m_pfokSetCaptureParam = (pfokSetCaptureParam)GetProcAddress(m_hDll, "okSetCaptureParam");
		m_pfokGetTargetInfo = (pfokGetTargetInfo)GetProcAddress(m_hDll, "okGetTargetInfo");
		m_pfokConvertRect = (pfokConvertRect)GetProcAddress(m_hDll, "okConvertRect");
		m_pfokSaveImageFile = (pfokSaveImageFile)GetProcAddress(m_hDll, "okSaveImageFile");
	}
}

COkiVideoCapture::~COkiVideoCapture()
{
	UnInit();

	if (m_hDll != NULL)
	{
		m_pfokOpenBoard = nullptr;
		m_pfokCloseBoard = nullptr;
		m_pfokSetTargetRect = nullptr;
		m_pfokSetSeqCallback = nullptr;
		m_pfokCaptureTo = nullptr;
		m_pfokStopCapture = nullptr;
		m_pfokSetCaptureParam = nullptr;
		m_pfokGetTargetInfo = nullptr;
		m_pfokConvertRect = nullptr;
		m_pfokSaveImageFile = nullptr;

		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

bool COkiVideoCapture::Init(CDC* pCDC, int x, int y, int w, int h)
{
	bool  ret = false;
	MLONG lIndex = -1;

	m_hBoard = m_pfokOpenBoard(&lIndex);

	if (m_hBoard != INVALID_HANDLE_VALUE)
	{
		m_Rect.SetRect(x, y, w, h);
		m_nDibSize = m_Rect.Width() * m_Rect.Height() * 8;
		m_lpBi = (LPBITMAPINFOHEADER)GlobalAlloc(GPTR, m_nDibSize + 5120);
		m_lpDib = (LPBYTE)m_lpBi + 5120;

		m_pCDC = new CDC();
		m_pCDC->CreateCompatibleDC(pCDC);

		m_Bitmap.CreateCompatibleBitmap(pCDC, m_Rect.Width(), m_Rect.Height());
		m_pCDC->SelectObject(m_Bitmap);

		m_pCDC->SetStretchBltMode(STRETCH_HALFTONE);

		ret = true;
	}

	return ret;
}

bool COkiVideoCapture::UnInit()
{
	CloseVideoCapture();

	m_pfokCloseBoard(m_hBoard);

	m_Bitmap.DeleteObject();

	if (m_pCDC != NULL)
	{
		m_pCDC->DeleteDC();

		delete m_pCDC;
		m_pCDC = nullptr;
	}
	
	if (m_lpBi != nullptr)
	{
		GlobalFree(m_lpBi);
		m_lpBi = nullptr;
	}

	return true;
}

bool COkiVideoCapture::OpenVideoCapture(int id)
{
	m_IsSnap = false;

	if (false == m_IsRunning)
	{
		m_pfokSetTargetRect(m_hBoard, VIDEO, &m_Rect);//Set Buffer Rect
		m_pfokSetTargetRect(m_hBoard, BUFFER, &m_Rect);//Set Buffer Rect

		m_pfokSetSeqCallback(m_hBoard, BeginCapture, Process, EndCapture);
		m_IsRunning = (m_pfokCaptureTo(m_hBoard, BUFFER, 0, -1) >= 0);
	}

	return m_IsRunning;
}

bool COkiVideoCapture::CloseVideoCapture()
{
	if (m_IsRunning)
	{
		m_pfokStopCapture(m_hBoard);
		m_IsRunning = false;
	}

	return !m_IsRunning;
}

bool COkiVideoCapture::SaveImage(std::string imgname)
{
	m_IsSnap = true;
	m_SnapName = imgname + "_o.jpg";
	return true;
}

BOOL COkiVideoCapture::BeginCapture(HANDLE hBoard)
{
	return This->InternalBeginCapture(hBoard);
}

BOOL COkiVideoCapture::Process(HANDLE hBoard, MLONG no)
{
	return This->InternalProcess(hBoard, no);
}

BOOL COkiVideoCapture::EndCapture(HANDLE hBoard)
{
	return This->InternalEndCapture(hBoard);
}

BOOL COkiVideoCapture::InternalBeginCapture(HANDLE hBoard)
{
	short width = 0;
	short height = 0;
	long  form = 0;

	exGetTargetSize(hBoard, BUFFER, &width, &height);
	form = (long)m_pfokSetCaptureParam(hBoard, CAPTURE_SCRRGBFORMAT, GETCURRPARAM);
	exSetBitmapHeader(m_lpBi, width, height, HIWORD(form), LOWORD(form));

	m_blk.lpBits = m_lpDib;
	m_blk.iBitCount = m_lpBi->biBitCount;
	m_blk.iHeight = -(short)m_lpBi->biHeight;
	m_blk.iWidth = (short)m_lpBi->biWidth;

	return TRUE;
}

BOOL COkiVideoCapture::InternalProcess(HANDLE hBoard, MLONG no)
{
	short  height;
	MLONG  stride;
	LPBYTE lpbuf;
	lpbuf = (LPBYTE)m_pfokGetTargetInfo(hBoard, BUFFER, no, NULL, &height, &stride);

	m_pfokConvertRect(hBoard, (TARGET)&m_blk, 0, BUFFER, (short)no, 1);

	if (m_IsSnap)
	{
		m_pfokSaveImageFile(m_hBoard, const_cast<char*>(m_SnapName.c_str()), 100, BUFFER, no, 1);
		m_IsSnap = false;
	}

	StretchDIBits(m_pCDC->GetSafeHdc(),
		0, 0, m_lpBi->biWidth, m_lpBi->biHeight,
		0, 0, m_lpBi->biWidth, m_lpBi->biHeight,
		m_lpDib, (LPBITMAPINFO)m_lpBi,
		DIB_RGB_COLORS, SRCCOPY);

	if (m_pfRecvNewFrame != nullptr) m_pfRecvNewFrame();

	return TRUE;
}

BOOL COkiVideoCapture::InternalEndCapture(HANDLE hBoard)
{
	m_pfokSetSeqCallback(hBoard, NULL, NULL, NULL);

	return TRUE;
}

long COkiVideoCapture::exSetBitmapHeader(LPBITMAPINFOHEADER lpbi, short width, short height, short bits, short form)
{
	long ret = 0;

	if (lpbi != nullptr)
	{
		long wbytes = 0;

		if (FORM_RGB555 == form)
		{
			bits = 16;
		}

		wbytes = ((((width * bits) + 31) & ~31) >> 3); // bmp need 4 byte align

		lpbi->biWidth = width;
		lpbi->biHeight = height;

		lpbi->biSize = sizeof(BITMAPINFOHEADER);
		lpbi->biPlanes = 1;

		lpbi->biBitCount = bits;
		lpbi->biSizeImage = wbytes * height;

		lpbi->biClrUsed = 0;

		if ((FORM_RGB555 == form) || (FORM_RGB565 == form) || (FORM_RGB8888 == form))
		{
			lpbi->biCompression = BI_BITFIELDS;
		}
		else
		{
			lpbi->biCompression = 0;
		}

		if (BI_BITFIELDS == lpbi->biCompression)
		{
			DWORD* lpmask = nullptr;
			lpmask = (DWORD*)((LPSTR)lpbi + lpbi->biSize);

			if (FORM_RGB555 == form)
			{
				lpmask[2] = 0x001f; // blue
				lpmask[1] = 0x03e0;
				lpmask[0] = 0x7c00;
			}
			else if (FORM_RGB565 == form)
			{
				lpmask[2] = 0x001f; // blue
				lpmask[1] = 0x07e0;
				lpmask[0] = 0xf800;
			}
			else if (32 == bits)
			{
				lpmask[2] = 0x0000ff;
				lpmask[1] = 0x00ff00;
				lpmask[0] = 0xff0000;
			}
		}
		else if (bits <= 14) // 8, 10, 12
		{
			int i = 0;
			int ratio = 0;

			RGBQUAD* lpRGB = (RGBQUAD*)((LPSTR)lpbi + lpbi->biSize);

			lpbi->biClrUsed = (1 << bits);
			ratio = lpbi->biClrUsed / 256;

			for (i = 0; i < (short)lpbi->biClrUsed; ++i)
			{
				lpRGB[i].rgbBlue = i / ratio;
				lpRGB[i].rgbGreen = i / ratio;
				lpRGB[i].rgbRed = i / ratio;
				lpRGB[i].rgbReserved = 0;
			}
		}

		lpbi->biClrImportant = lpbi->biClrUsed;

		ret = lpbi->biClrUsed;
	}

	return ret;
}

long COkiVideoCapture::exGetTargetSize(HANDLE hBoard, TARGET tgt, short *width, short *height)
{
	RECT rect;
	long form;

	if ((SCREEN == tgt) || (BUFFER == tgt))
	{
		rect.right = -1;
		m_pfokSetTargetRect(hBoard, tgt, &rect); // get current rect

		if (width != nullptr)
		{
			*width = (short)(rect.right - rect.left);
		}
		if (height != nullptr)
		{
			*height = (short)(rect.bottom - rect.top);
		}

		if (SCREEN == tgt)
		{
			form = (long)m_pfokSetCaptureParam(hBoard, CAPTURE_SCRRGBFORMAT, GETCURRPARAM);
			// limit to video rect
			rect.right = -1; // max. captured rect
			m_pfokSetTargetRect(hBoard, VIDEO, &rect); // get video rect

			if (width != nullptr)
			{
				if (*width < rect.right - rect.left)
				{
					*width = (short)(rect.right - rect.left);
				}
			}

			if (height != nullptr)
			{
				if (*height < rect.bottom - rect.top)
				{
					*height = (short)(rect.bottom - rect.top);
				}
			}
		}
		else if (BUFFER == tgt)
		{
			form = (long)m_pfokSetCaptureParam(hBoard, CAPTURE_BUFRGBFORMAT, GETCURRPARAM);
		}
	}
	else if (tgt > BUFFER) // from blkinfo
	{
		LPBLOCKINFO lpblk;
		lpblk = (LPBLOCKINFO)tgt;

		if (width != nullptr)
		{
			*width = lpblk->iWidth;
		}
		if (height != nullptr)
		{
			*height = abs(lpblk->iHeight);
		}

		form = MAKELONG(lpblk->iFormType, lpblk->iBitCount);
	}

	return form;
}

