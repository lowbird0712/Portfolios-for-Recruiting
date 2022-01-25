#pragma once

#include "Collider.h"

class Pixel_Collider;

class Point_Collider : public Collider {
	// Variables
	POSITION	m_origin_pos;
	POSITION	m_pos;

	// Functions
	bool				Rect_Collision(const FLOATRECT &_rect)					{ return PointRect_Collision(m_pos, _rect); }
	bool				Circle_Collision(const FLOATCIRCLE &_circle)			{ return PointCircle_Collision(m_pos, _circle); }
	bool				Pixel_Collision(Pixel_Collider * const _pixel_vector,
							const size_t _width, const size_t _height, const COLOR &_color);
public:
	// Functions
	Point_Collider();
	Point_Collider(const Point_Collider &_point);
	virtual ~Point_Collider();

	Point_Collider*	Clone()														{ return new Point_Collider(*this); }

	void				Set_Origin_Pos(const float _x, const float _y)			{ m_origin_pos = POSITION(_x, _y); }
	void				Set_Origin_Pos(const POSITION &_pos)					{ m_origin_pos = _pos; }
	void				Set_Pos();
	POSITION			Get_Pos() const											{ return m_pos; }

	virtual void		Input();
	virtual void		Update();
	virtual bool		Collision(Collider * const _collider);
	virtual void		Late_Update();
	virtual void		Render(const HDC &_hDC);
};