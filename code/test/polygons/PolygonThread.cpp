// CPolygonThread.cpp : implementation file
//

#include "pch.h"
#include "polygons.h"
#include "PolygonThread.h"
#include "polygonsView.h"
#include "PolygonsWnd.h"


// CPolygonThread

IMPLEMENT_DYNCREATE(CPolygonThread, CWinThread)

CPolygonThread::CPolygonThread()
	:m_id(0)
	, m_view(NULL)
	, m_wnd(NULL)
{
}

CPolygonThread::~CPolygonThread()
{
}

BOOL CPolygonThread::InitInstance()
{
	// Создаем окно. Внутри окна будут создавться и отрисовываться все фигуры
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CPolygonsWnd);
	m_wnd = (CPolygonsWnd*)pRuntimeClass->CreateObject();

	CString s;
	s.Format(_T("Thread #%d"), m_id);
	m_wnd->m_id = m_id;
	m_wnd->Create(NULL, s, WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CAPTION, CRect(0, 0, 0, 0), NULL, NULL, 0, NULL);
	return TRUE;
}

int CPolygonThread::ExitInstance()
{
	m_wnd->DestroyWindow();
	return CWinThread::ExitInstance();
}

// Обработчик сообщения WM_RESIZE
// Окна размещаются по отношению к родительскому окну (m_view) так:
//                                                             1 2
//                                                             3 42
void CPolygonThread::OnResize(WPARAM wParam, LPARAM lParam)
{
	CRect r0, r1, r2, r3, r4, rw;
	m_view->GetClientRect(&r0);
	r1 = r0; r2 = r0; r3 = r0; r4 = r0;

	r1.right = r0.right / 2;
	r1.bottom = r0.bottom / 2;

	r2.left = r1.right + 1;
	r2.bottom = r0.bottom / 2;

	r3.right = r0.right / 2;
	r3.top = r1.bottom + 1;

	r4.left = r1.right + 1;
	r4.top = r1.bottom + 1;

	switch (m_id)
	{
	case 1:
		rw = r1; break;
	case 2:
		rw = r2; break;
	case 3:
		rw = r3; break;
	case 4:
		rw = r4; break;
	default:
		rw = r0; break;
	}
	m_view->ClientToScreen(rw);
	m_wnd->SetWindowPos(&CWnd::wndTopMost, rw.left, rw.top, rw.Width(), rw.Height(), 0);
}

BEGIN_MESSAGE_MAP(CPolygonThread, CWinThread)
	ON_THREAD_MESSAGE(WM_RESIZE, OnResize)
END_MESSAGE_MAP()


// CPolygonThread message handlers
