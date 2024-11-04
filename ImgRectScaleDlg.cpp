// ImgRectScaleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "ImgRectScaleDlg.h"
#include "afxdialogex.h"
#include "Common.h"


// CImgRectScaleDlg 对话框

IMPLEMENT_DYNAMIC(CImgRectScaleDlg, CDialogEx)

CImgRectScaleDlg::CImgRectScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgRectScaleDlg::IDD, pParent)
	, m_Scale(1)
{

}

CImgRectScaleDlg::~CImgRectScaleDlg()
{
}

void CImgRectScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SCALE_EDIT, m_Scale);
}


BEGIN_MESSAGE_MAP(CImgRectScaleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImgRectScaleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImgRectScaleDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CImgRectScaleDlg 消息处理程序


void CImgRectScaleDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SCALE_CLOSE, 0, 0);
	CDialogEx::OnOK();
}


void CImgRectScaleDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SCALE_CLOSE, 1, 0);
	CDialogEx::OnCancel();
}

float CImgRectScaleDlg::ScaleValue()
{
	return m_Scale;
}

void CImgRectScaleDlg::UpdateScaleData()
{
	UpdateData(FALSE);
}


BOOL CImgRectScaleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
// 	((CEdit*)GetDlgItem(IDC_SCALE_EDIT))->SetWindowText(_T("10"));
// 	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
