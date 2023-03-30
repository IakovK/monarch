#pragma once

// CPolygonThread

class CPolygonThread : public CWinThread
{
	DECLARE_DYNCREATE(CPolygonThread)

protected:
	CPolygonThread();           // protected constructor used by dynamic creation
	virtual ~CPolygonThread();

public:
	int m_id;
	class CPolygonsView* m_view;
	class CPolygonsWnd* m_wnd;
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnResize(WPARAM wParam, LPARAM lParam);
};


