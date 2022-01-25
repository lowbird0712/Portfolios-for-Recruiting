#pragma once

#include "Collider.h"

class Pixel_Collider;

class Rect_Collider : public Collider {
	// Variables
	FLOATRECT	m_origin_rect;
	FLOATRECT	m_rect;

	// Functions
	bool				Rect_Collision(const FLOATRECT &_rect)			{ return RectRect_Collision(m_rect, _rect); }
	bool				Circle_Collision(const FLOATCIRCLE &_circle)	{ return RectCircle_Collision(m_rect, _circle); }
	bool				Pixel_Collision(Pixel_Collider * const _pixel_collider,
							const size_t _width, const size_t _height, const COLOR &_color);
	bool				Point_Collision(const POSITION &_pos)			{ return PointRect_Collision(_pos, m_rect); }
public:
	// Functions
	Rect_Collider();
	Rect_Collider(const Rect_Collider &_rect);
	virtual ~Rect_Collider();

	virtual Collider*	Clone()										{ return new Rect_Collider(*this); }

	void				Set_Origin_Rect(const FLOATRECT &_rect)		{ m_origin_rect = _rect; }
	void				Set_Origin_Rect(const float _left, const float _top, const float _right, const float _bottom)
																	{ m_origin_rect = FLOATRECT(_left, _top, _right, _bottom); }
	void				Set_Rect();

	FLOATRECT			Get_Origin_Rect() const						{ return m_origin_rect; }
	FLOATRECT			Get_Rect() const							{ return m_rect; }

	virtual void		Input();
	virtual void		Update();
	virtual bool		Collision(Collider * const _collider);
	virtual void		Late_Update();
	virtual void		Render(const HDC &_hDC);
};