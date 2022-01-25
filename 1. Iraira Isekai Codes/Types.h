#pragma once

#include <Windows.h>

#define		FAILED_FLOATPOINT		FLOATPOINT(-1.f, -1.f)

typedef struct FLOATPOINT {
	float x;
	float y;

	FLOATPOINT();
	FLOATPOINT(const float _x, const float _y);
	FLOATPOINT(const FLOATPOINT &_point);
	FLOATPOINT(const POINT &_point);

	void		operator=(const FLOATPOINT &_point);
	void		operator=(const POINT &_point);

	FLOATPOINT	operator+(const FLOATPOINT &_size) const;
	void		operator+=(const FLOATPOINT &_size);
	FLOATPOINT	operator-(const FLOATPOINT &_size) const;
	void		operator-=(const FLOATPOINT &_size);
	FLOATPOINT	operator*(const FLOATPOINT &_pivot) const; // Pivotsize를 구하기 위해 사용되는 연산자이다
	void		operator*=(const FLOATPOINT &_pivot);

	bool		operator==(const FLOATPOINT &_point) const;
	bool		operator!=(const FLOATPOINT &_point) const;
} FLOATPOINT, FLOATSIZE, POSITION, PIVOT;

struct FLOATRECT {
	float left;
	float top;
	float right;
	float bottom;

	FLOATRECT();
	FLOATRECT(const float _left, const float _top, const float _right, const float _bottom);
	FLOATRECT(const FLOATRECT &_rect);
	FLOATRECT(const RECT &_rect);

	void		operator=(const FLOATRECT &_rect);
	void		operator=(const RECT &_rect);

	FLOATRECT	operator+(const FLOATSIZE &_size) const;
	void		operator+=(const FLOATSIZE &_size);
};

struct FLOATSIZERECT {
	float left;
	float top;
	float width;
	float height;

	FLOATSIZERECT();
	FLOATSIZERECT(const float _left, const float _top, const float _width, const float _height);
};

struct FLOATCIRCLE {
	float x;
	float y;
	float radius;

	FLOATCIRCLE();
	FLOATCIRCLE(const float _x, const float _y, const float _radius);
	FLOATCIRCLE(const POSITION &_center, const float _radius);
	FLOATCIRCLE(const FLOATCIRCLE &_circle);

	void			operator=(const FLOATCIRCLE &_circle);

	FLOATCIRCLE		operator+(const FLOATSIZE &_size) const;
	void			operator+=(const FLOATSIZE &_size);
};

struct COLOR {
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	COLOR();
	COLOR(const unsigned char _red, const unsigned char _green, const unsigned char _blue);
	COLOR(const COLOR &_color);
	COLOR(const COLORREF &_color);

	void	operator=(const COLORREF &_color);

	bool	operator==(const COLOR &_color) const;
	bool	operator==(const COLORREF &_color) const;
	bool	operator!=(const COLOR &_color) const;
	bool	operator!=(const COLORREF &_color) const;
};