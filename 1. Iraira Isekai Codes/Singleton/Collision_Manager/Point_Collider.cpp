#pragma once

#include "Point_Collider.h"
#include "../Camera.h"
#include "../../Pieces/Bases/Object.h"
#include "Rect_Collider.h"
#include "Circle_Collider.h"
#include "Pixel_Collider.h"

Point_Collider::Point_Collider() : m_origin_pos(0.f, 0.f), m_pos(0.f, 0.f) { m_type = CT_POINT; }
Point_Collider::Point_Collider(const Point_Collider & _point) : Collider(_point), m_origin_pos(_point.m_origin_pos), m_pos(0.f, 0.f) { }
Point_Collider::~Point_Collider() { }

bool Point_Collider::Pixel_Collision(Pixel_Collider * const _pixel_vector,
	const size_t _width, const size_t _height, const COLOR & _color) {

	size_t index = m_pos.y * _width + m_pos.x;
	vector<COLOR> pixel_vector = _pixel_vector->Get_Pixel_Vector();
	COLOR pixel = pixel_vector[index];

	if (pixel == _color) {
		_pixel_vector->Set_Topline(m_pos.y);
		return TRUE;
	}
	else
		return FALSE;
}

void Point_Collider::Set_Pos() {
	POSITION pos = m_object->Get_Pos();
	m_pos.x = pos.x + m_origin_pos.x;
	m_pos.y = pos.y + m_origin_pos.y;
}

void Point_Collider::Input() {
	Set_Pos();
}

void Point_Collider::Update() {
	Set_Pos();
}

bool Point_Collider::Collision(Collider * const _collider) {
	switch (_collider->Get_Type()) {
	case CT_NONE:
		break;
	case CT_RECT:
		return Rect_Collision(((Rect_Collider*)_collider)->Get_Rect());
	case CT_CIRCLE:
		return Circle_Collision(((Circle_Collider*)_collider)->Get_Circle());
	case CT_LINE:
		break;
	case CT_POINT:
		break;
	case CT_PIXEL:
		return Pixel_Collision((Pixel_Collider*)_collider, ((Pixel_Collider*)_collider)->Get_Width(),
			((Pixel_Collider*)_collider)->Get_Height(), ((Pixel_Collider*)_collider)->Get_Color());
	case CT_END:
		break;
	}

	return FALSE;
}

void Point_Collider::Late_Update() {
	Set_Pos();
}

void Point_Collider::Render(const HDC & _hDC) {
	POSITION camera_pos;

	if (SINGLE_DECLARED(Camera))
		camera_pos = GET_SINGLE(Camera)->Get_Pos();

	Ellipse(_hDC, m_pos.x - camera_pos.x - 5, m_pos.y - camera_pos.y - 5, m_pos.x - camera_pos.x + 5, m_pos.y - camera_pos.y + 5);
}