#pragma once

#include "Camera.h"
#include "Input_Manager/Input_Manager.h"
#include "Scene_Manager/Scene_Manager.h"
#include "Scene_Manager/Scene.h"
#include "Resource_Manager/Texture.h"
#include "../Pieces/Bases/Object.h"
#include "../Pieces/Pieces/Background.h"

DEFINITION_SINGLE(Camera);

Camera::Camera() : m_pos(0.f, 0.f), m_client_resolution(0, 0), m_world_resolution(0, 0),
m_pivot(0.f, 0.f), m_target(NULL), m_scroll_speed(0.f) { }
Camera::~Camera() { SAFE_RELEASE(m_target); }

void Camera::Set_Target(Object * const _target) {
	SAFE_RELEASE(m_target);

	if (_target)
		_target->Add_Ref();
	m_target = _target;
}

void Camera::Release_Target() {
	SAFE_RELEASE(m_target);
}

string Camera::Get_Target_Strtag() const {
	return m_target->Get_Strtag();
}

bool Camera::Init(const POSITION & _pos, const RESOLUTION & _client_resolution, const PIVOT &_pivot) {
	Object *background = GET_SINGLE(Scene_Manager)->Find_Object(BACKGROUND_CLONE_KEY, BACKGROUND_LAYER_KEY);
	if (!background) {
		SAFE_RELEASE(background);
		return FALSE;
	}

	m_pos = _pos;
	m_client_resolution = _client_resolution;

	if (background->Get_Texture())
		m_world_resolution = background->Get_Texture()->Get_Frame_Size();

	SAFE_RELEASE(background);

	m_pivot = _pivot;
	m_scroll_speed = CAMERA_SCROLL_SPEED;

	if (!GET_SINGLE(Input_Manager)->Add_Key('A', CAMERA_LEFTMOVE_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('D', CAMERA_RIGHTMOVE_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('W', CAMERA_UPMOVE_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('S', CAMERA_DOWNMOVE_KEY))
		return FALSE;

	return TRUE;
}

void Camera::Input(const float _deltatime) {
	if (!m_target) {
		if (GET_KEYDOWNPRESSED(CAMERA_LEFTMOVE_KEY) && 0 < m_pos.x)
			m_pos.x += -m_scroll_speed * _deltatime;
		if (GET_KEYDOWNPRESSED(CAMERA_RIGHTMOVE_KEY) && m_pos.x + m_client_resolution.width < m_world_resolution.width)
			m_pos.x += m_scroll_speed * _deltatime;
		if (GET_KEYDOWNPRESSED(CAMERA_UPMOVE_KEY) && 0 < m_pos.y)
			m_pos.y += -m_scroll_speed * _deltatime;
		if (GET_KEYDOWNPRESSED(CAMERA_DOWNMOVE_KEY) && m_pos.y + m_client_resolution.height < m_world_resolution.height)
			m_pos.y += m_scroll_speed * _deltatime;
	}
}

void Camera::Update() {
	if (m_target) {
		POSITION target_pos = m_target->Get_Pos();

		const float left_area = m_client_resolution.width * m_pivot.x;
		const float right_area = m_client_resolution.width - left_area;
		const float top_area = m_client_resolution.height * m_pivot.y;
		const float bottom_area = m_client_resolution.height - top_area;

		if (left_area < target_pos.x && target_pos.x < m_world_resolution.width - right_area)
			m_pos.x = target_pos.x - left_area;
		if (top_area < target_pos.y && target_pos.y < m_world_resolution.height - bottom_area)
			m_pos.y = target_pos.y - top_area;
	}
	else {
		if (m_pos.x < 0)
			m_pos.x = 0;
		else if (m_pos.x + m_client_resolution.width > m_world_resolution.width)
			m_pos.x = m_world_resolution.width - m_client_resolution.width;

		if (m_pos.y < 0)
			m_pos.y = 0;
		else if (m_pos.y + m_client_resolution.height > m_world_resolution.height)
			m_pos.y = m_world_resolution.height - m_client_resolution.height;
	}
}