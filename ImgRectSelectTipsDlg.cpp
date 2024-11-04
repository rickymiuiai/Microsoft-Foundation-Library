// ImgRectSelectTipsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "ImgRectSelectTipsDlg.h"
#include "afxdialogex.h"
#include "Common.h"


// CImgRectSelectTipsDlg �Ի���

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


// CImgRectSelectTipsDlg ��Ϣ�������


void CImgRectSelectTipsDlg::OnBnClickedSkipBtn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SELECT_TIPS_CLOSE, 1, 0);
	EndDialog(IDC_SKIP_BTN);
}


void CImgRectSelectTipsDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SELECT_TIPS_CLOSE, 0, 0);
	CDialogEx::OnOK();
}


void CImgRectSelectTipsDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_SKIP_BTN)->EnableWindow(m_IsEnableSkipBtn);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
