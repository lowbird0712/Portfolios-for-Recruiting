#pragma once

#include "Mouse.h"
#include "../Resource_Manager/Resource_Manager.h"
#include "../Camera.h"
#include "../Resource_Manager/Texture.h"
#include "../Collision_Manager/Point_Collider.h"

Mouse::Mouse() : m_mousedir(0.f, 0.f) { }
Mouse::Mouse(const Mouse & _mouse) : Unmoving(_mouse), m_mousedir(0.f, 0.f) { }
Mouse::~Mouse() { }

bool Mouse::Init() {
	if (!Set_Texture(MOUSE_IMAGE_KEY, MOUSE_IMAGE_FILENAME, RGB_BLACK))
		return FALSE;
	m_size = m_texture->Get_Frame_Size();
	m_pivot = MOUSE_PIVOT;

	Point_Collider *collider = Add_Collider<Point_Collider>(MOUSE_CLICKRANGE_KEY);
	if (!collider) {
		SAFE_RELEASE(collider);
		return FALSE;
	}
	collider->Set_Origin_Pos(Get_Pos());

	SAFE_RELEASE(collider);

	return TRUE;
}

void Mouse::Update(const float _deltatime) {
	Unmoving::Update(_deltatime);

	POINT pos;
	GetCursorPos(&pos); // 마우스 커서의 위치를 매개변수로 들어온 주소에 넣어준다.
	ScreenToClient(GET_HWND(), &pos);
	m_mousedir.x = pos.x - m_pos.x;
	m_mousedir.y = pos.y - m_pos.y;
	m_pos = pos;
}

void Mouse::Render(const HDC & _hDC) {
	if (SINGLE_DECLARED(Camera))
		m_pos += GET_SINGLE(Camera)->Get_Pos();

	(*m_collist.begin())->Late_Update();
	Unmoving::Render(_hDC);

	if (SINGLE_DECLARED(Camera))
		m_pos -= GET_SINGLE(Camera)->Get_Pos();
}