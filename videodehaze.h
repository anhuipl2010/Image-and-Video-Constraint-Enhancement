
// videodehaze.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <opencv2\opencv.hpp>

#define IMAGE_WIDTH 555
#define IMAGE_HEIGHT 388
#define IMAGE_CHANNELS 3

// CvideodehazeApp:
// See videodehaze.cpp for the implementation of this class
//

class CvideodehazeApp : public CWinApp
{
public:
	CvideodehazeApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CvideodehazeApp theApp;