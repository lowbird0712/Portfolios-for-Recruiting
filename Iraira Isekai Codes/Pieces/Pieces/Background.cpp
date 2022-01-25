#pragma once

#include "Background.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Camera.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../../Singleton/Collision_Manager/Pixel_Collider.h"

Background::Background() : m_camera_pos(0.f, 0.f) { }
Background::Background(const Background &_background)
	: Unmoving(_background), m_camera_pos(_background.m_camera_pos) { }
Background::~Background() {
	DESTROY_SINGLE(Camera);
}

bool Background::Clone_Init(const string & _texture_key, const TCHAR * _filename, const bool _using_camera,
	const COLORREF & _color, const string & _base_key) {

	if (!Set_Texture(_texture_key, _filename, _color, _base_key))
		return FALSE;
	m_size = m_texture->Get_Frame_Size();

	if (_using_camera) {
		if (!GET_SINGLE(Camera)->Init(POSITION(0.f, 0.f), GET_RESOLUTION()))
			return FALSE;
	}
	else {
		const FLOATSIZE client_resolution(WINDOW_WIDTH, WINDOW_HEIGHT);
		if (m_size != client_resolution)
			return FALSE;
	}

	return TRUE;
}

bool Background::Pixel_Collider_Init(const TCHAR * _filename, const string & _coll_key) {
	Pixel_Collider *pixel = Add_Collider<Pixel_Collider>(_coll_key);
	if (!pixel->Set_Pixel_Info(_filename)) {
		SAFE_RELEASE(pixel);
		return FALSE;
	}

	SAFE_RELEASE(pixel);

	return TRUE;
}

void Background::Update(const float _deltatime) {
	if (SINGLE_DECLARED(Camera))
		m_camera_pos = GET_SINGLE(Camera)->Get_Pos();

	Unmoving::Update(_deltatime);
}

void Background::Render(const HDC & _hDC) {
	if (m_texture) {
		POSITION pos = m_pos - m_size * m_pivot;
		BitBlt(_hDC, pos.x, pos.y, GET_RESOLUTION().width, GET_RESOLUTION().height, m_texture->Get_MemDC(),
			m_camera_pos.x, m_camera_pos.y, SRCCOPY);
	}
}