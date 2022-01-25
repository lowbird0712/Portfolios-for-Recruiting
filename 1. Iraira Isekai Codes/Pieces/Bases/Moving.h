#pragma once

#include "Object.h"

#define PI								3.141592f
#define GRAVITY							980.f

#define OBJECT_ANGLE_UNUSE				-1.f
#define OBJECT_ANGLE_INIT				0.f
#define OBJECT_VSPEED_UNUSE				-1.f
#define OBJECT_VSPEED_INIT				0.f

enum NEEDED_SPEED_OPTION {
	NSO_HEIGHT,
	NSO_TIME
};

class Moving : public Object {
protected:
	// Variables
	float		m_angle;
	float		m_speed;
	float		m_vertical_speed;
public:
	// Functions
	Moving();
	Moving(const Moving &_moving);
	virtual ~Moving() = 0;

	virtual Moving* Clone() = 0;

	void			Move(const float _x, const float _y, const float _deltatime)	{ m_pos.x += _x * _deltatime; m_pos.y += _y * _deltatime; }
	void			Move(const FLOATSIZE &_size, const float _deltatime)			{ m_pos.x += _size.x * _deltatime; m_pos.y += _size.y * _deltatime; }
	void			Angle_Move()													{ m_pos.x += m_speed * cosf(m_angle); m_pos.y -= m_speed * sinf(m_angle); }
	void			Angle_Move(const float _deltatime)								{ m_pos.x += m_speed * cosf(m_angle) * _deltatime; m_pos.y -= m_speed * sinf(m_angle) * _deltatime; }

	void			Set_Angle(const float _angle)									{ m_angle = _angle; }
	void			Set_Speed(const float _speed)									{ m_speed = _speed; }
	void			Set_Gravity(const bool _gravity);
	void			Set_Vertical_Speed(const float _vertical_speed)					{ m_vertical_speed = _vertical_speed; }
	void			Reset_Vertical_Speed()											{ m_vertical_speed = OBJECT_VSPEED_INIT; }
	float			Get_Angle() const												{ return m_angle; }
	float			Get_Speed() const												{ return m_speed; }
	float			Get_Needed_Vertical_Speed(const float _needed_amount, const NEEDED_SPEED_OPTION _option) const;
	float			Get_Vertical_Speed() const										{ return m_vertical_speed; }

	virtual bool	Init() = 0;
	virtual void	Input(const float _deltatime)									{ Object::Input(_deltatime); }
	virtual void	Update(const float _deltatime);
	virtual void	Late_Update()													{ Object::Late_Update(); }
	virtual void	Render(const HDC &_hDC)											{ Object::Render(_hDC); }
};