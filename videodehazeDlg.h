
// videodehazeDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "CvvImage.h"
#include "afxcmn.h"
#include "MyNewDlg.h "

// CvideodehazeDlg dialog
class CvideodehazeDlg : public CDialogEx
{
// Construction
public:
	CvideodehazeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIDEODEHAZE_DIALOG };
	int m_Sum;
	int m_Present;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_HazePicture;
	CStatic m_DehazePicture;
	afx_msg void OnBnClickedOpenVideo();
	IplImage *TheImage;
	CButton m_Open;
	void ShowImage(IplImage* img, UINT ID);
	afx_msg void OnBnClickedPause();
	bool m_run;
	CString vPath;
	CString SumFrames;
	CString PresentFrames;
	afx_msg void OnBnClickedHandle();
	afx_msg void OnBnClickedCompare();
	afx_msg void OnBnClickedSave();
	
	CSliderCtrl m_Slider;
	CSliderCtrl m_Radius;
	CSliderCtrl m_MaxAtom;
	CSliderCtrl m_GuideRadius;
	CSliderCtrl m_Epilson;
	CSliderCtrl m_T;
	void dehaze(void);
	afx_msg void OnBnClickedImage();
	afx_msg void OnBnClickedVideo();
	afx_msg void OnBnClickedOpen2();
	afx_msg void OnBnClickedHandle2();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedReset();
	int m_Reset;
	IplImage* TheImage2;
	void Hist_Equalize(IplImage * Image, IplImage * ImageOut);
	CSliderCtrl m_Weight;
	int m_Save;
	afx_msg void OnBnClickedExit();
	void HE_Color(IplImage * pImage, IplImage * pEImage);
	void enhance(IplImage * Image, IplImage * ImageOut);
	int m_Choose;
};
