#pragma once
#include "afxwin.h"


// CMyNewDlg dialog

class CMyNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyNewDlg)

public:
	CMyNewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyNewDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_Enter;
	afx_msg void OnBnClickedFine();

	CString m_FileName;
	CString FileName;
};
