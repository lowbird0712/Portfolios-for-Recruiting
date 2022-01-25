#pragma once

#include "Input_Manager.h"
#include "Mouse.h"

DEFINITION_SINGLE(Input_Manager);

Input_Manager::Input_Manager() : m_hWnd(NULL), m_creating_key(NULL), m_mouse(NULL) { }
Input_Manager::~Input_Manager() {
	SAFE_MAP_DELETE(m_keymap);
	SAFE_RELEASE(m_mouse);
}

PKEYINFO Input_Manager::Find_Key(const string & _keyinfo_key) const {
	unordered_map<string, PKEYINFO>::const_iterator it = m_keymap.find(_keyinfo_key);
	if (it == m_keymap.end())
		return NULL;

	return it->second;
}

bool Input_Manager::Get_Down(const string & _keyinfo_key) const {
	PKEYINFO key = Find_Key(_keyinfo_key);
	if (!key)
		return FALSE;

	return key->down;
}

bool Input_Manager::Get_Pressed(const string & _keyinfo_key) const {
	PKEYINFO key = Find_Key(_keyinfo_key);
	if (!key)
		return FALSE;

	return key->pressed;
}

bool Input_Manager::Get_Up(const string & _keyinfo_key) const {
	PKEYINFO key = Find_Key(_keyinfo_key);
	if (!key)
		return FALSE;

	return key->up;
}

Collider * Input_Manager::Get_Mouse_Clickrange() const {
	return m_mouse->Get_Mouse_Clickrange();
}

POSITION Input_Manager::Get_Mousepos() const {
	return m_mouse->Get_Pos();
}

FLOATSIZE Input_Manager::Get_Mousedir() const {
	return m_mouse->Get_Mousedir();
}

bool Input_Manager::Init(HWND _hWnd) {
	m_hWnd = _hWnd;

	// 마우스 클릭
	if (!Add_Key(VK_LBUTTON, MOUSE_LEFT_KEY))
		return FALSE;
	if (!Add_Key(VK_RBUTTON, MOUSE_RIGHT_KEY))
		return FALSE;

	m_mouse = new Mouse;
	if (!m_mouse->Init()) {
		SAFE_RELEASE(m_mouse);
		return FALSE;
	}

	return TRUE;
}

void Input_Manager::Update() {
	unordered_map<string, PKEYINFO>::iterator it;
	unordered_map<string, PKEYINFO>::iterator it_end = m_keymap.end();

	PKEYINFO key = NULL;

	for (it = m_keymap.begin(); it != it_end; it++) {
		int push_count = 0;
		key = it->second;

		for (size_t i = 0; i < key->button_vector.size(); i++) {
			if (GetAsyncKeyState(key->button_vector[i]) & 0x8000)
				push_count++;
		}

		if (push_count == key->button_vector.size()) {
			if (!key->down && !key->pressed)
				key->down = TRUE;
			else if (key->down && !key->pressed) {
				key->down = FALSE;
				key->pressed = TRUE;
			}
		}
		else {
			if (key->down || key->pressed) {
				key->down = FALSE;
				key->pressed = FALSE;
				key->up = TRUE;
			}
			else if (key->up)
				key->up = FALSE;
		}
	}

	m_mouse->Update(NULL);
}

void Input_Manager::Render(HDC _hDC) {
	m_mouse->Render(_hDC);
}