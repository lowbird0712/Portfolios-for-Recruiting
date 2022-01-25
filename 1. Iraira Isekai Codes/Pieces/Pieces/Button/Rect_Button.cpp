#pragma once

#include "Rect_Button.h"
#include "../../../Singleton/Core.h"
#include "../../../Singleton/Resource_Manager/Texture.h"
#include "../../../Singleton/Collision_Manager/Rect_Collider.h"

Rect_Button::Rect_Button() { }
Rect_Button::Rect_Button(const Rect_Button & _button) : Button(_button) { }
Rect_Button::~Rect_Button() { }

bool Rect_Button::Init() {
	if (!Button::Init())
		return FALSE;

	Rect_Collider *collider = Add_Collider<Rect_Collider>(RECT_BUTTON_CLICKRANGE_KEY);
	if (!collider) {
		SAFE_RELEASE(collider);
		return FALSE;
	}
	collider->Add_Function(CS_ENTER, (Button*)this, &Button::Mouse_Enter);
	collider->Add_Function(CS_LEAVE, (Button*)this, &Button::Mouse_Leave);

	SAFE_RELEASE(collider);

	return TRUE;
}

void Rect_Button::Render(const HDC & _hDC) {
	Button::Render(_hDC);

	// HBITMAP이 있어야 BitBlt 등의 함수를 사용할 수 있다고 생각하자
	// 클론을 생성할 때 복사 생성자에서 비트맵과 출력 크기에 대해 유의하자. 잘못하면 알파블렌딩이 안될 수도 있다.

	switch (m_state) {
	case BS_NORMAL:
		break;
	case BS_MOUSEON:
		if (m_mouseon_use) {
			GdiAlphaBlend(_hDC, m_pos.x, m_pos.y, m_size.x, m_size.y, m_alpha_DC,
				m_mouseon_image_lefttop.x, m_mouseon_image_lefttop.y, m_size.x, m_size.y, m_mouseon_blendfunc);
		}
		break;
	case BS_CLICKED:
		if (m_clicked_use) {
			GdiAlphaBlend(_hDC, m_pos.x, m_pos.y, m_size.x, m_size.y, m_alpha_DC,
				m_clicked_image_lefttop.x, m_clicked_image_lefttop.y, m_size.x, m_size.y, m_clicked_blendfunc);
		}
		break;
	}
}
