
// BUltrasonicExpansionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "BUltrasonicExpansionDlg.h"
#include "afxdialogex.h"
#include "DataPersistence.h"
#include "CommonFunction.h"

#include "ImgRectSelectTipsDlg.h"
#include "ImgRectScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL DestroyWindow();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	static_cast<CEdit*>(GetDlgItem(IDC_COPYRIGHT_EDIT))->SetWindowText(_T("Copyright © 2021 中日友好医院&国家呼吸医学中心 All Rights Reserved."));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBUltrasonicExpansionDlg 对话框
CBUltrasonicExpansionDlg::CBUltrasonicExpansionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBUltrasonicExpansionDlg::IDD, pParent)
	, m_imgSnap()
	, m_imgDVI()
	, m_nsw(0)
	, m_nsh(0)
	, m_nNrmViewW(1920)
	, m_nNrmViewH(1080)
	, m_nViewIdx(-1)
	, m_nCalRadiusViewIdx(-1)
	, m_nRadiusAlgorithm(0)
	, m_fRate(0.5f)
	, m_fImgRate(9.0f / 16.0f)
	, m_dwStyle(0)
	, m_dwExStyle(0)
	, m_bPlaying(FALSE)
	, m_pRegPatientDlg(nullptr)
	, m_pPatientListDlg(nullptr)
	, m_pRectSelectTipsDlg(nullptr)
	, m_pRectScaleDlg(nullptr)
	, m_rcClient()
	, m_rcDesktop()
{
	m_imgDVI.RegistView(&m_imgSnap);
	m_imgSnap.RegistView(&m_imgDVI);

	int w = CSystemConfig::Instance()->GrabW;
	int h = CSystemConfig::Instance()->GrabH;

	m_fRate = CSystemConfig::Instance()->ViewScale;
	m_nNrmViewW = (int)(w * m_fRate);
	m_nNrmViewH = (int)(h * m_fRate);
	m_fImgRate = ((float)h / (float)w);

	VideoCaptureType vcType;

	switch (CSystemConfig::Instance()->Type)
	{
		case 0:
		{
			vcType = Oki_Dvi;
			break;
		}
		case 1:
		{
			vcType = Usb;
			break;
		}
		default:
		{
			vcType = Usb;
			break;
		}
	}

	IVideoCapture* pVideoCapture = m_VideoCaptureFactory.CreateVideoCapture(vcType);
	m_imgDVI.RegistVideoCapture(pVideoCapture);
	m_imgSnap.RegistVideoCapture(pVideoCapture);

	m_hIcon      = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBUltrasonicExpansionDlg::~CBUltrasonicExpansionDlg()
{
	if (m_pRegPatientDlg != nullptr)
	{
		delete m_pRegPatientDlg;
		m_pRegPatientDlg = nullptr;
	}

	if (m_pPatientListDlg != nullptr)
	{
		delete m_pPatientListDlg;
		m_pPatientListDlg = nullptr;
	}

	if (m_pRectSelectTipsDlg != nullptr)
	{
		delete m_pRectSelectTipsDlg;
		m_pRectSelectTipsDlg = nullptr;
	}

	if (m_pRectScaleDlg != nullptr)
	{
		delete m_pRectScaleDlg;
		m_pRectScaleDlg = nullptr;
	}
}

void CBUltrasonicExpansionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DVI, m_imgDVI);
	DDX_Control(pDX, IDC_SNAP, m_imgSnap);
}

BEGIN_MESSAGE_MAP(CBUltrasonicExpansionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CBUltrasonicExpansionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBUltrasonicExpansionDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START_BTN, &CBUltrasonicExpansionDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, &CBUltrasonicExpansionDlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, &CBUltrasonicExpansionDlg::OnBnClickedClearBtn)
	ON_MESSAGE(WM_VIEW_DB_CLICK, OnViewDBClick)
	ON_MESSAGE(WM_SNAP_IMG, OnSnapImage)
	ON_MESSAGE(WM_VIEW_RESIZE, OnViewResize)
	ON_MESSAGE(WM_VIEW_RESIZE_STOP, OnViewResizeStop)
	ON_MESSAGE(WM_UPDATE_PATIENT_INFO, OnUpdatePatientInfo)
	ON_MESSAGE(WM_IMG_VALID_RECT_SELECT_FINISH, OnImgValidRectSelectFinish)
	ON_MESSAGE(WM_TRIANGLE_VERTEXES_SELECT_FINISH, OnTriangleVertexesSelectFinish)
	ON_MESSAGE(WM_IMGRECT_SELECT_TIPS_CLOSE, OnImgRectSelectTipsDlgClose)
	ON_MESSAGE(WM_IMGRECT_SCALE_CLOSE, OnImgRectScaleDlgClose)
	ON_BN_CLICKED(IDC_SNAP_BTN, &CBUltrasonicExpansionDlg::OnBnClickedSnapBtn)
	ON_BN_CLICKED(IDC_CLEAR_IMG_BTN, &CBUltrasonicExpansionDlg::OnBnClickedClearImgBtn)
	ON_BN_CLICKED(IDC_ADD_BTN, &CBUltrasonicExpansionDlg::OnBnClickedAddBtn)
	ON_BN_CLICKED(IDC_QUERY_BTN, &CBUltrasonicExpansionDlg::OnBnClickedQueryBtn)
	ON_BN_CLICKED(IDC_OPENFOLDER_BTN, &CBUltrasonicExpansionDlg::OnBnClickedOpenfolderBtn)
	ON_BN_CLICKED(IDC_AIRWAYRADIUS_BTN, &CBUltrasonicExpansionDlg::OnBnClickedAirwayradiusBtn)
	ON_CBN_SELCHANGE(IDC_RADIUS_ALGORITHM_COMBO, &CBUltrasonicExpansionDlg::OnSelchangeRadiusAlgorithmCombo)
END_MESSAGE_MAP()


// CBUltrasonicExpansionDlg 消息处理程序
BOOL CBUltrasonicExpansionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			if (pMsg->wParam == 'C')
			{
			}
			if (pMsg->wParam == 'V')
			{
			}
		}

		if (pMsg->wParam == VK_DELETE)
		{
			if (GRAB_DIV_VIEW == m_nViewIdx)
			{
				m_imgDVI.Clear();
			}
			else if (SNAP_IMG_VIEW == m_nViewIdx)
			{
				m_imgSnap.Clear();
			}
			else
			{

			}
		}

		if (pMsg->wParam == 'S')
		{
			m_imgDVI.Snap();
		}

		if (pMsg->wParam == 'M')
		{
			OnBnClickedAirwayradiusBtn();
		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CBUltrasonicExpansionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SystemParametersInfo(SPI_GETWORKAREA, 0, &m_rcDesktop, 0);

	m_dwStyle = GetStyle();
	m_dwExStyle = GetExStyle();

	m_nsw = GetSystemMetrics(SM_CXSCREEN);
	m_nsh = GetSystemMetrics(SM_CYSCREEN);

	//////////////////////////////////////////////////////////////////////////
	SetWindowPos(&wndTop, 0, 0, m_rcDesktop.Width(), m_rcDesktop.Height(), SWP_HIDEWINDOW);

	GetClientRect(m_rcClient);

	// Calculate View Size
	// NormalSize
	int w = m_nNrmViewW * 2;
	int h = m_nNrmViewH * 2;

	if (w > m_rcClient.Width())
	{
		m_nNrmViewW = m_rcClient.Width() / 2;
		m_nNrmViewH = (int)(m_nNrmViewW * m_fImgRate);
		h = m_nNrmViewH + 150;
	}

	if (h > m_rcClient.Height())
	{
		m_nNrmViewH = m_rcClient.Height() - 150;
		m_nNrmViewW = (int)(m_nNrmViewH / m_fImgRate);
	}

	int x = 0;
	int y = 0;

	w = m_nNrmViewW * 2;
	w = (m_rcClient.Width() - w) / 3;
	x = w;
	y = (m_rcClient.Height() - m_nNrmViewH - 150) / 2;

	m_imgSnap.SetWindowPos(&wndTop, x, y, m_nNrmViewW, m_nNrmViewH, SWP_SHOWWINDOW);
	x += (m_nNrmViewW + w);
	m_imgDVI.SetWindowPos(&wndTop, x, y, m_nNrmViewW, m_nNrmViewH, SWP_SHOWWINDOW);

	x = 0;
	y = m_rcClient.Height() - 65;//35;
	w = 100;
	h = 30;
	static_cast<CButton*>(GetDlgItem(IDC_START_BTN))->SetWindowPos(&wndTop, x, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_STOP_BTN))->SetWindowPos(&wndTop, x + w + 20, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_CLEAR_IMG_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 2, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_CLEAR_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 3, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_SNAP_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 4, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_ADD_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 5, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_QUERY_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 6, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_OPENFOLDER_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 7, y, w, h, SWP_SHOWWINDOW);
	static_cast<CButton*>(GetDlgItem(IDC_AIRWAYRADIUS_BTN))->SetWindowPos(&wndTop, x + (w + 20) * 8, y, w, h, SWP_SHOWWINDOW);

	static_cast<CComboBox*>(GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO))->AddString(_T("半径算法1"));
	static_cast<CComboBox*>(GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO))->AddString(_T("半径算法2"));
	static_cast<CComboBox*>(GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO))->SetCurSel(0);
	static_cast<CComboBox*>(GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO))->SetWindowPos(&wndTop, x + (w + 20) * 9, y, w, h, SWP_SHOWWINDOW);

	GetDlgItem(IDC_PATIENTINFO_STATIC)->SetWindowPos(&wndTop, x, y - 30, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);

	GetDlgItem(IDC_COPYRIGHT_EDIT)->SetWindowPos(&wndTop, ((m_rcClient.Width() - 510) / 2), y + 35, 510, 30, SWP_SHOWWINDOW);
	static_cast<CEdit*>(GetDlgItem(IDC_COPYRIGHT_EDIT))->SetWindowText(_T("Copyright © 2021 中日友好医院&国家呼吸医学中心 All Rights Reserved."));

	m_imgDVI.Init(m_nsw, m_nsh);
	m_imgSnap.Init(m_nsw, m_nsh);

	static_cast<CButton*>(GetDlgItem(IDC_START_BTN))->EnableWindow(TRUE);
	static_cast<CButton*>(GetDlgItem(IDC_STOP_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_CLEAR_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_SNAP_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_AIRWAYRADIUS_BTN))->EnableWindow(FALSE);

	CClientDC dc(this);
	m_VideoCaptureFactory.VideoCapture()->Init(&dc, 
		CSystemConfig::Instance()->GrabX, 
		CSystemConfig::Instance()->GrabY,
		CSystemConfig::Instance()->GrabW,
		CSystemConfig::Instance()->GrabH);

	//Counter();

	ShowWindow(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBUltrasonicExpansionDlg::Counter()
{
	FILE* fpCounter = nullptr;
	FILE* fpCounterInfo = nullptr;

	fopen_s(&fpCounter, "Counter.txt", "r");
	fopen_s(&fpCounterInfo, "CounterInfo.txt", "a+");

	int counter = 1;
	char buf[128] = { 0 };

	if (fpCounter != nullptr)
	{
		fseek(fpCounter, 0, SEEK_SET);
		fread(buf, 1, 128, fpCounter);
		counter = atoi(buf);

		++counter;
		fclose(fpCounter);
	}

	fopen_s(&fpCounter, "Counter.txt", "w");
	if (fpCounter != nullptr)
	{
		memset(buf, 0, 128);
		sprintf_s(buf, "%d", counter);
		fwrite(buf, 1, strlen(buf), fpCounter);
		fclose(fpCounter);
	}
	
	if (fpCounterInfo != nullptr)
	{
		memset(buf, 0, 128);

		SYSTEMTIME st;

		GetLocalTime(&st);
		sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d [%d]\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, counter);
		fwrite(buf, 1, strlen(buf), fpCounterInfo);
		fclose(fpCounterInfo);
	}
}

BOOL CBUltrasonicExpansionDlg::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_imgDVI.Uninit();
	m_imgSnap.Uninit();
	m_VideoCaptureFactory.VideoCapture()->UnInit();

	if (m_pRegPatientDlg != nullptr)
	{
		if (m_pRegPatientDlg->GetSafeHwnd() != NULL)
		{
			m_pRegPatientDlg->DestroyWindow();
		}
	}

	if (m_pPatientListDlg != nullptr)
	{
		if (m_pPatientListDlg->GetSafeHwnd() != NULL)
		{
			m_pPatientListDlg->DestroyWindow();
		}
	}

	if (m_pRectSelectTipsDlg != nullptr)
	{
		if (m_pRectSelectTipsDlg->GetSafeHwnd() != NULL)
		{
			m_pRectSelectTipsDlg->DestroyWindow();
		}
	}

	if (m_pRectScaleDlg != nullptr)
	{
		if (m_pRectScaleDlg->GetSafeHwnd() != NULL)
		{
			m_pRectScaleDlg->DestroyWindow();
		}
	}

	return CDialogEx::DestroyWindow();
}

void CBUltrasonicExpansionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBUltrasonicExpansionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBUltrasonicExpansionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBUltrasonicExpansionDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CBUltrasonicExpansionDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CDataPersistence::getInstance()->IsSave())
	{
		CDialogEx::OnCancel();
	}
	else
	{
		if (IDYES == AfxMessageBox(_T("没有登记患者信息，确定要退出？"), MB_YESNO | MB_SYSTEMMODAL | MB_ICONQUESTION))
		{
			CDialogEx::OnCancel();
		}
	}
}

void CBUltrasonicExpansionDlg::OnBnClickedStartBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	bool bGoOn = CDataPersistence::getInstance()->IsValid();

	if (!bGoOn)
	{
		bGoOn = (IDYES == AfxMessageBox(_T("尚未登记患者信息，确定要继续操作吗？"), MB_YESNO | MB_SYSTEMMODAL | MB_ICONQUESTION));
	}

	if (bGoOn)
	{
		if (m_imgDVI.Start())
		{
			Counter();

			static_cast<CButton*>(GetDlgItem(IDC_START_BTN))->EnableWindow(FALSE);
			static_cast<CButton*>(GetDlgItem(IDC_STOP_BTN))->EnableWindow(TRUE);
			static_cast<CButton*>(GetDlgItem(IDC_CLEAR_BTN))->EnableWindow(TRUE);
			static_cast<CButton*>(GetDlgItem(IDC_SNAP_BTN))->EnableWindow(TRUE);
			static_cast<CButton*>(GetDlgItem(IDC_AIRWAYRADIUS_BTN))->EnableWindow(TRUE);
			GetDlgItem(IDC_ADD_BTN)->EnableWindow(FALSE);
			m_bPlaying = TRUE;
		}
	}
}

void CBUltrasonicExpansionDlg::OnBnClickedStopBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	m_imgDVI.Stop();

	static_cast<CButton*>(GetDlgItem(IDC_START_BTN))->EnableWindow(TRUE);
	static_cast<CButton*>(GetDlgItem(IDC_STOP_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_CLEAR_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_SNAP_BTN))->EnableWindow(FALSE);
	static_cast<CButton*>(GetDlgItem(IDC_AIRWAYRADIUS_BTN))->EnableWindow(FALSE);
	GetDlgItem(IDC_ADD_BTN)->EnableWindow(TRUE);
	m_bPlaying = FALSE;
}


void CBUltrasonicExpansionDlg::OnBnClickedClearBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	m_imgDVI.Clear();
}

void CBUltrasonicExpansionDlg::OnBnClickedClearImgBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	m_imgSnap.Clear();
}

void CBUltrasonicExpansionDlg::OnBnClickedSnapBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	m_imgDVI.Snap();
	m_imgSnap.Snap();
}

void CBUltrasonicExpansionDlg::OnBnClickedAddBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	if (nullptr == m_pRegPatientDlg)
	{
		m_pRegPatientDlg = new CRegPatientDlg();

		if (m_pRegPatientDlg != nullptr)
		{
			m_pRegPatientDlg->Create(IDD_REGPATIENT_DLG, this);
		}
	}

	if (m_pRegPatientDlg != nullptr)
	{
		// 		m_pRegPatientDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		m_pRegPatientDlg->ClearDlg();
		m_pRegPatientDlg->CenterWindow(this);
		m_pRegPatientDlg->ShowWindow(SW_SHOW);
	}
}


void CBUltrasonicExpansionDlg::OnBnClickedQueryBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	if (nullptr == m_pPatientListDlg)
	{
		m_pPatientListDlg = new CPatientListDlg();
		if (m_pPatientListDlg != nullptr)
		{
			m_pPatientListDlg->Create(IDD_PATIENTLIST_DLG, this);
		}
	}

	if (m_pPatientListDlg != nullptr)
	{
		m_pPatientListDlg->ClearDlg(!m_bPlaying);
		m_pPatientListDlg->CenterWindow(this);
		m_pPatientListDlg->ShowWindow(SW_SHOW);
	}
}

void CBUltrasonicExpansionDlg::OnBnClickedOpenfolderBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	std::string strPath = CDataPersistence::getInstance()->m_strRootPath + "Archives\\";

	strPath = CDataPersistence::getInstance()->m_strRootPath + CDataPersistence::getInstance()->m_currentPatient.m_strPath;

	ShellExecute(NULL, _T("explore"), CString(strPath.c_str()), NULL, NULL, SW_SHOWNORMAL);
}

void CBUltrasonicExpansionDlg::OnBnClickedAirwayradiusBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	// 探出MessageBox提示开始选择影响有效区域
	m_nCalRadiusViewIdx = -1;

	m_imgDVI.ShowValidRect();
	m_imgSnap.ShowValidRect();

	if (nullptr == m_pRectSelectTipsDlg)
	{
		m_pRectSelectTipsDlg = new CImgRectSelectTipsDlg();

		if (m_pRectSelectTipsDlg != nullptr)
		{
			m_pRectSelectTipsDlg->Create(IDD_IMGRECTSELECTTIPS_DLG, this);
		}
	}

	if (m_pRectSelectTipsDlg != nullptr)
	{
		bool isEnableSkip = m_imgDVI.IsValidRectSelected() || m_imgSnap.IsValidRectSelected();
		m_pRectSelectTipsDlg->EnableSkip(isEnableSkip);
		m_pRectSelectTipsDlg->CenterWindow(this);
		m_pRectSelectTipsDlg->ShowWindow(SW_SHOW);
	}
}


void CBUltrasonicExpansionDlg::OnSelchangeRadiusAlgorithmCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	m_nRadiusAlgorithm = static_cast<CComboBox*>(GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO))->GetCurSel();
}


LRESULT CBUltrasonicExpansionDlg::OnViewDBClick(WPARAM wParam, LPARAM lParam)
{
	int viewSize = (int)lParam;
	int viewIdx = (int)wParam;

	if (VIEW_MAX_SIZE == viewSize)
	{
		m_nViewIdx = viewIdx;
		if (SNAP_IMG_VIEW == viewIdx)
		{
			ShowViewFullScreen(2);
		}
		else
		{
			ShowViewFullScreen(1);
		}
	}
	else
	{
		m_nViewIdx = -1;
		ShowViewFullScreen(0);
	}

	m_imgDVI.Invalidate();
	m_imgSnap.Invalidate();
	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnSnapImage(WPARAM wParam, LPARAM lParam)
{
	m_imgSnap.Rending();
	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnViewResize(WPARAM wParam, LPARAM lParam)
{
	int direction = (int)wParam;
	int distance = (int)lParam;

	CRect rcImg;
	CRect rcDvi;
	m_imgSnap.GetWindowRect(rcImg);
	m_imgDVI.GetWindowRect(rcDvi);
	int wImg = rcImg.Width();
	int hImg = rcImg.Height();
	int wDvi = rcDvi.Width();
	int hDvi = rcDvi.Height();

	if (1 == direction)
	{
		wImg = distance;
		hImg = (int)((rcImg.Width() + distance) * m_fImgRate) - rcImg.Height();

		int x = 0;
		int y = (m_rcClient.Height() - 40 - rcImg.Height() - hImg) / 2;
		m_imgSnap.SetWindowPos(&wndTop, x, y, rcImg.Width() + wImg, rcImg.Height() + hImg, SWP_SHOWWINDOW);
		wDvi = rcDvi.Width() - wImg;
		hDvi = rcDvi.Height() - hImg;
		x += rcImg.Width() + wImg;
		y = (m_rcClient.Height() - 40 - rcDvi.Height() + hImg) / 2;
		m_imgDVI.SetWindowPos(&wndTop, x, y, rcDvi.Width() - wImg, rcDvi.Height() - hImg, SWP_SHOWWINDOW);
		m_imgDVI.m_zoomPoint.x -= wImg;
		m_imgDVI.m_zoomPoint.y -= hImg;
	}
	else
	{
		wImg = distance;
		hImg = rcImg.Height() - (int)((rcImg.Width() - distance) * m_fImgRate);
		int x = 0;
		int y = (m_rcClient.Height() - 40 - rcImg.Height() + hImg) / 2;
		m_imgSnap.SetWindowPos(&wndTop, x, y, rcImg.Width() - wImg, rcImg.Height() - hImg, SWP_SHOWWINDOW);
		wDvi = rcDvi.Width() + wImg;
		hDvi = rcDvi.Height() + hImg;
		x += rcImg.Width() - wImg;
		y = (m_rcClient.Height() - 40 - rcDvi.Height() - hImg) / 2;
		m_imgDVI.SetWindowPos(&wndTop, x, y, rcDvi.Width() + wImg, rcDvi.Height() + hImg, SWP_SHOWWINDOW);
		m_imgDVI.m_zoomPoint.x += wImg;
		m_imgDVI.m_zoomPoint.y += hImg;
	}

	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnViewResizeStop(WPARAM wParam, LPARAM lParam)
{
	m_imgDVI.ForceReleaseRMbtn();
	m_imgSnap.ForceReleaseRMbtn();

	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnUpdatePatientInfo(WPARAM wParam, LPARAM lParam)
{
	std::string strInfo = "当前患者：" + CDataPersistence::getInstance()->m_currentPatient.m_strId + "  " + CDataPersistence::getInstance()->m_currentPatient.m_strName;
	CString info(strInfo.c_str());

	GetDlgItem(IDC_PATIENTINFO_STATIC)->SetWindowText(info);

	SetWindowText(_T("BUltrasonicExpansion ") + info);

	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnImgValidRectSelectFinish(WPARAM wParam, LPARAM lParam)
{
	int viewIdx = (int)wParam;

	m_nCalRadiusViewIdx = (int)wParam;
	if (SNAP_IMG_VIEW == m_nCalRadiusViewIdx)
	{
		m_imgDVI.CloseSelectValidRect();
	}
	else if (GRAB_DIV_VIEW == m_nCalRadiusViewIdx)
	{
		m_imgSnap.CloseSelectValidRect();
	}
	else
	{
		m_imgDVI.CloseSelectValidRect();
		m_imgSnap.CloseSelectValidRect();
	}

	if (nullptr == m_pRectScaleDlg)
	{
		m_pRectScaleDlg = new CImgRectScaleDlg();

		if (m_pRectScaleDlg != nullptr)
		{
			m_pRectScaleDlg->Create(IDD_IMGRECTSCALE_DLG, this);
		}
	}

	if (m_pRectScaleDlg != nullptr)
	{
		// 		m_pRegPatientDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		m_pRectScaleDlg->UpdateScaleData();
		m_pRectScaleDlg->CenterWindow(this);
		m_pRectScaleDlg->ShowWindow(SW_SHOW);
	}

	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnTriangleVertexesSelectFinish(WPARAM wParam, LPARAM lParam)
{
	// 计算气道半径，及标注圆心
	m_nCalRadiusViewIdx = (int)wParam;
	float radius = 0;
	int id = m_nRadiusAlgorithm;
// 	id = 3;
	if (SNAP_IMG_VIEW == m_nCalRadiusViewIdx)
	{
		m_imgSnap.CalculateAirwayRadius(id);
		radius = m_imgSnap.AirwayRadius();

		m_imgDVI.CloseSelectTriangleVertexes();
		m_imgDVI.CloseSelectValidRect();
	}
	else
	{
		m_imgDVI.CalculateAirwayRadius(id);
		radius = m_imgDVI.AirwayRadius();

		m_imgSnap.CloseSelectTriangleVertexes();
		m_imgSnap.CloseSelectValidRect();
	}

	CString strMsg = _T("");
	strMsg.Format(_T("气道半径：%.3f"), radius);
	AfxMessageBox(strMsg, MB_OK);
	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnImgRectSelectTipsDlgClose(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		m_imgDVI.StartSelectValidRect();
		m_imgSnap.StartSelectValidRect();
	}
	else if (wParam == 1)
	{
		OnImgValidRectSelectFinish(-1, 0);
	}
	else
	{

	}
	return 0;
}

LRESULT CBUltrasonicExpansionDlg::OnImgRectScaleDlgClose(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		if (SNAP_IMG_VIEW == m_nCalRadiusViewIdx)
		{
			m_imgSnap.SetImgRectScale(m_pRectScaleDlg->ScaleValue());
			m_imgSnap.StartSelectTriangleVertexes();
		}
		else if (GRAB_DIV_VIEW == m_nCalRadiusViewIdx)
		{
			m_imgDVI.SetImgRectScale(m_pRectScaleDlg->ScaleValue());
			m_imgDVI.StartSelectTriangleVertexes();
		}
		else
		{
// 			m_imgDVI.SetImgRectScale(m_pRectScaleDlg->ScaleValue());
// 			m_imgDVI.StartSelectTriangleVertexes();

			if (m_imgDVI.IsValidRectSelected())
			{
				m_imgDVI.SetImgRectScale(m_pRectScaleDlg->ScaleValue());
				m_imgDVI.StartSelectTriangleVertexes();
			}
			else
			{
				m_imgDVI.CloseSelectValidRect();
				m_imgDVI.CloseSelectTriangleVertexes();
			}

			if (m_imgSnap.IsValidRectSelected())
			{
				m_imgSnap.SetImgRectScale(m_pRectScaleDlg->ScaleValue());
				m_imgSnap.StartSelectTriangleVertexes();
			}
			else
			{
				m_imgSnap.CloseSelectValidRect();
				m_imgSnap.CloseSelectTriangleVertexes();
			}
		}
	}
	else
	{
		m_imgDVI.StartSelectValidRect();
		m_imgSnap.StartSelectValidRect();
	}
	return 0;
}



// helper
void CBUltrasonicExpansionDlg::ShowHideCtrl(BOOL bShow /* = TRUE */)
{
	GetDlgItem(IDC_START_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_STOP_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_SNAP_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_CLEAR_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_CLEAR_IMG_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_ADD_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_QUERY_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_PATIENTINFO_STATIC)->ShowWindow(bShow);
	GetDlgItem(IDC_OPENFOLDER_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_AIRWAYRADIUS_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_RADIUS_ALGORITHM_COMBO)->ShowWindow(bShow);
}

void CBUltrasonicExpansionDlg::ShowViewFullScreen(int fs /* = 0 */)
{
	if (0 == fs)
	{
		SetWindowLong(m_hWnd, GWL_STYLE, m_dwStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_dwExStyle);
		SetWindowPos(&wndTop, 0, 0, m_rcDesktop.Width(), m_rcDesktop.Height(), SWP_SHOWWINDOW);
		ShowHideCtrl(TRUE);

		int x = 0;
		int y = 0;
		int w = 0;

		w = m_nNrmViewW * 2;
		w = (m_rcClient.Width() - w) / 3;
		x = w;
		y = (m_rcClient.Height() - m_nNrmViewH - 150) / 2;

		m_imgDVI.FullScreen(FALSE, FALSE);
		m_imgSnap.FullScreen(FALSE, FALSE);
		m_imgSnap.SetWindowPos(&wndTop, x, y, m_nNrmViewW, m_nNrmViewH, SWP_SHOWWINDOW);
		x += (m_nNrmViewW + w);
		m_imgDVI.SetWindowPos(&wndTop, x, y, m_nNrmViewW, m_nNrmViewH, SWP_SHOWWINDOW);

		GetDlgItem(IDC_COPYRIGHT_EDIT)->ShowWindow(TRUE);
	}
	else
	{
		ShowHideCtrl(FALSE);
		DWORD dwStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		dwStyle &= m_dwStyle;
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

		DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
		dwExStyle &= m_dwExStyle;
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);

		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		if (1 == fs)
		{
			m_imgSnap.FullScreen(FALSE, FALSE);
			m_imgDVI.FullScreen(TRUE, m_imgDVI.hasSnap());
			m_imgSnap.ShowWindow(FALSE);
			m_imgDVI.SetWindowPos(&wndTop, 0, 0, m_nsw, m_nsh, SWP_SHOWWINDOW);
		}
		else
		{
			m_imgDVI.FullScreen(FALSE, FALSE);
			m_imgSnap.FullScreen(TRUE, m_imgDVI.isGrabbing());
			m_imgDVI.ShowWindow(FALSE);
			m_imgSnap.SetWindowPos(&wndTop, 0, 0, m_nsw, m_nsh, SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_COPYRIGHT_EDIT)->ShowWindow(FALSE);
	}

	this->BringWindowToTop();
	this->SetFocus();
}



