#ifndef _JH_OKI_TRANSFER_H_
#define _JH_OKI_TRANSFER_H_

#include <afxwin.h>

#ifdef _WIN64
#define OKI_DLL_NAME "okapi64.dll"
#else
#define OKI_DLL_NAME "OKAPI32.dll"
#endif

#ifdef _WIN64
typedef		__int64				MLONG, *PMLONG, *LPMLONG;  //which are 4 bytes in 32 system, 8 bytes in 64 system 
typedef		unsigned __int64	MWORD, *PMWORD, *LPMWORD;  //which are 4 bytes in 32 system, 8 bytes in 64 system 
#define MAX_VALID_FRAME 0x400000000000000
#else
typedef		long				MLONG, *PMLONG, *LPMLONG;	//which are 4 bytes in 32 system, 8 bytes in 64 system 
typedef		unsigned long		MWORD, *PMWORD, *LPMWORD;  //which are 4 bytes in 32 system, 8 bytes in 64 system 
#define MAX_VALID_FRAME 0x40000000
#endif

typedef		MLONG 			TARGET;

#define		BUFFER			(TARGET)1	//Buffer(physical) allocated from host memory
#define		VIDEO			(TARGET)0	//Video source input to the board 
#define		SCREEN			(TARGET)-1	//Screen supported by VGA
#define		FRAME			(TARGET)-2	//Frame buffer on the board
#define		GRAPH			(TARGET)-4	//Graph frame on the board 
#define		MONITOR			(TARGET)-3	//Monitor(image out) supported by (D/A) TV standard

#define		CAPTURE_SCRRGBFORMAT	3
#define		CAPTURE_BUFRGBFORMAT	4

#define		GETCURRPARAM			-1 

#define		FORM_RGB565				2
#define		FORM_RGB555				3
#define		FORM_RGB8888			4

typedef struct _blockinfo {
	SHORT	iType;	//=BK or SQ, BM
	//struct _blocksize;
	SHORT	iWidth;		//width *
	SHORT	iHeight;	//height *
	SHORT	iBitCount; //pixel bytes iBitCount *
	SHORT	iFormType;	//rgb format type, need to fill when RGB565 or RGB 555
	union {
		struct {
			SHORT	lBlockStep; //block stride (step to next image header)
			SHORT	iHiStep;// HIWORD of block stride
		};
		DWORD	dwBlockStep;
	};
	union {
		struct {
			SHORT	lTotal;	//frame num
			SHORT	iHiTotal;// HIWORD of total
		};
		DWORD	dwTotal;
	};
	SHORT	iInterval; //frame interval
	LPBYTE	lpBits;// image data pointer / file path name
	LPBYTE	lpExtra;// extra data (like as palette, mask) pointer
} BLOCKINFO, *LPBLOCKINFO;

typedef HANDLE(WINAPI *pfokOpenBoard)(MLONG *iIndex);
typedef BOOL(WINAPI *pfokCloseBoard)(HANDLE hBoard);
typedef MLONG(WINAPI *pfokSetTargetRect)(HANDLE hBoard, TARGET target, LPRECT lpTgtRect);
typedef BOOL(WINAPI *pfokSetSeqCallback)(HANDLE hBoard, BOOL CALLBACK BeginProc(HANDLE hBoard), BOOL CALLBACK SeqProc(HANDLE hBoard, MLONG No), BOOL CALLBACK EndProc(HANDLE hBoard));
typedef MLONG(WINAPI *pfokCaptureTo)(HANDLE hBoard, TARGET Dest, MLONG start, MLONG lParam);
typedef MLONG(WINAPI *pfokStopCapture)(HANDLE hBoard);
typedef MLONG(WINAPI *pfokSetCaptureParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);
typedef LPVOID(WINAPI *pfokGetTargetInfo)(HANDLE hBoard, TARGET target, MLONG lNoFrame, SHORT *wid, SHORT *ht, MLONG *stride);
typedef INT32(WINAPI *pfokConvertRect)(HANDLE hBoard, TARGET dst, MLONG first, TARGET src, MLONG start, MLONG lParam);
typedef MLONG(WINAPI *pfokSaveImageFile)(HANDLE hBoard, LPSTR szFileName, MLONG first, TARGET target, MLONG start, MLONG num);

#endif
