#pragma once

#include "Button.h"
#include "../../../Singleton/Core.h"
#include "../../../Singleton/Input_Manager/Input_Manager.h"
#include "../../../Singleton/Resource_Manager/Texture.h"

Button::Button() { }
Button::Button(const Button & _button) : Unmoving(_button), m_callback(_button.m_callback), m_state(BS_NORMAL),
	m_mouseon_blendfunc(_button.m_mouseon_blendfunc), m_clicked_blendfunc(_button.m_clicked_blendfunc) {

	m_alpha_DC = CreateCompatibleDC(_button.m_alpha_DC);
	const FLOATSIZE frame_size = m_texture->Get_Frame_Size();
	const HBITMAP alpha_bitmap = CreateCompatibleBitmap(_button.m_alpha_DC, frame_size.x, frame_size.y);
	m_alpha_bitmap_tmp = (HBITMAP)SelectObject(m_alpha_DC, alpha_bitmap);

	if (m_texture->Get_Colorkey() != TEXTURE_COLORKEY_UNUSE) {
		TransparentBlt(m_alpha_DC, 0, 0, frame_size.x, frame_size.y,
			m_texture->Get_MemDC(), 0, 0, frame_size.x, frame_size.y, m_texture->Get_Colorkey());
	}
	else
		BitBlt(m_alpha_DC, 0, 0, frame_size.x, frame_size.y, m_texture->Get_MemDC(), 0, 0, SRCCOPY);

	m_mouseon_image_lefttop = _button.m_mouseon_image_lefttop;
	m_clicked_image_lefttop = _button.m_clicked_image_lefttop;
	m_mouseon_use = _button.m_mouseon_use;
	m_clicked_use = _button.m_clicked_use;
}
Button::~Button() {
	DeleteObject(SelectObject(m_alpha_DC, m_alpha_bitmap_tmp));
	DeleteDC(m_alpha_DC);
}

void Button::Mouse_Enter(Collider * const _button_clickrange, Collider * const _mouse_clickrange, const float _deltatime) {
	((Button*)_button_clickrange->Get_Object())->Set_State(BS_MOUSEON);
}

void Button::Mouse_Leave(Collider * const _button_clickrange, Collider * const _mouse_clickrange, const float _deltatime) {
	((Button*)_button_clickrange->Get_Object())->Set_State(BS_NORMAL);
}

bool Button::Init() {
	if (!Set_Texture(BUTTONSET_IMAGE_KEY, BUTTONSET_IMAGE_FILENAME, RGB_MAGENTA))
		return FALSE;

	HDC core_DC = GET_HDC();
	m_alpha_DC = CreateCompatibleDC(core_DC);
	const FLOATSIZE frame_size = m_texture->Get_Frame_Size();
	HBITMAP alpha_bitmap = CreateCompatibleBitmap(core_DC, frame_size.x, frame_size.y);
	m_alpha_bitmap_tmp = (HBITMAP)SelectObject(m_alpha_DC, alpha_bitmap);

	if (m_texture->Get_Colorkey() != TEXTURE_COLORKEY_UNUSE) {
		TransparentBlt(m_alpha_DC, 0, 0, frame_size.x, frame_size.y,
			m_texture->Get_MemDC(), 0, 0, frame_size.x, frame_size.y, m_texture->Get_Colorkey());
	}
	else
		BitBlt(m_alpha_DC, 0, 0, frame_size.x, frame_size.y, m_texture->Get_MemDC(), 0, 0, SRCCOPY);

	m_mouseon_blendfunc.BlendOp = 0;
	m_mouseon_blendfunc.BlendFlags = 0;
	m_mouseon_blendfunc.AlphaFormat = AC_SRC_OVER;

	m_clicked_blendfunc.BlendOp = 0;
	m_clicked_blendfunc.BlendFlags = 0;
	m_clicked_blendfunc.AlphaFormat = AC_SRC_OVER;

	return TRUE;
}

void Button::Input(const float _deltatime) {
	Unmoving::Input(_deltatime);

	if (m_state == BS_MOUSEON && GET_KEYDOWN(MOUSE_LEFT_KEY)) {
		m_state = BS_CLICKED;
	}
	else if (m_state == BS_CLICKED && GET_KEYUP(MOUSE_LEFT_KEY)) {
		m_state = BS_MOUSEON;

		if (m_callback)
			m_callback();
	}
}

void Button::Late_Update() {
	Unmoving::Late_Update();

	Collider *clickrange = GET_MOUSE_CLICKRANGE();

	// Button 객체의 m_collist의 크기는 항상 1이다.
	list<Collider*>::const_iterator it = m_collist.begin();

	if ((*it)->Collision(clickrange)) {
		if (!(*it)->Check_Colliding_List(clickrange)) {
			(*it)->Add_Collider(clickrange);
			(clickrange)->Add_Collider(*it);

			(*it)->Call_function(CS_ENTER, clickrange, NULL);
			(clickrange)->Call_function(CS_ENTER, *it, NULL);
		}
		else {
			(*it)->Call_function(CS_STAY, clickrange, NULL);
			(clickrange)->Call_function(CS_STAY, *it, NULL);
		}
	}
	else {
		if ((*it)->Check_Colliding_List(clickrange)) {
			(*it)->Erase_Collider(clickrange);
			(clickrange)->Erase_Collider(*it);

			(*it)->Call_function(CS_LEAVE, clickrange, NULL);
			(clickrange)->Call_function(CS_LEAVE, *it, NULL);
		}
	}
}