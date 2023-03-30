// PolygonsWnd.cpp : implementation file
//

#include "pch.h"
#include "polygons.h"
#include "PolygonsWnd.h"


// CPolygonsWnd

IMPLEMENT_DYNCREATE(CPolygonsWnd, CFrameWnd)

void CPolygonsWnd::Figure::Draw(CDC& dc, CRect& r)
{
	CPoint scaledCenter = Scale(m_center, r);
	dc.SelectObject(GetStockObject(DC_PEN));
	dc.SetDCPenColor(m_color);
	dc.SelectObject(GetStockObject(DC_BRUSH));
	dc.SetDCBrushColor(m_color);

	DrawFigure(dc, scaledCenter);
}

CPoint CPolygonsWnd::Figure::Scale(CPoint& p, CRect& r)
{
	double xf = (double)r.Width() / centerRange;
	double yf = (double)r.Height() / centerRange;
	double f = min(xf, yf);
	CPoint retVal = { (int)(p.x * xf), (int)(p.y * yf) };
	return retVal;
}

void CPolygonsWnd::Circle::DrawFigure(CDC& dc, CPoint& center)
{
	dc.Ellipse(center.x - m_radius, center.y - m_radius, center.x + m_radius, center.y + m_radius);
}

void CPolygonsWnd::Triangle::DrawFigure(CDC& dc, CPoint& center)
{
	CPoint points[]{ m_p1 + center, m_p2 + center, m_p3 + center };
	dc.Polygon(points, 3);
}

void CPolygonsWnd::Rectangle::DrawFigure(CDC& dc, CPoint& center)
{
	dc.Rectangle(m_p1.x + center.x, m_p1.y + center.y, m_p2.x + center.x, m_p2.y + center.y);
}

void CPolygonsWnd::Pentagon::DrawFigure(CDC& dc, CPoint& center)
{
	CPoint points[5];
	double PI = acos(-1);
	for (int j = 0; j < 5; j++)
	{
		points[j].x = center.x + m_radius * cos((2 * PI * j) / 5);
		points[j].y = center.y + m_radius * sin((2 * PI * j) / 5);
	}
	dc.Polygon(points, 5);
}

CPolygonsWnd::CPolygonsWnd()
{
}

CPolygonsWnd::~CPolygonsWnd()
{
}


BEGIN_MESSAGE_MAP(CPolygonsWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPolygonsWnd message handlers


void CPolygonsWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CFrameWnd::OnPaint() for painting messages
	CRect r;
	GetClientRect(&r);
	for (auto &x : m_figures)
	{
		x->Draw(dc, r);
	}
}


int CPolygonsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GenerateGeometry();

	return 0;
}

CPoint CPolygonsWnd::GetRandomPoint(int xRange, int yRange)
{
	int x = rand() % xRange;
	int y = rand() % yRange;
	return CPoint(x, y);
}

CPoint CPolygonsWnd::GetRandomPointCentered(int xRange, int yRange)
{
	CPoint p1 = GetRandomPoint(2 * xRange, 2 * yRange);
	CPoint p2 = p1 - CPoint(xRange, yRange);
	return p2;
}

int CPolygonsWnd::GetRandomInteger(int range)
{
	int n = rand() % range;
	return n;
}

COLORREF CPolygonsWnd::GetRandomColor()
{
	int r = rand() % 240;
	int g = rand() % 240;
	int b = rand() % 240;
	return RGB(r, g, b);
}

CPolygonsWnd::Figure* CPolygonsWnd::CreateCircle()
{
	CPoint center = GetRandomPoint(Figure::centerRange, Figure::centerRange);
	int radius = GetRandomInteger(Figure::radiusRange);
	COLORREF cr = GetRandomColor();
	return new Circle(center, radius, cr);
}

CPolygonsWnd::Figure* CPolygonsWnd::CreateTriangle()
{
	CPoint center = GetRandomPoint(Figure::centerRange, Figure::centerRange);
	int radius = GetRandomInteger(Figure::radiusRange);
	CPoint p1 = GetRandomPointCentered(Figure::radiusRange, Figure::radiusRange);
	CPoint p2 = GetRandomPointCentered(Figure::radiusRange, Figure::radiusRange);
	CPoint p3 = GetRandomPointCentered(Figure::radiusRange, Figure::radiusRange);
	COLORREF cr = GetRandomColor();
	return new Triangle(center, p1, p2, p3, cr);
}

CPolygonsWnd::Figure* CPolygonsWnd::CreateRectangle()
{
	CPoint center = GetRandomPoint(Figure::centerRange, Figure::centerRange);
	int radius = GetRandomInteger(Figure::radiusRange);
	CPoint p1 = GetRandomPointCentered(Figure::radiusRange, Figure::radiusRange);
	CPoint p2 = GetRandomPointCentered(Figure::radiusRange, Figure::radiusRange);
	CRect r(p1, p2);
	r.NormalizeRect();
	COLORREF cr = GetRandomColor();
	return new Rectangle(center, r.TopLeft(), r.BottomRight(), cr);
}

CPolygonsWnd::Figure* CPolygonsWnd::CreatePentagon()
{
	CPoint center = GetRandomPoint(Figure::centerRange, Figure::centerRange);
	int radius = GetRandomInteger(Figure::radiusRange);
	COLORREF cr = GetRandomColor();
	return new Pentagon(center, radius, cr);
}

CPolygonsWnd::Figure* CPolygonsWnd::GenerateFigure()
{
	int figureKind = rand() % 4;
	switch (figureKind)
	{
	case 0:
		return CreateCircle();
	case 1:
		return CreateTriangle();
	case 2:
		return CreateRectangle();
	case 3:
		return CreatePentagon();
	}
	return NULL;
}

void CPolygonsWnd::GenerateGeometry()
{
	LARGE_INTEGER pc;
	QueryPerformanceCounter(&pc);
	srand(pc.LowPart);
	int numFigures = rand() % 100;

	for (int j = 0; j < numFigures; j++)
	{
		Figure* f = GenerateFigure();
		m_figures.push_back(std::unique_ptr<Figure>(f));
	}
}


void CPolygonsWnd::OnClose()
{
	// Do nothing
}
