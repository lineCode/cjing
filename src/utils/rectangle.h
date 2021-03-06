#ifndef _RECT_ANGLE_H_
#define _RECT_ANGLE_H_

#include"common\common.h"
#include"utils\point.h"
#include"utils\size.h"

class Rect
{
public:
	constexpr Rect();
	constexpr Rect(int x,int y);
	explicit constexpr Rect(const Point2& xy);
	explicit constexpr Rect(const Size& size);
	constexpr Rect(int x, int y, int width, int height);
	constexpr Rect(const Point2&xy, const Size& size);
	constexpr Rect(const Point2&lt, const Point2&rb);

	void Set(int x1, int y1, int x2, int y2);
	void SetPos(int x, int y);
	void SetSize(int w, int h);
	void SetCenter(int cx, int cy);

	Point2 GetPos()const;
	Size GetSize()const;
	Point2 GetCenterPos()const;

	void AddX(int dx);
	void AddY(int dy);
	void AddPos(int dx, int dy);
	void AddWidth(int dw);
	void AddHeight(int dh);
	void AddSize(int dw, int dh);

	bool Contains(int px, int py)const;
	bool Contains(const Point2& point)const;
	bool Overlaps(const Rect& rect)const;


	bool operator ==(const Rect& rect);
	bool operator !=(const Rect& rect);
	Rect& operator &=(const Rect& rect);
	Rect& operator |=(const Rect& rect);

public:
	int x, y;
	int width, height;

};

#include"rectangle.inl"

#endif