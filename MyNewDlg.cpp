// MyNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "videodehaze.h"
#include "MyNewDlg.h"
#include "afxdialogex.h"


// CMyNewDlg dialog

IMPLEMENT_DYNAMIC(CMyNewDlg, CDialogEx)

CMyNewDlg::CMyNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyNewDlg::IDD, pParent)
	, m_FileName(_T(""))
	, FileName(_T(""))
{

}

CMyNewDlg::~CMyNewDlg()
{
}

void CMyNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Enter, m_FileName);
}


BEGIN_MESSAGE_MAP(CMyNewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Fine, &CMyNewDlg::OnBnClickedFine)
END_MESSAGE_MAP()


// CMyNewDlg message handlers



void CMyNewDlg::OnBnClickedFine()
{
	// TODO: Add your control notification handler code here
	
	GetDlgItemText(IDC_Enter, FileName);
	EndDialog(0);
}
