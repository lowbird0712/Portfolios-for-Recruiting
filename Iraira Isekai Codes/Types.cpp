#include "Types.h"

// FLOATPOINT
FLOATPOINT::FLOATPOINT() : x(0.f), y(0.f) { }
FLOATPOINT::FLOATPOINT(const float _x, const float _y) : x(_x), y(_y) { }
FLOATPOINT::FLOATPOINT(const FLOATPOINT & _point) : x(_point.x), y(_point.y) { }
FLOATPOINT::FLOATPOINT(const POINT & _point) : x(_point.x), y(_point.y) { }

void FLOATPOINT::operator=(const FLOATPOINT & _point) { x = _point.x; y = _point.y; }
void FLOATPOINT::operator=(const POINT & _point) { x = _point.x; y = _point.y; }

FLOATPOINT FLOATPOINT::operator+(const FLOATPOINT & _size) const {
	FLOATPOINT point = *this;
	point.x += _size.x;
	point.y += _size.y;
	return point;
}
void FLOATPOINT::operator+=(const FLOATPOINT & _size) {
	this->x += _size.x;
	this->y += _size.y;
}
FLOATPOINT FLOATPOINT::operator-(const FLOATPOINT & _size) const {
	FLOATPOINT point = *this;
	point.x -= _size.x;
	point.y -= _size.y;
	return point;
}
void FLOATPOINT::operator-=(const FLOATPOINT & _size) {
	this->x -= _size.x;
	this->y -= _size.y;
}
FLOATPOINT FLOATPOINT::operator*(const FLOATPOINT & _pivot) const {
	FLOATPOINT point = *this;
	point.x *= _pivot.x;
	point.y *= _pivot.y;
	return point;
}
void FLOATPOINT::operator*=(const FLOATPOINT & _pivot) {
	this->x *= _pivot.x;
	this->y *= _pivot.y;
}

bool FLOATPOINT::operator==(const FLOATPOINT & _point) const {
	if (x == _point.x && y == _point.y)
		return TRUE;
	else
		return FALSE;
}

bool FLOATPOINT::operator!=(const FLOATPOINT & _point) const {
	return !(*this == _point);
}

// FLOATRECT
FLOATRECT::FLOATRECT() : left(0.f), top(0.f), right(0.f), bottom(0.f) { }
FLOATRECT::FLOATRECT(const float _left, const float _top, const float _right, const float _bottom)
	: left(_left), top(_top), right(_right), bottom(_bottom) { }
FLOATRECT::FLOATRECT(const FLOATRECT & _rect) : left(_rect.left), top(_rect.top), right(_rect.right), bottom(_rect.bottom) { }
FLOATRECT::FLOATRECT(const RECT & _rect) : left(_rect.left), top(_rect.top), right(_rect.right), bottom(_rect.bottom) { }

void FLOATRECT::operator=(const FLOATRECT & _rect) {
	left = _rect.left;
	top = _rect.top;
	right = _rect.right;
	bottom = _rect.bottom;
}
void FLOATRECT::operator=(const RECT & _rect) {
	left = _rect.left;
	top = _rect.top;
	right = _rect.right;
	bottom = _rect.bottom;
}

FLOATRECT FLOATRECT::operator+(const FLOATSIZE & _size) const {
	FLOATRECT rect = *this;
	rect.left += _size.x;
	rect.top += _size.y;
	rect.right += _size.x;
	rect.bottom += _size.y;
	return rect;
}
void FLOATRECT::operator+=(const FLOATSIZE & _size) {
	left += _size.x;
	top += _size.y;
	right += _size.x;
	bottom += _size.y;
}

FLOATCIRCLE::FLOATCIRCLE() : x(0.f), y(0.f), radius(0.f) { }
FLOATCIRCLE::FLOATCIRCLE(const float _x, const float _y, const float _radius) : x(_x), y(_y), radius(_radius) { }
FLOATCIRCLE::FLOATCIRCLE(const POSITION & _center, const float _radius) : x(_center.x), y(_center.y), radius(_radius) { }
FLOATCIRCLE::FLOATCIRCLE(const FLOATCIRCLE & _circle) : x(_circle.x), y(_circle.y), radius(_circle.radius) { }

void FLOATCIRCLE::operator=(const FLOATCIRCLE & _circle) {
	x = _circle.x;
	y = _circle.y;
	radius = _circle.radius;
}

FLOATCIRCLE FLOATCIRCLE::operator+(const FLOATSIZE & _size) const {
	FLOATCIRCLE circle = *this;
	circle.x += _size.x;
	circle.y += _size.y;
	return circle;
}

void FLOATCIRCLE::operator+=(const FLOATSIZE & _size) {
	x += _size.x;
	y += _size.y;
}

COLOR::COLOR() : red(0), green(0), blue(0) { }
COLOR::COLOR(const unsigned char _red, const unsigned char _green, const unsigned char _blue)
	: red(_red), green(_green), blue(_blue) { }
COLOR::COLOR(const COLOR & _color) : red(_color.red), green(_color.green), blue(_color.blue) { }
COLOR::COLOR(const COLORREF & _color) : red(GetRValue(_color)), green(GetGValue(_color)), blue(GetBValue(_color)) { }

void COLOR::operator=(const COLORREF & _color) {
	red = GetRValue(_color);
	green = GetGValue(_color);
	blue = GetBValue(_color);
}

bool COLOR::operator==(const COLOR & _color) const {
	if (red == _color.red && green == _color.green && blue == _color.blue)
		return TRUE;
	else
		return FALSE;
}

bool COLOR::operator==(const COLORREF & _color) const {
	COLOR color = _color;

	return *this == color;
}

bool COLOR::operator!=(const COLOR & _color) const {
	if (*this == _color)
		return FALSE;
	else
		return TRUE;
}

bool COLOR::operator!=(const COLORREF & _color) const {
	COLOR color = _color;

	return *this != color;
}

FLOATSIZERECT::FLOATSIZERECT() : left(0.f), top(0.f), width(0.f), height(0.f) { }
FLOATSIZERECT::FLOATSIZERECT(const float _left, const float _top, const float _width, const float _height)
	: left(_left), top(_top), width(_width), height(_height) { }