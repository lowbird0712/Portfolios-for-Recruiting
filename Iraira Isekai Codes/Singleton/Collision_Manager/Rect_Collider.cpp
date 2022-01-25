#pragma once

#include "Rect_Collider.h"
#include "../Camera.h"
#include "../../Pieces/Bases/Object.h"
#include "Circle_Collider.h"
#include "Pixel_Collider.h"
#include "Point_Collider.h"

Rect_Collider::Rect_Collider() : m_origin_rect(FLOATRECT()), m_rect(FLOATRECT()) { m_type = CT_RECT; }
Rect_Collider::Rect_Collider(const Rect_Collider & _rect)
	: Collider(_rect), m_origin_rect(_rect.m_origin_rect), m_rect(FLOATRECT()) { }
Rect_Collider::~Rect_Collider() { }

bool Rect_Collider::Pixel_Collision(Pixel_Collider * const _pixel_collider,
	const size_t _width, const size_t _height, const COLOR &_color) {

	size_t start_x, end_x;
	size_t start_y, end_y;
	const vector<COLOR> pixel_vector = _pixel_collider->Get_Pixel_Vector();

	start_x = m_rect.left < 0 ? 0 : m_rect.left;
	end_x = m_rect.right > _width ? _width : m_rect.right;
	start_y = m_rect.top < 0 ? 0 : m_rect.top;
	end_y = m_rect.bottom > _height ? _height : m_rect.bottom;

	for (size_t i = start_y; i <= end_y; i++) {
		for (size_t j = start_x; j <= end_x; j++) {
			size_t index = i * _width + j;
			COLOR pixel = pixel_vector[index];
			if (pixel == _color) {
				_pixel_collider->Set_Topline(i);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void Rect_Collider::Set_Rect() {
	POSITION pos = m_object->Get_Pos();
	m_rect.left = pos.x + m_origin_rect.left;
	m_rect.top = pos.y + m_origin_rect.top;
	m_rect.right = pos.x + m_origin_rect.right;
	m_rect.bottom = pos.y + m_origin_rect.bottom;
}

void Rect_Collider::Input() {
	Set_Rect();
}

void Rect_Collider::Update() {
	Set_Rect();
}

bool Rect_Collider::Collision(Collider * const _collider) {
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
		return Pixel_Collision((Pixel_Collider*)_collider, ((Pixel_Collider*)_collider)->Get_Width(),
			((Pixel_Collider*)_collider)->Get_Height(), ((Pixel_Collider*)_collider)->Get_Color());
	case CT_END:
		break;
	}

	return FALSE;
}

void Rect_Collider::Late_Update() {
	Set_Rect();
}

void Rect_Collider::Render(const HDC & _hDC) {
	POSITION camera_pos;

	if (SINGLE_DECLARED(Camera))
		camera_pos = GET_SINGLE(Camera)->Get_Pos();

	Rectangle(_hDC, m_rect.left - camera_pos.x, m_rect.top - camera_pos.y,
		m_rect.right - camera_pos.x, m_rect.bottom - camera_pos.y);
}