#pragma once

#include "Collider.h"

class Circle_Collider : public Collider {
	// Variables
	FLOATCIRCLE			m_origin_circle;
	FLOATCIRCLE			m_circle;

	// Functions
	bool				Rect_Collision(const FLOATRECT &_rect)										{ return RectCircle_Collision(_rect, m_circle); }
	bool				Circle_Collision(const FLOATCIRCLE &_circle)								{ return CircleCircle_Collision(m_circle, _circle); }
	bool				Pixel_Collision(const vector<COLOR> &_pixel_vector,
							const size_t _width, const size_t _height, const COLOR &_color)			{ return FALSE; }
	bool				Point_Collision(const POSITION &_pos)										{ return PointCircle_Collision(_pos, m_circle); }
public:
	// Functions
	Circle_Collider();
	Circle_Collider(const Circle_Collider &_circle);
	virtual ~Circle_Collider();

	virtual Collider*	Clone()																		{ return new Circle_Collider(*this); }

	void				Set_Origin_Circle(const FLOATCIRCLE &_circle)								{ m_origin_circle = _circle; }
	void				Set_Origin_Circle(const float _x, const float _y, const float _radius)
																									{ m_origin_circle = FLOATCIRCLE(_x, _y, _radius); }
	void				Set_Circle();

	FLOATCIRCLE			Get_Origin_Circle() const													{ return m_origin_circle; }
	FLOATCIRCLE			Get_Circle() const															{ return m_circle; }

	virtual void		Input();
	virtual void		Update();
	virtual bool		Collision(Collider * const _collider);
	virtual void		Late_Update();
	virtual void		Render(const HDC &_hDC);
};