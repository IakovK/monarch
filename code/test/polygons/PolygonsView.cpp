
// polygonsView.cpp : implementation of the CPolygonsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "polygons.h"
#endif

#include "polygonsDoc.h"
#include "polygonsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPolygonsView

IMPLEMENT_DYNCREATE(CPolygonsView, CView)

BEGIN_MESSAGE_MAP(CPolygonsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CPolygonsView construction/destruction

CPolygonsView::CPolygonsView() noexcept
{
	// TODO: add construction code here

}

CPolygonsView::~CPolygonsView()
{
}

BOOL CPolygonsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPolygonsView drawing

void CPolygonsView::OnDraw(CDC* /*pDC*/)
{
	CpolygonsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPolygonsView printing

BOOL CPolygonsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPolygonsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPolygonsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPolygonsView diagnostics

#ifdef _DEBUG
void CPolygonsView::AssertValid() const
{
	CView::AssertValid();
}

void CPolygonsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpolygonsDoc* CPolygonsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpolygonsDoc)));
	return (CpolygonsDoc*)m_pDocument;
}
#endif //_DEBUG


// CPolygonsView message handlers


int CPolygonsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Создание 4-х потоков - каждый поток создает свое окно и рисует в нем
	CreateThreads();

	return 0;
}

void CPolygonsView::CreateThreads()
{
	for (int j = 0; j < 4; j++)
	{
		CPolygonThread* pThread = (CPolygonThread*)AfxBeginThread(RUNTIME_CLASS(CPolygonThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pThread->m_id = j + 1; // идентификатор. В зависимости от него окно будет размещаться в главном окне:
							   //                                                                        1 2
							   //                                                                        3 4
		pThread->m_view = this;
		pThread->ResumeThread();                   //New Thread Started
		m_threads[j] = pThread;
	}
}

void CPolygonsView::StopThreads()
{
	for (int j = 0; j < 4; j++)
	{
		m_threads[j]->PostThreadMessageW(WM_QUIT, 0, 0);
		WaitForSingleObject(m_threads[j]->m_hThread, INFINITE);
	}
}

void CPolygonsView::OnDestroy()
{
	CView::OnDestroy();

	StopThreads();
}


void CPolygonsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// Каждому потоку посылаем сигнал, чтобы он изменил размер своего окна
	for (int j = 0; j < 4; j++)
	{
		m_threads[j]->PostThreadMessage(WM_RESIZE, 0, 0);
	}
}
