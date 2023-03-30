
// polygons.h : main header file for the polygons application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CpolygonsApp:
// See polygons.cpp for the implementation of this class
//

class CpolygonsApp : public CWinApp
{
public:
	CpolygonsApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
};

#define WM_RESIZE (WM_USER + 100)

extern CpolygonsApp theApp;
