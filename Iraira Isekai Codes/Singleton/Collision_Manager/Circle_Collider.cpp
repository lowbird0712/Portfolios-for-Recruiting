#pragma once

#include "Circle_Collider.h"
#include "../Camera.h"
#include "../../Pieces/Bases/Object.h"
#include "Rect_Collider.h"
#include "Pixel_Collider.h"
#include "Point_Collider.h"

Circle_Collider::Circle_Collider() : m_origin_circle(FLOATCIRCLE()), m_circle(FLOATCIRCLE()) { m_type = CT_CIRCLE; }
Circle_Collider::Circle_Collider(const Circle_Collider & _circle)
	: Collider(_circle), m_origin_circle(_circle.m_origin_circle), m_circle(FLOATCIRCLE()) { }
Circle_Collider::~Circle_Collider() { }

void Circle_Collider::Set_Circle() {
	POSITION pos = m_object->Get_Pos();
	m_circle.x = pos.x;
	m_circle.y = pos.y;
}

void Circle_Collider::Input() {
	Set_Circle();
}

void Circle_Collider::Update() {
	Set_Circle();
}

bool Circle_Collider::Collision(Collider * const _collider) {
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
		return Point_Collision(((Point_Collider*)_collider)->Get_Pos());
	case CT_PIXEL:
		return Pixel_Collision(((Pixel_Collider*)_collider)->Get_Pixel_Vector(),
			((Pixel_Collider*)_collider)->Get_Width(), ((Pixel_Collider*)_collider)->Get_Height(), ((Pixel_Collider*)_collider)->Get_Color());
	case CT_END:
		break;
	}

	return FALSE;
}

void Circle_Collider::Late_Update() {
	Set_Circle();
}

void Circle_Collider::Render(const HDC & _hDC) {
	POSITION camera_pos;

	if (SINGLE_DECLARED(Camera))
		camera_pos = GET_SINGLE(Camera)->Get_Pos();

	FLOATRECT rect = {
		m_circle.x - m_circle.radius - camera_pos.x,
		m_circle.y - m_circle.radius - camera_pos.y,
		m_circle.x + m_circle.radius - camera_pos.x,
		m_circle.y + m_circle.radius - camera_pos.y,
	};
	Ellipse(_hDC, rect.left, rect.top, rect.right, rect.bottom);
}