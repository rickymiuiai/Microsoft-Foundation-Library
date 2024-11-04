#pragma once
#include "VideoCaptureBase.h"
#include "JhOkiTransfer.h"

class COkiVideoCapture : public CVideoCaptureBase
{
public:
	COkiVideoCapture();
	virtual ~COkiVideoCapture();

public:
	bool Init(CDC* pCDC, int x, int y, int w, int h) override;
	bool UnInit() override;
	bool OpenVideoCapture(int id) override;
	bool CloseVideoCapture() override;
	bool SaveImage(std::string imgname) override;
	int  Width() const override { return m_Rect.Width(); }
	int  Height() const override { return m_Rect.Height(); }

public:
	static BOOL CALLBACK BeginCapture(HANDLE hBoard);
	static BOOL CALLBACK Process(HANDLE hBoard, MLONG no);
	static BOOL CALLBACK EndCapture(HANDLE hBoard);

private:
	BOOL InternalBeginCapture(HANDLE hBoard);
	BOOL InternalProcess(HANDLE hBoard, MLONG no);
	BOOL InternalEndCapture(HANDLE hBoard);
	long exSetBitmapHeader(LPBITMAPINFOHEADER lpbi, short width, short height, short bits, short form);
	long exGetTargetSize(HANDLE hBoard, TARGET tgt, short *width, short *height);

private:
	HMODULE                  m_hDll;
	pfokOpenBoard            m_pfokOpenBoard;
	pfokCloseBoard           m_pfokCloseBoard;
	pfokSetTargetRect        m_pfokSetTargetRect;
	pfokSetSeqCallback       m_pfokSetSeqCallback;
	pfokCaptureTo            m_pfokCaptureTo;
	pfokStopCapture          m_pfokStopCapture;
	pfokSetCaptureParam      m_pfokSetCaptureParam;
	pfokGetTargetInfo        m_pfokGetTargetInfo;
	pfokConvertRect          m_pfokConvertRect;
	pfokSaveImageFile        m_pfokSaveImageFile;
	int                      m_nDibSize;
	HANDLE                   m_hBoard;
	LPBYTE                   m_lpDib;
	LPBITMAPINFOHEADER       m_lpBi;
	CRect                    m_Rect;
	CBitmap                  m_Bitmap;
	BLOCKINFO                m_blk;

	static COkiVideoCapture* This;
};

