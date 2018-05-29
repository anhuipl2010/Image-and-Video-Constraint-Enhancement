
// videodehazeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "videodehaze.h"
#include "videodehazeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CvideodehazeDlg dialog

CvideodehazeDlg::CvideodehazeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CvideodehazeDlg::IDD, pParent)
	, TheImage(NULL)
	, m_Sum(0)
	, m_Present(0)
	, m_Reset(0)
	, TheImage2(NULL)
	, m_Save(0)
	, m_Choose(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvideodehazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Haze, m_HazePicture);
	DDX_Control(pDX, IDC_Dehaze, m_DehazePicture);
	DDX_Control(pDX, IDC_Open, m_Open);
	DDX_Text(pDX, IDC_SumFrame, m_Sum);
	DDX_Text(pDX, IDC_PresentFrame, m_Present);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_Block, m_Radius);
	DDX_Control(pDX, IDC_maxatom, m_MaxAtom);
	DDX_Control(pDX, IDC_GuideRadius, m_GuideRadius);
	DDX_Control(pDX, IDC_Weight, m_Weight);
}

BEGIN_MESSAGE_MAP(CvideodehazeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Open, &CvideodehazeDlg::OnBnClickedOpenVideo)
	ON_BN_CLICKED(IDC_Pause, &CvideodehazeDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_Handle, &CvideodehazeDlg::OnBnClickedHandle)
	ON_BN_CLICKED(IDC_Compare, &CvideodehazeDlg::OnBnClickedCompare)
	ON_BN_CLICKED(IDC_Save, &CvideodehazeDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Image, &CvideodehazeDlg::OnBnClickedImage)
	ON_BN_CLICKED(IDC_Video, &CvideodehazeDlg::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_Open2, &CvideodehazeDlg::OnBnClickedOpen2)
	ON_BN_CLICKED(IDC_Handle2, &CvideodehazeDlg::OnBnClickedHandle2)
	ON_BN_CLICKED(IDC_Save2, &CvideodehazeDlg::OnBnClickedSave2)
	ON_BN_CLICKED(IDC_Reset, &CvideodehazeDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_Exit, &CvideodehazeDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CvideodehazeDlg message handlers

BOOL CvideodehazeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CDC* pDC=GetDlgItem(IDC_Haze)->GetDC ();
	HDC hDC=pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_Haze)->GetClientRect (&rect);

	int rw=rect.right-rect.left ;
	int rh=rect.bottom -rect.top ;

	CvSize ImgSize;
	ImgSize.height = rh ;
	ImgSize.width = rw ;
	TheImage=cvCreateImage(ImgSize,IPL_DEPTH_8U,IMAGE_CHANNELS);
	TheImage2=cvCreateImage(ImgSize,IPL_DEPTH_8U,IMAGE_CHANNELS);
	ReleaseDC(pDC);

	//�ɵ��ڲ�������
	m_Radius.SetRange(0,200);
	m_Radius.SetPos(10);
	m_MaxAtom.SetRange(0,200);
	m_MaxAtom.SetPos(10);
	m_GuideRadius.SetRange(0,200);
	m_GuideRadius.SetPos(0);
	m_Weight.SetRange(0,100);
	m_Weight.SetPos (0);

	//�ؼ�ʹ��
	GetDlgItem( IDC_Handle )->EnableWindow( FALSE );
	GetDlgItem( IDC_Save )->EnableWindow( FALSE );
	GetDlgItem( IDC_Pause )->EnableWindow( FALSE );
	GetDlgItem( IDC_Compare )->EnableWindow( FALSE );
	GetDlgItem( IDC_Block )->EnableWindow( FALSE );
	GetDlgItem( IDC_maxatom )->EnableWindow( FALSE );
	GetDlgItem( IDC_GuideRadius )->EnableWindow( FALSE );
	GetDlgItem( IDC_SLIDER1 )->EnableWindow( FALSE );
	GetDlgItem( IDC_SumFrame )->EnableWindow( FALSE );
	GetDlgItem( IDC_PresentFrame )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_Save2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Handle2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Reset )->EnableWindow( FALSE );
	GetDlgItem( IDC_Weight )->EnableWindow( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CvideodehazeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CvideodehazeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::OnPaint ();
		CDialog::UpdateWindow ();
		ShowImage(TheImage2,IDC_Dehaze);
	    ShowImage(TheImage2,IDC_Haze);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CvideodehazeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//��������
int Width,Height,frames,frames2 ;
float Lambda=1,Gama=1,Beta=0,a,b,Weight=0;
int numfrm = 0;
CvCapture* g_capture1 = NULL;
CvCapture* g_capture2 = NULL;
IplImage* ipl,* ipl1;
double fps;
CvSize size;
CvVideoWriter* enhanceVideo;
CString sPath;
int sumI20=0,sumI21=0,sumI22=0,Lnum=0;
int LLast0=0, LLast1=0, LLast2=0;
float last0[256],last1[256],last2[256];

//����������
void onTrackbarSlide1( int pos1 )  
{  
 cvSetCaptureProperty( g_capture1, CV_CAP_PROP_POS_FRAMES, pos1 );  
} 

//����������Ƶ
void CvideodehazeDlg::OnBnClickedOpenVideo()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );

	CString szFileFilter=_T("media files(*.wmv,*.mp3,*.avi,*.rm,*.rmvb,*.mkv,*.mp4,*.flv)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4;*.flv|");   //����Ƶ����
    CFileDialog dlg(true,NULL,NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,szFileFilter,NULL);  //�ļ��򿪺�������

	if( dlg.DoModal() != IDOK )					
		return;
	vPath = dlg.GetPathName();  //��������Ƶ·��
	if( vPath == "" ){  //�жϻ�ȡ·���Ƿ�ɹ������ɹ�����
		MessageBox("��ѡ����Ƶ");
		GetDlgItem( IDC_Open )->EnableWindow( TRUE );
		return;
	}
	
	g_capture1=cvCreateFileCapture(vPath);  //��ȡ��Ƶ��Ϣ
	if(!(g_capture1)){  //�жϻ�ȡ��Ƶ��Ϣ�Ƿ�ɹ������ɹ�����
		MessageBox("����Ƶ�ļ�ʧ��");
		return;
	 }

	frames = (int) cvGetCaptureProperty( g_capture1, CV_CAP_PROP_FRAME_COUNT ); //�����Ƶ֡��
	fps = (int) cvGetCaptureProperty( g_capture1, CV_CAP_PROP_FPS );  //�����Ƶÿ�봫��֡��
	size = cvSize(  //�����Ƶ���
		(int)cvGetCaptureProperty(g_capture1, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(g_capture1, CV_CAP_PROP_FRAME_HEIGHT));

	UpdateData();
	m_Sum=frames;  //��ʾ��Ƶ��֡��
	UpdateData(false);

	m_Slider.SetRange(0,frames);  //��������Χ
	m_Slider.SetPos(0);  //���ý�������ʼ״̬

	GetDlgItem( IDC_Save )->EnableWindow( TRUE );
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_Handle )->EnableWindow( FALSE );
	GetDlgItem( IDC_Pause )->EnableWindow( FALSE );
	GetDlgItem( IDC_Compare )->EnableWindow( FALSE );
}

//��������ʾͼ��
void CvideodehazeDlg::ShowImage(IplImage * img, UINT ID)
{
	
	CDC* pDC=GetDlgItem(ID)->GetDC ();
	HDC hDC=pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect (&rect);//�����ʾ���ھ��

	int rw=rect.right-rect.left ;
	int rh=rect.bottom -rect.top ;

	int w=img->width ;
	int h=img->height ;
	
	float scaleW=(float)((float)w/rw);
	float scaleH=(float)((float)h/rh);
	float max=(scaleW>scaleH)?scaleW:scaleH;

	int nw=(int)(w/max);
	int nh=(int)(h/max);

	int tlx=(nw>nh)?0:(int)(rw-nw)/2;
	int tly=(nw>nh)?(int)(rh-nh)/2:0;
	
	cvSetImageROI(TheImage,cvRect(tlx,tly,nw,nh));  //����ͼ��ROIΪ��ʾ���ڴ�С
	cvResize(img,TheImage);  //��������ͼ���С

	int iw=TheImage->width ;
	int ih=TheImage->height ;

	int tx=(int)(rw-iw)/2;
	int ty=(int)(rh-ih)/2;
	SetRect(rect,tx,ty,tx+iw,ty+ih);

	CvvImage cimg;
	cimg.CopyOf(TheImage);

	cimg.DrawToHDC(hDC,&rect);  //����ͼ�񵽴���
	cvResetImageROI(TheImage);
	cimg.Destroy ();
	ReleaseDC(pDC);//�ͷž��
}

//��������ͣ/��ʼ
void CvideodehazeDlg::OnBnClickedPause()
{
	// TODO: Add your control notification handler code here
	if(m_run)m_run=0;  //m_runΪ���Ʋ�����Ϊ0ʱ��ͣ��Ϊ1ʱ��ʼ
	else m_run=1;
}

//��������Ƶ����
void CvideodehazeDlg::OnBnClickedHandle()
{
	// TODO: Add your control notification handler code here
	IplImage *pFrame = NULL;  //����ǰ��Ƶ֡
	IplImage *pFrame1 = NULL;  //����ǰ����Ƶ֡
	int nPos=0;  //������λ��
	numfrm = 0; //��Ƶ��ǰ֡��

	GetDlgItem( IDC_Pause )->EnableWindow( TRUE );
	GetDlgItem( IDC_Handle )->EnableWindow( FALSE );
	GetDlgItem( IDC_SLIDER1 )->EnableWindow( TRUE );

	enhanceVideo = cvCreateVideoWriter(sPath,CV_FOURCC('X','V','I','D'), fps, size);//������Ƶд����                   //'M','J','P','G'
	if( TheImage )cvZero( TheImage );
	m_run = 1;
	pFrame = cvQueryFrame( g_capture1 );//��ȡ��Ƶ��һ֡
	
	numfrm++;  //��ǰ����֡����1
	Width=pFrame->width ;
	Height=pFrame->height ;
	ShowImage( pFrame, IDC_Haze );  //��ʾ����ǰ��Ƶ���󴰿�

	if(TheImage) cvZero(TheImage);
	pFrame1 = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, IMAGE_CHANNELS);

	Lambda=float(m_Radius.GetPos())/10;  //��ÿɱ������ǰֵ
	Gama=float(m_MaxAtom.GetPos())/10;
	Beta=float(m_GuideRadius.GetPos())/20;
	    
		CString Rstr;
        Rstr.Format("%0.2f",Lambda);
		SetDlgItemText(IDC_B,Rstr);
		CString Mstr;
        Mstr.Format("%0.2f",Gama);
		SetDlgItemText(IDC_M,Mstr);
		CString Gstr;
        Gstr.Format("%0.2f",Beta);
		SetDlgItemText(IDC_G,Gstr);

		Weight=float(m_Weight.GetPos())/100;
        CString Wstr;
        Wstr.Format("%0.2f",Weight);
        SetDlgItemText(IDC_W,Wstr);

	HE_Color(pFrame, pFrame1);  //������Ƶ
	ShowImage( pFrame1, IDC_Dehaze );  //��ʾ�������Ƶ���Ҵ���
	cvWriteFrame(enhanceVideo,pFrame1 );  //д�봦�����Ƶ

	if(TheImage) cvZero(TheImage);
	
	UpdateData();
	m_Present++;  //��ʾ��ǰ����֡��
	UpdateData(false);
	nPos++;  //������λ�ü�1
	m_Slider.SetPos ( nPos );  //���õ�ǰλ�ý�����λ��

    while( m_Reset ){ //���������ѭ��
		if (numfrm>=frames)break;  //����������֡����ѭ��
		if(m_run == 1){  //m_runΪ1����m_runΪ0��ͣ
		int cPos=m_Slider.GetPos ();  //��õ�ǰ������λ��
		if(cPos!=nPos){  //������֡������������λ�ô�
		    cvSetCaptureProperty( g_capture1,CV_CAP_PROP_POS_FRAMES, double(cPos));
			m_Present=cPos;
			numfrm=cPos;
			nPos=cPos;
		}

		pFrame = cvQueryFrame( g_capture1 ); //��ȡ��Ƶ֡
		numfrm++;   //��ǰ����֡����1
        if (pFrame == NULL) break;
		ShowImage( pFrame, IDC_Haze );  //��ʾ����ǰ��Ƶ���󴰿�
		if(TheImage) cvZero(TheImage);
		
	    Lambda=float(m_Radius.GetPos())/10;  //��ÿɱ������ǰֵ
	    Gama=float(m_MaxAtom.GetPos())/10;
	    Beta=float(m_GuideRadius.GetPos())/20;
	    
		CString Rstr;
        Rstr.Format("%0.2f",Lambda);
		SetDlgItemText(IDC_B,Rstr);
		CString Mstr;
        Mstr.Format("%0.2f",Gama);
		SetDlgItemText(IDC_M,Mstr);
		CString Gstr;
        Gstr.Format("%0.2f",Beta);
		SetDlgItemText(IDC_G,Gstr);

		Weight=float(m_Weight.GetPos())/100;
        CString Wstr;
        Wstr.Format("%0.2f",Weight);
        SetDlgItemText(IDC_W,Wstr);

		HE_Color(pFrame, pFrame1);   //������Ƶ
		ShowImage( pFrame1, IDC_Dehaze );  //��ʾ�������Ƶ���Ҵ���
		cvWriteFrame(enhanceVideo, pFrame1 );   //д�봦�����Ƶ
		
		if(TheImage) cvZero(TheImage);
			
		UpdateData();
		m_Present++;  //��ʾ��ǰ����֡��
		UpdateData(false);
		nPos++;  //������λ�ü�1
		m_Slider.SetPos ( nPos );  //���õ�ǰλ�ý�����λ��

		}
		MSG msg; // ��Ϣ�ṹ��
		if (::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){        // ��Ϣ�ṹ��        
			::TranslateMessage (&msg) ;         
			::DispatchMessage (&msg) ;     
		} 
	}
	m_run = 0;
	m_Slider.SetPos(0);
	    
	cvReleaseImage( &pFrame1 );  //�ͷű���
    cvReleaseVideoWriter( &enhanceVideo);
	cvReleaseCapture(&g_capture1);

	if(m_Reset==0)return;

	sumI20=0;
	sumI21=0;
	sumI22=0;
	Lnum=0;

	g_capture1=cvCreateFileCapture(vPath);//����ǰ��Ƶ��Ϣ
	g_capture2=cvCreateFileCapture(sPath);//�������Ƶ��Ϣ
	frames2 = (int) cvGetCaptureProperty( g_capture2, CV_CAP_PROP_FRAME_COUNT );//�������Ƶ֡��
	if(frames2<frames)frames=frames2; //���洦��ǰ���С��֡��
	UpdateData();
	m_Sum=frames;
	m_Present=0;
	UpdateData(false);

	GetDlgItem( IDC_Compare )->EnableWindow( TRUE );
	GetDlgItem( IDC_Pause )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_Block )->EnableWindow( FALSE );
	GetDlgItem( IDC_maxatom )->EnableWindow( FALSE );
	GetDlgItem( IDC_GuideRadius )->EnableWindow( FALSE );
	GetDlgItem( IDC_Weight )->EnableWindow( FALSE );
	GetDlgItem( IDC_SLIDER1 )->EnableWindow( FALSE );
}

//��������Ƶ����ǰ��Ա�
void CvideodehazeDlg::OnBnClickedCompare()
{
	// TODO: Add your control notification handler code here 
	IplImage *pFrame = NULL;  //����ǰ��Ƶ֡
	IplImage *pFrame1 = NULL;  //�������Ƶ֡
	if(!(g_capture2)){
		MessageBox("��Ƶ����ʧ�ܣ��޷����бȽ�");
		return;
	 }
	m_Slider.SetRange(0,frames);  //���ý�������֡��
	m_Slider.SetPos(0);  //���ý�������ʼλ��
	UpdateData();
	m_Present=0;  //��ʾ��ǰ����֡��
	UpdateData(false);

	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_SLIDER1 )->EnableWindow( TRUE );
	GetDlgItem( IDC_Pause )->EnableWindow( TRUE );
	GetDlgItem( IDC_Compare )->EnableWindow( FALSE );
	
	int nPos=0;
	int numfrm = 0;
	m_run = 1;

    while( m_Reset ){   //���������ѭ��
		if(numfrm >=frames)break;  //����������֡����ѭ��
	    if(m_run == 1){  //m_runΪ1����m_runΪ0��ͣ
		int cPos=m_Slider.GetPos ();   //��õ�ǰ������λ��
		if(cPos!=nPos){  //������֡������������λ�ô�
		    cvSetCaptureProperty( g_capture1,CV_CAP_PROP_POS_FRAMES, double(cPos));
			cvSetCaptureProperty( g_capture2,CV_CAP_PROP_POS_FRAMES, double(cPos));
			m_Present=cPos;
			numfrm=cPos;
			nPos=cPos;
		}
		pFrame = cvQueryFrame( g_capture1 ); //��ȡ����ǰ��Ƶ֡
		if (pFrame==NULL) break;
		ShowImage( pFrame, IDC_Haze );//��ʾ����ǰ��Ƶ���󴰿�
		if(TheImage) cvZero(TheImage);	
		pFrame1 = cvQueryFrame( g_capture2 );  //��ȡ�������Ƶ֡
		if (pFrame1==NULL) break;
	    ShowImage(pFrame1,IDC_Dehaze);//��ʾ�������Ƶ���Ҵ���
		if(TheImage) cvZero(TheImage);	
		Sleep(1000/fps);  //���Ʋ�������

		numfrm++;
		nPos++;
		m_Slider.SetPos ( nPos ); //���õ�ǰ���������λ��

		UpdateData();
		m_Present=numfrm;
		UpdateData(false);
		}

		MSG msg;// ��Ϣ�ṹ��
		if (::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){        // ��Ϣ�ṹ��   
			::TranslateMessage (&msg) ;         
			::DispatchMessage (&msg) ;     
		}
	}

	m_run = 0;
	m_Slider.SetPos(0);
	UpdateData();
	m_Present=0;
	m_Sum=0;
	UpdateData(false);
	cvReleaseCapture(&g_capture1); //�ͷű���
	cvReleaseCapture(&g_capture2);

	GetDlgItem( IDC_Video )->EnableWindow( TRUE );
	GetDlgItem( IDC_Pause )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_SLIDER1 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Image )->EnableWindow( TRUE );
}

//������ѡ�񱣴洦�����Ƶ��ַ
void CvideodehazeDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CString sFilePath;
	GetDlgItem( IDC_Save )->EnableWindow( FALSE );

	BROWSEINFO save;  //���ñ����ļ��жԻ���
    TCHAR Buffer[MAX_PATH];   
    save.hwndOwner = NULL;  
    save.pidlRoot =NULL;
    save.pszDisplayName = Buffer;
    save.lpszTitle = "ѡ�񱣴�·��";  
    save.ulFlags = BIF_EDITBOX;  
  
    save.lpfn = NULL;  
    save.iImage=IDR_MAINFRAME;  
    LPITEMIDLIST pIDList = SHBrowseForFolder(&save);
    if(pIDList){  //���뱣��·��
        SHGetPathFromIDList(pIDList, Buffer);          
        sFilePath = Buffer; 
    }

	if(sFilePath==""){
	::MessageBox(NULL,CString("��ѡ��·��") , NULL, MB_OK);
	GetDlgItem( IDC_Save )->EnableWindow( TRUE );
	return;
	}

	CMyNewDlg NewDlg;  //�½��Ի���
    NewDlg.DoModal();
	CString Path;
	Path = NewDlg.FileName;  //��ñ���·��
	if(Path==""){
	::MessageBox(NULL,CString("��������Ƶ������") , NULL, MB_OK);
	GetDlgItem( IDC_Save )->EnableWindow( TRUE );
	return;
	}
	int Length=sFilePath.GetLength ();
	for(int i=0;i<Length;++i){  //���ı���·��Ϊ��ȷ��ʽ
		if(sFilePath.GetAt(i)=='\\'){
			sFilePath.SetAt(i,'/');
		}
	}
	if(sFilePath.GetAt(Length-1)=='/')sPath=sFilePath + Path+".avi";
	else sPath=sFilePath +'/'+ Path+".avi";
	GetDlgItem( IDC_Handle )->EnableWindow( TRUE );
	GetDlgItem( IDC_Block )->EnableWindow( TRUE );
	GetDlgItem( IDC_maxatom )->EnableWindow( TRUE );
	GetDlgItem( IDC_GuideRadius )->EnableWindow( TRUE );
	GetDlgItem( IDC_Weight )->EnableWindow( TRUE );
}

//������ѡ����ͼ��
void CvideodehazeDlg::OnBnClickedImage()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Open2 )->EnableWindow( TRUE );
	GetDlgItem( IDC_Image )->EnableWindow( FALSE );
	GetDlgItem( IDC_Video )->EnableWindow( FALSE );
	GetDlgItem( IDC_Compare )->EnableWindow( FALSE );
	GetDlgItem( IDC_Reset )->EnableWindow( TRUE );
	m_Reset=1;
	m_Choose=0;
}

//������ѡ������Ƶ
void CvideodehazeDlg::OnBnClickedVideo()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Open )->EnableWindow( TRUE );
	GetDlgItem( IDC_Image )->EnableWindow( FALSE );
	GetDlgItem( IDC_Video )->EnableWindow( FALSE );
	GetDlgItem( IDC_Save2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Reset )->EnableWindow( TRUE );
	m_Reset=1;
	m_Choose=1;
}

//��������ͼ��
void CvideodehazeDlg::OnBnClickedOpen2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Open2 )->EnableWindow( FALSE );
	m_Weight.SetPos (100);

	CString szFileFilter=_T("image files (*.jpg;*.bmp;*.gif;*.tif;*.png)|*.jpg;*.bmp;*.gif;*.tif;*.png|");  //��ͼ������
    CFileDialog dlg(true,NULL,NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,szFileFilter,NULL);  //�ļ��򿪺�������

	if(dlg.DoModal ()!=IDOK) return;
	CString mPath=dlg.GetPathName (); //���ͼ��·��
	if( mPath == "" ) { //�жϻ�ȡ·���Ƿ�ɹ������ɹ�����
		MessageBox("��ѡ��ͼ��");
		GetDlgItem( IDC_Open2 )->EnableWindow( TRUE );
		return;
	}
	ipl = cvLoadImage(mPath, 1);  //��ȡͼ��
	if(!(ipl)){
			MessageBox("��ͼ���ļ�ʧ��"); //�жϻ�ȡ��Ƶ��Ϣ�Ƿ�ɹ������ɹ�����
			GetDlgItem( IDC_Open2 )->EnableWindow( TRUE );
			return;
	}

	Width=ipl->width;//��ȡͼ����
    Height=ipl->height;
	if(!ipl)return;
	ShowImage(ipl,IDC_Haze);  //��ʾ����ǰͼ��
	if(TheImage) cvZero(TheImage);
	
	GetDlgItem( IDC_Handle2 )->EnableWindow( TRUE );
	GetDlgItem( IDC_Weight )->EnableWindow( TRUE );
}

//����������ͼ��
void CvideodehazeDlg::OnBnClickedHandle2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Save2 )->EnableWindow( TRUE );
	GetDlgItem( IDC_Handle2 )->EnableWindow( FALSE );
	
	Weight=float(m_Weight.GetPos())/100;  //��ÿɱ������ǰֵ
    CString Wstr;
    Wstr.Format("%0.2f",Weight);
    SetDlgItemText(IDC_W,Wstr);

	ipl1 = cvCreateImage(cvSize(Width, Height), IPL_DEPTH_8U, IMAGE_CHANNELS);

	HE_Color(ipl, ipl1);  //����ͼ��
	ShowImage( ipl1, IDC_Dehaze );  //��ʾ�����ͼ��
	
	m_Save = 1;

	while(m_Save){  //m_Save=0�����桢���á��˳������ѭ��
		Weight=float(m_Weight.GetPos())/100; //��ÿɱ������ǰֵ
        CString Wstr;
        Wstr.Format("%0.2f",Weight);
        SetDlgItemText(IDC_W,Wstr);
	
		HE_Color(ipl, ipl1);  //����ͼ��
		ShowImage( ipl1, IDC_Dehaze );  //��ʾ�����ͼ��

	    MSG msg;  // ��Ϣ�ṹ��
		if (::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){         // ��Ϣ�ṹ��   
			::TranslateMessage (&msg) ;         
			::DispatchMessage (&msg) ;     
		}
		
	}
	if(TheImage) cvZero(TheImage);
	cvWaitKey(0);
     	
	cvReleaseImage(&ipl); //�ͷű���

    GetDlgItem( IDC_Weight )->EnableWindow( FALSE );
}

//����������ͼ��
void CvideodehazeDlg::OnBnClickedSave2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Save2 )->EnableWindow( FALSE );
	m_Save = 0;
	CString sFilePath;
	CString sPath2;
	BROWSEINFO save;   //���ñ����ļ��жԻ���
    TCHAR Buffer[MAX_PATH];   
    save.hwndOwner = NULL;  
    save.pidlRoot =NULL;
    save.pszDisplayName = Buffer;
    save.lpszTitle = "ѡ�񱣴�·��";  
    save.ulFlags = BIF_EDITBOX;  
  
    save.lpfn = NULL;  
    save.iImage=IDR_MAINFRAME;  
    LPITEMIDLIST pIDList = SHBrowseForFolder(&save);
    if(pIDList)   //���뱣��·��
    {  
        SHGetPathFromIDList(pIDList, Buffer);          
        sFilePath = Buffer;
    }
	if(sFilePath==""){
	::MessageBox(NULL,CString("��ѡ��·��") , NULL, MB_OK);
	GetDlgItem( IDC_Save2 )->EnableWindow( TRUE );
	return;
	}

	CMyNewDlg NewDlg;
    NewDlg.DoModal();
	CString Path;
	Path = NewDlg.FileName;   //��ñ���·��
	
	if(Path==""){
	::MessageBox(NULL,CString("������ͼ�񱣴���") , NULL, MB_OK);
	GetDlgItem( IDC_Save2 )->EnableWindow( TRUE );
	return;
	}

	int Length=sFilePath.GetLength ();
	for(int i=0;i<Length;++i){   //���ı���·��Ϊ��ȷ��ʽ
		if(sFilePath.GetAt(i)=='\\'){
			sFilePath.SetAt(i,'/');
		}
	}
	if(sFilePath.GetAt(Length-1)=='/')sPath2=sFilePath + Path+".jpg";
	else sPath2=sFilePath +'/'+ Path+".jpg";

    cvSaveImage(sPath2,ipl1 );
	cvReleaseImage(&ipl1);
	GetDlgItem( IDC_Video )->EnableWindow( TRUE );
	GetDlgItem( IDC_Image )->EnableWindow( TRUE );
}

//����������
void CvideodehazeDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_Handle )->EnableWindow( FALSE );
	GetDlgItem( IDC_Save )->EnableWindow( FALSE );
	GetDlgItem( IDC_Pause )->EnableWindow( FALSE );
	GetDlgItem( IDC_Compare )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open )->EnableWindow( FALSE );
	GetDlgItem( IDC_Save2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Handle2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Open2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_Image )->EnableWindow( TRUE );
	GetDlgItem( IDC_Video )->EnableWindow( TRUE );
	GetDlgItem( IDC_Reset )->EnableWindow( FALSE );
	GetDlgItem( IDC_Weight )->EnableWindow( FALSE );
	GetDlgItem( IDC_Block )->EnableWindow( FALSE );
	GetDlgItem( IDC_maxatom )->EnableWindow( FALSE );
	GetDlgItem( IDC_GuideRadius )->EnableWindow( FALSE );

	m_Radius.SetPos(10);
	m_MaxAtom.SetPos(10);
	m_GuideRadius.SetPos(0);
	m_Weight.SetPos (0);

	m_Reset = 0;
	m_Save = 0;
    sumI20=0;
    sumI21=0;
	sumI22=0;
	Lnum=0;
	ShowImage(TheImage2,IDC_Dehaze);
	ShowImage(TheImage2,IDC_Haze);
	UpdateData();
	m_Sum = 0;
	m_Present = 0;
	UpdateData(false);
}

//����������ͼ����ǿ
void CvideodehazeDlg::Hist_Equalize(IplImage * Image, IplImage * ImageOut)
{
    int height,width;//��������
	int hist[256],h[256],h2[256];
	int k=0,count=0,count2=0;
	float  fpHist[256],fpHist2[256];
	float eqHistTemp[256],eqHistTemp2[256];
	int eqHist2[256];
	height = Image->height ;//��ȡͼ����
	width = Image->width ;
	int size = height * width;
	memset(&hist,0x00,sizeof(int)*256);  //�����ʼ��
	memset(&h,0x00,sizeof(int)*256);
	memset(&h2,0x00,sizeof(int)*256);
	memset(&fpHist,0x00,sizeof(float)*256);
	memset(&eqHistTemp,0x00,sizeof(float)*256);
	memset(&fpHist2,0x00,sizeof(float)*256);
	memset(&eqHistTemp2,0x00,sizeof(float)*256);
	memset(&eqHist2,0x00,sizeof(int)*256);

	for (int i = 0;i < height*width; i++) {//����ԭʼͼ��ֱ��ͼ
	    unsigned char GrayIndex = Image->imageData[i];
		hist[GrayIndex] ++ ;
	}
	for (int i = 0; i< 256; i++) {  // ����Ҷȷֲ��ܶ�
		fpHist[i] = (float)hist[i] / (float)(size); 
	}

	eqHistTemp[0] = fpHist[0];
	for ( int i = 1; i< 256; i++) {  // �����ۼ�ֱ��ͼ�ֲ�
		eqHistTemp[i] = eqHistTemp[i-1] + fpHist[i];
	}

	for (int i = 0; i < height; ++i){ 
		for (int j = 0; j < width; ++j){  //�ж���������֮���ֵ
			if ( j>=2 ){
			    k+=abs(Image->imageData[i*j+j]-Image->imageData[i*j+j-2]);
			    if(abs(Image->imageData[i*j+j]-Image->imageData[i*j+j-2])>5){
					unsigned char GrayIndex =Image->imageData[i*j+j];
				    hist[ GrayIndex ]++;//�������5��������ֱ��ͼ��1
				    count++;
				}
			}
		}
	}
	
	int unit;
	 if((count/256)>(size/256))unit=size/256+0.5;
	 else unit=count/256+0.5; //�������ֱ��ͼȡֵ
	 
	 for (int i = 0; i< 256; i++){
		h[i] =unit; //�������ֱ��ͼ
	    if( 50< i <250 )  //���㴦���ֱ��ͼ
		    h2[i] = int(( 1-Weight) * hist[i] + Weight*h[i] + 0.5 );
		else
			h2[i] = int( (( 1-Weight) * hist[i] + Weight*h[i])/6 + 0.5 );

		count2+=h2[i];//����������ֵ
    }  
	
	for (int i = 0; i< 256; i++) { // ���㴦���Ҷȷֲ��ܶ�
		fpHist2[i] = (float)h2[i] / (float)(count2); 
	}
	
	eqHistTemp2[0] = fpHist2[0];
	for ( int i = 1; i< 256; i++) {  // ���㴦����ۼ�ֱ��ͼ�ֲ�
		eqHistTemp2[i] = eqHistTemp2[i-1] + fpHist2[i];
	}

	for ( int i = 0; i< 256; i++) { //ͨ��ԭʼ�ۼ�ֱ��ͼ�ֲ��ʹ�����ۼ�ֱ��ͼ�ֲ�����ƥ�亯��
		if(eqHistTemp[i]<=eqHistTemp2[0]){
			eqHist2[i] = 0;
		}
		else if(eqHistTemp[i]>=eqHistTemp2[255]){
			for (i; i< 256; i++) eqHist2[i] = 255;
			break;
		}
		else{
		for(int l=1;l<256;l++){
		    if(eqHistTemp2[l-1]<eqHistTemp[i]&&eqHistTemp[i]<=eqHistTemp2[l]){
				eqHist2[i] = l;
				break;
			}
		}
		}
	}
	
	for ( int i = 0;i < height*width; i++) {//���лҶ�ӳ�� ���⻯
		unsigned char GrayIndex = Image->imageData[i];
		ImageOut->imageData[i] = eqHist2[GrayIndex];
	}

}

//�������˳�
void CvideodehazeDlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	m_Reset = 0;
	m_Save = 0;
	AfxGetMainWnd()->SendMessage ( WM_CLOSE );
}

//��������ɫͼ���ͨ������
void CvideodehazeDlg::HE_Color(IplImage * pImage, IplImage * pEImage)
{      
     
    const int MAX_CHANNEL = 4;  
    IplImage *pImageChannel[MAX_CHANNEL] = {NULL};  
  
    int i;  
    for (i = 0; i < pImage->nChannels; i++)  
        pImageChannel[i] = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);  
   //��ͼ���ΪpImage->nChannels��ͨ��
    cvSplit(pImage, pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3]);  
      
    for (i = 0; i < pImage->nChannels; i++)  {//��ͨ������
		if(m_Choose==1) enhance(pImageChannel[i], pImageChannel[i]);
		else Hist_Equalize(pImageChannel[i], pImageChannel[i]);  
	}
   //�ϲ�
    cvMerge(pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3], pEImage);  
  
    for (i = 0; i < pImage->nChannels; i++)  
        cvReleaseImage(&pImageChannel[i]);  //�ͷű���
}

//������ʱ�򼰿���ͼ����ǿ
void CvideodehazeDlg::enhance(IplImage * Image, IplImage * ImageOut)
{  //��������
	int height,width,sumI=0;
    int hist[256],hist1[256],hist2[256],h[256],h2[256],h3[256];
	int k=0,count=0,L=0,L1=0,count2=0;
    float  fpHist[256],fpHist2[256],f1[256],f2[256],f3[256];
	float eqHistTemp[256],eqHistTemp1[256],eqHistTemp2[256];
	int eqHist[256],eqHist1[256], eqHist2[256];
	height = Image->height ;  //��ȡͼ����
	width = Image->width ;
	IplImage * Save=cvCreateImage(cvSize(height,width),Image->depth, 3);

	int size = height * width;
	memset(&hist,0x00,sizeof(int)*256);//�����ʼ��
	memset(&hist1,0x00,sizeof(int)*256);
	memset(&hist2,0x00,sizeof(int)*256);
	memset(&h,0x00,sizeof(int)*256);
	memset(&h2,0x00,sizeof(int)*256);
	memset(&h3,0x00,sizeof(int)*256);
	memset(&eqHistTemp,0x00,sizeof(float)*256);
	memset(&eqHistTemp1,0x00,sizeof(float)*256);
	memset(&eqHistTemp2,0x00,sizeof(float)*256);
	memset(&f1,0x00,sizeof(float)*256);
	memset(&f2,0x00,sizeof(float)*256);
	memset(&f3,0x00,sizeof(float)*256);
	memset(&eqHist,0x00,sizeof(int)*256);
	memset(&eqHist1,0x00,sizeof(int)*256);
	memset(&eqHist2,0x00,sizeof(int)*256);

	for (int i = 0;i < height*width; i++) {//����ԭֱ��ͼ
	    unsigned char GrayIndex = Image->imageData[i];
		hist[GrayIndex] ++ ;
		sumI+=GrayIndex;
	}
	
	for (int i = 0;i < 256; i++) {
		h[i] = int(float(size)/256+0.5);                  //�������ֱ��ͼ
	}
	
	if( numfrm == 1 ){
		for (int i = 0; i< 256; i++) {  // ����Ҷȷֲ��ܶ�
			f1[i] = (float)hist[i] / (float)(size); 
	    }

		eqHistTemp[0] = f1[0];
	    for ( int i = 1; i< 256; i++) {   // �����ۼ�ֱ��ͼ�ֲ�
		    eqHistTemp[i] = eqHistTemp[i-1] + f1[i];
	    }

	    for (int i = 0; i< 256; i++){   // ���㴦���ֱ��ͼ�ֲ�
		    h2[i] =  int( Lambda/(1+Lambda) * h[i] + 1.0/(1+Lambda)*hist[i] + 0.5 );//h[i];//
	    }
	}
	else{
	    L=(int)(float(sumI)/size+0.5);	//����ƽ������
		if((Lnum%3)==0) 
			L1=(int)(1.0/(1+Beta)*L + Beta/(1+Beta)*LLast1+0.5); //���㱾֡����ǰ֡��ƽ������
	    else if((Lnum%3)==1) 
			L1=(int)(1.0/(1+Beta)*L + Beta/(1+Beta)*LLast2+0.5);
	    else 
			L1=(int)(1.0/(1+Beta)*L + Beta/(1+Beta)*LLast0+0.5);
	    
	    a=(1-float(L1)/L)/(255-L);  //����ӳ�亯������
	    b=1-255*a;

		eqHist[0]=0;
		eqHist[255]=255;
		for (int i = 1; i< 255; i++){//ɫ��ӳ��
			if( 0<i&&i<255 )eqHist[i]=int(a*i*i+b*i+0.5);
		    if( eqHist[i]<=0 ) eqHist[i]=0;
		    else if(eqHist[i]>=255) eqHist[i]=255;
	    }

	    for(int i = 0;i < height*width; i++){//��ɫ��ӳ����ͼ�񱣴�
		    unsigned char GrayIndex = Image->imageData[i];
		    Save->imageData [i] =eqHist[ GrayIndex ];
		    hist1[eqHist[ GrayIndex ]] ++ ; //����ֱ��ͼ
	    }

		for (int i = 0; i< 256; i++) {  
			f1[i] = (float)hist1[i] / (float)(size); // ����Ҷȷֲ��ܶ�
	    }
	
		eqHistTemp[0] = f1[0];
	    for ( int i = 1; i< 256; i++) {  // �����ۼ�ֱ��ͼ�ֲ�
		    eqHistTemp[i] = eqHistTemp[i-1] + f1[i];
	    }

	    for (int i = 0; i< 256; i++){  //����ֱ��ͼ
			if((Lnum%3)==0) 
				h2[i] = int( Lambda/(1+Lambda+Gama) * h[i] + 1.0/(1+Lambda+Gama)*hist1[i] + Gama/(1+Lambda+Gama) * last1[i]+ 0.5 );
		    else if((Lnum%3)==1) 
				h2[i] =int( Lambda/(1+Lambda+Gama) * h[i] + 1.0/(1+Lambda+Gama)*hist1[i] + Gama/(1+Lambda+Gama) * last2[i]+ 0.5 );
		    else 
		     h2[i] =  int( Lambda/(1+Lambda+Gama) * h[i] + 1.0/(1+Lambda+Gama)*hist1[i] + Gama/(1+Lambda+Gama) * last0[i]+ 0.5 );// h[i];//
	    }
	}
	Lnum++;
	//memset(&last,0x00,sizeof(float)*256);
	for (int i = 0; i< 256; i++){  //�洢��һֱ֡��ͼ
		count += h2[i];
		if((Lnum%3)==1)last1[i] = h2[i] ;
		else if((Lnum%3)==2) last2[i] = h2[i] ;
		else last0[i] = h2[i] ;
	}
	
	for (int i = 0; i< 256; i++){  //����ֱ��ͼ
		h3[i]=int((1-Weight)*h2[i]+Weight*h[i]+0.5);
		count2 += h3[i];
	}
	
		
	for (int i = 0; i< 256; i++) {  
		f2[i] = (float)h2[i] / (float)(count); // ����Ҷȷֲ��ܶ�
		f3[i] = (float)h3[i] / (float)(count2); 
	}
	
	eqHistTemp1[0] = f2[0];
	for ( int i = 1; i< 256; i++) {  // �����ۼ�ֱ��ͼ�ֲ�
		eqHistTemp1[i] = eqHistTemp1[i-1] + f2[i];
	}

	for ( int i = 0; i< 256; i++) { //ͨ��ԭʼ�ۼ�ֱ��ͼ�ֲ��ʹ�����ۼ�ֱ��ͼ�ֲ�����ƥ�亯��1
		if(eqHistTemp[i]<=eqHistTemp1[0]){
			eqHist1[i] = 0;
		}
		else if(eqHistTemp[i]>=eqHistTemp1[255]){
			eqHist1[i] = 255;
		}
		else{
		for(int j=1;j<256;++j){
		    if(eqHistTemp1[j-1]<eqHistTemp[i]&&eqHistTemp[i]<=eqHistTemp1[j]){
				eqHist1[i] = j;
				break;
			}
		}
		}
	}
	
	eqHistTemp2[0] = f3[0];
	for ( int i = 1; i< 256; i++) { //ͨ��ԭʼ�ۼ�ֱ��ͼ�ֲ��ʹ�����ۼ�ֱ��ͼ�ֲ�����ƥ�亯��2
		eqHistTemp2[i] = eqHistTemp2[i-1] + f3[i];
	}

	for ( int i = 0; i< 256; i++) { 
		if(eqHistTemp1[i]<=eqHistTemp2[0]){
			eqHist2[i] = 0;
		}
		else if(eqHistTemp1[i]>=eqHistTemp2[255]){
			eqHist2[i] = 255;
		}
		else{
		for(int j=1;j<256;++j){
		    if(eqHistTemp2[j-1]<eqHistTemp1[i]&&eqHistTemp1[i]<=eqHistTemp2[j]){
				eqHist2[i] = j;
				break;
			}
		}
		}
	}	
	

	for ( int i = 0;i < height*width; i++) { //���лҶ�ӳ��
		unsigned char GrayIndex;
		if(numfrm==1) {
			 GrayIndex = Image->imageData[i];
			 ImageOut->imageData[i] =  eqHist1[GrayIndex];
		}
		else {
			 GrayIndex = Save->imageData[i];
			 ImageOut->imageData[i] =  eqHist2[eqHist1[GrayIndex]];
		}
			 if((Lnum%3)==1) sumI21 += eqHist1[GrayIndex] ;
		     else if((Lnum%3)==2) sumI22 += eqHist1[GrayIndex] ;
		     else sumI20 += eqHist1[GrayIndex];	
	}
	if(numfrm==1){  //�洢ǰ����֡ƽ������
		if((Lnum%3)==1){
		LLast1=(int)(float(sumI21)/(size*numfrm)+0.5);
		sumI21=0;
	}
	else if((Lnum%3)==2) {
		LLast2=(int)(float(sumI22)/(size*numfrm)+0.5);
		sumI22=0;
	}
	else {
		LLast0=(int)(float(sumI20)/(size*numfrm)+0.5);
		sumI20=0;
	}

	}
	else{
	if((Lnum%3)==1){
		LLast1=(int)(float(LLast1)*(float(numfrm)/(numfrm+1))+float(sumI21)/(size*(numfrm+1))+0.5);
		sumI21=0;
	}
	else if((Lnum%3)==2) {
		LLast2=(int)(float(LLast2)*(float(numfrm)/(numfrm+1))+float(sumI22)/(size*(numfrm+1))+0.5);
		sumI22=0;
	}
	else {
		LLast0=(int)(float(LLast0)*(float(numfrm)/(numfrm+1))+float(sumI20)/(size*(numfrm+1))+0.5);
		sumI20=0;
	}
	}

  	cvReleaseImage(&Save);
}
