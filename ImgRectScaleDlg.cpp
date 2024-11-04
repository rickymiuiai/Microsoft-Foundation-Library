// ImgRectScaleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BUltrasonicExpansion.h"
#include "ImgRectScaleDlg.h"
#include "afxdialogex.h"
#include "Common.h"


// CImgRectScaleDlg �Ի���

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


// CImgRectScaleDlg ��Ϣ�������


void CImgRectScaleDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_IMGRECT_SCALE_CLOSE, 0, 0);
	CDialogEx::OnOK();
}


void CImgRectScaleDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
// 	((CEdit*)GetDlgItem(IDC_SCALE_EDIT))->SetWindowText(_T("10"));
// 	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
