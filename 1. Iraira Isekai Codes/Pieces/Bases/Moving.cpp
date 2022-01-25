#include "Moving.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Camera.h"

Moving::Moving() : m_angle(OBJECT_ANGLE_UNUSE), m_speed(0.f), m_vertical_speed(OBJECT_VSPEED_UNUSE) { }
Moving::Moving(const Moving & _moving) : Object(_moving), m_angle(OBJECT_ANGLE_INIT), m_speed(_moving.m_speed) {
	if (_moving.m_angle == OBJECT_ANGLE_UNUSE)
		m_angle = OBJECT_ANGLE_UNUSE;
	else
		m_angle = OBJECT_ANGLE_INIT;

	if (_moving.m_vertical_speed == OBJECT_VSPEED_UNUSE)
		m_vertical_speed = OBJECT_VSPEED_UNUSE;
	else
		m_vertical_speed = OBJECT_VSPEED_INIT;
}
Moving::~Moving() { }

void Moving::Set_Gravity(const bool _gravity) {
	if (!_gravity)
		m_vertical_speed = OBJECT_VSPEED_UNUSE;
	else if (m_vertical_speed != OBJECT_VSPEED_UNUSE)
		return;
	else
		m_vertical_speed = OBJECT_VSPEED_INIT;
}

float Moving::Get_Needed_Vertical_Speed(const float _needed_amount, const NEEDED_SPEED_OPTION _option) const {
	float plus_speed;

	switch (_option) {
	case NSO_HEIGHT:
		plus_speed = sqrt(2 * GRAVITY * _needed_amount);
		break;
	case NSO_TIME:
		plus_speed = GRAVITY * _needed_amount;
		break;
	}

	return -plus_speed;
}

void Moving::Update(const float _deltatime) {
	Object::Update(_deltatime);

	if (m_vertical_speed != OBJECT_VSPEED_UNUSE) {
		m_vertical_speed += GRAVITY * _deltatime;
		m_pos.y += m_vertical_speed * _deltatime;

		POSITION pos = m_pos - Get_Pivotsize();
		RESOLUTION world_resolution = GET_SINGLE(Core)->Get_Resolution();

		if (SINGLE_DECLARED(Camera))
			world_resolution = GET_SINGLE(Camera)->Get_World_Resolution();

		if (pos.y < 0)
			m_pos.y = Get_Pivotsize().y;
		else if (pos.y + m_size.y > world_resolution.height)
			m_pos.y = world_resolution.height - m_size.y + Get_Pivotsize().y;
	}
}