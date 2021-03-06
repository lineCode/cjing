

constexpr Rect::Rect():
	x(0),y(0),
	width(0),height(0)
{
}

constexpr Rect::Rect(int x, int y):
	x(x), y(y),
	width(0), height(0)
{
}

constexpr Rect::Rect(const Point2& xy) :
	x(xy.x), y(xy.y),
	width(0), height(0)
{
}

constexpr Rect::Rect(const Size& size):
	x(0),y(0),
	width(size.width),
	height(size.height)
{
}

constexpr Rect::Rect(int x, int y, int width, int height):
	x(x),y(y),
	width(width),height(height)
{
}

constexpr Rect::Rect(const Point2&xy, const Size& size):
	x(xy.x),
	y(xy.y),
	width(size.width),
	height(size.height)
{
}

constexpr Rect::Rect(const Point2&lt, const Point2&rb):
	x(lt.x),
	y(lt.y),
	width(rb.x - lt.x),
	height(rb.y - lt.y)
{
}

inline void Rect::Set(int x1, int y1, int x2, int y2)
{
	x = x1;
	y = y1;
	width = x2 - x1;
	height = y2 - y1;
}

inline void Rect::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}

inline void Rect::SetSize(int w, int h)
{
	width = w;
	height = h;
}

inline void Rect::SetCenter(int cx, int cy)
{
	x = cx - width / 2;
	y = cy - height / 2;
}

inline Point2 Rect::GetPos() const
{
	return Point2(x, y);
}

inline Size Rect::GetSize() const
{
	return Size(width, height);
}

inline Point2 Rect::GetCenterPos() const
{
	return Point2(x + width / 2, y + height / 2);
}

inline void Rect::AddX(int dx)
{
	x += dx;
}

inline void Rect::AddY(int dy)
{
	y += dy;
}

inline void Rect::AddPos(int dx, int dy)
{
	x += dx; 
	y += dy;
}

inline void Rect::AddWidth(int dw)
{
	width += dw;
}

inline void Rect::AddHeight(int dh)
{
	height += dh;
}

inline void Rect::AddSize(int dw, int dh)
{
	width += dw;
	height += dh;
}

inline bool Rect::Contains(int px, int py) const
{
	return px >= x && px <= (x + width) &&
		py >= y && py <= (y + height);
}

inline bool Rect::Contains(const Point2 & point) const
{
	return point.x >= x && point.x <= (x + width) &&
		point.y >= y && point.y <= (y + height);
}

inline bool Rect::Overlaps(const Rect & rect) const
{
	return !(x + width < rect.x || x > rect.x +rect.width ||
		y + height < rect.y || y > rect.y + rect.height);
}

inline bool Rect::operator==(const Rect & rect)
{
	return x == rect.x && y == rect.y &&
		width == rect.width && height == rect.width;
}

inline bool Rect::operator!=(const Rect & rect)
{
	return x != rect.x || y != rect.y ||
		width != rect.width || height != rect.width;
}

/**
*	\brief 返回两个rect的相交区域
*
*    如果存在相交，则以rect返回相交区域
*    如果不存在相交，则返回原点在起始位置，大小为0的rects
*/
inline Rect& Rect::operator&=(const Rect& rect)
{
	int lx = max(x, rect.x);
	int ly = max(y, rect.y);
	int rx = min(x + width, rect.x + rect.width);
	int ry = min(y + height, rect.y + rect.height);

	if (lx >= rx || ly >= ry)
		*this = Rect();
	else
		*this = Rect(Point2(lx, ly), Point2(rx, ry));
	return *this;
}

/**
*	\brief 返回两个rect的并区域
*/
inline Rect& Rect::operator |=(const Rect& rect)
{
	int lx = min(x, rect.x);
	int ly = min(y, rect.y);
	int rx = max(x + width, rect.x + rect.width);
	int ry = max(y + height, rect.y + rect.height);

	if (lx >= rx || ly >= ry)
		*this = Rect();
	else
		*this = Rect(Point2(lx, ly), Point2(rx, ry));
	return *this;
}
