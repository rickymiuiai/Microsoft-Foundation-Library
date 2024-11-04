// ImgRectSelectTipsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "ImgRectSelectTipsDlg.h"
#include "afxdialogex.h"
#include "Common.h"


// CImgRectSelectTipsDlg 对话框

IMPLEMENT_DYNAMIC(CImgRectSelectTipsDlg, CDialogEx)

CImgRectSelectTipsDlg::CImgRectSelectTipsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgRectSelectTipsDlg::IDD, pParent)
	, m_IsEnableSkipBtn(false)
{

}

CImgRectSelectTipsDlg::~CImgRectSelectTipsDlg()
{
}

void CImgRectSelectTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImgRectSelectTipsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SKIP_BTN, &CImgRectSelectTipsDlg::OnBnClickedSkipBtn)
	ON_BN_CLICKED(IDOK, &CImgRectSelectTipsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImgRectSelectTipsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CImgRectSelectTipsDlg 消息处理程序


void CImgRectSelectTipsDlg::OnBnClickedSkipBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SELECT_TIPS_CLOSE, 1, 0);
	EndDialog(IDC_SKIP_BTN);
}


void CImgRectSelectTipsDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SELECT_TIPS_CLOSE, 0, 0);
	CDialogEx::OnOK();
}


void CImgRectSelectTipsDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SELECT_TIPS_CLOSE, 2, 0);
	CDialogEx::OnCancel();
}

void CImgRectSelectTipsDlg::EnableSkip(bool isEnable)
{
	m_IsEnableSkipBtn = isEnable;
	GetDlgItem(IDC_SKIP_BTN)->EnableWindow(m_IsEnableSkipBtn);
}


BOOL CImgRectSelectTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_SKIP_BTN)->EnableWindow(m_IsEnableSkipBtn);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
