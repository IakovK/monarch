#pragma once

// CPolygonsWnd frame

#include <memory>
#include <vector>

class CPolygonsWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CPolygonsWnd)

	// абстрактный класс для всех фигур.
	class Figure
	{
		CPoint m_center;
		COLORREF m_color;
	public:
		enum
		{
			centerRange = 1000,
			radiusRange = 50
		};

		Figure(CPoint center, COLORREF color)
			:m_center(center)
			,m_color(color)
		{
		}
		void Draw(CDC& dc, CRect& r);
	private:
		CPoint Scale(CPoint &p, CRect& r);
		virtual void DrawFigure(CDC& dc, CPoint& center) = 0;
	};

	class Circle : public Figure
	{
		int m_radius;
	public:
		Circle(CPoint center, int radius, COLORREF color)
			:m_radius(radius)
			, Figure(center, color)
		{
		}
	private:
		virtual void DrawFigure(CDC& dc, CPoint& center);
	};

	class Triangle : public Figure
	{
		CPoint m_p1, m_p2, m_p3;
	public:
		Triangle(CPoint center, CPoint p1, CPoint p2, CPoint p3, COLORREF color)
			:m_p1(p1)
			, m_p2(p2)
			, m_p3(p3)
			, Figure(center, color)
		{
		}
	private:
		virtual void DrawFigure(CDC& dc, CPoint& center);
	};

	class Rectangle : public Figure
	{
		CPoint m_p1, m_p2;
	public:
		Rectangle(CPoint center, CPoint p1, CPoint p2, COLORREF color)
			:m_p1(p1)
			, m_p2(p2)
			, Figure(center, color)
		{
		}
	private:
		virtual void DrawFigure(CDC& dc, CPoint& center);
	};

	class Pentagon : public Figure
	{
		int m_radius;
	public:
		Pentagon(CPoint center, int radius, COLORREF color)
			:m_radius(radius)
			, Figure(center, color)
		{
		}
	private:
		virtual void DrawFigure(CDC& dc, CPoint& center);
	};

protected:
	CPolygonsWnd();           // protected constructor used by dynamic creation
	virtual ~CPolygonsWnd();

	void GenerateGeometry();
	Figure* GenerateFigure();
	Figure* CreateCircle();
	Figure* CreateTriangle();
	Figure* CreateRectangle();
	Figure* CreatePentagon();
	CPoint GetRandomPoint(int xRange, int yRange);
	CPoint GetRandomPointCentered(int xRange, int yRange);
	int GetRandomInteger(int range);
	COLORREF GetRandomColor();

	std::vector<std::unique_ptr<Figure>> m_figures;

public:
	int m_id;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
};


