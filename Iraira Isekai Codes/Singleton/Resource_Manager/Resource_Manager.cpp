#pragma once

#include "Resource_Manager.h"
#include "Texture.h"

DEFINITION_SINGLE(Resource_Manager);

Resource_Manager::Resource_Manager() : m_hInst(NULL), m_hDC(NULL), m_backbuffer(NULL) { }
Resource_Manager::~Resource_Manager() {
	SAFE_MAP_RELEASE(m_texturemap);
	SAFE_RELEASE(m_backbuffer);
}

Texture* Resource_Manager::Load_Texture(const string &_texture_key, const TCHAR * const _filename,
	const COLORREF &_color, const string &_path_key) {
	Texture *texture = Find_Texture(_texture_key);

	if (texture)
		return texture;
	else {
		texture = new Texture;

		if (!texture->Load_Texture(m_hInst, m_hDC, _texture_key, _filename, _color, _path_key)) {
			SAFE_RELEASE(texture);
			return NULL;
		}

		texture->Add_Ref();
		m_texturemap.insert(make_pair(_texture_key, texture));

		return texture;
	}
}

Texture* Resource_Manager::Find_Texture(const string &_texture_key) {
	unordered_map<string, Texture *>::iterator it = m_texturemap.find(_texture_key);
	if (it == m_texturemap.end())
		return NULL;

	it->second->Add_Ref();
	return it->second;
}

Texture* Resource_Manager::Get_BackBuffer() const {
	m_backbuffer->Add_Ref();
	return m_backbuffer;
}

bool Resource_Manager::Init(HINSTANCE _hInst, HDC _hDC) {
	m_hInst = _hInst;
	m_hDC = _hDC;

	m_backbuffer = Load_Texture(BACKBUFFER_TEXTURE_KEY, BACKBUFFER_TEXTURE_FILENAME, TEXTURE_COLORKEY_UNUSE, SYSTEM_PATH_KEY);
	SetBkMode(m_backbuffer->Get_MemDC(), TRANSPARENT);

	if (!m_backbuffer) {
		SAFE_RELEASE(m_backbuffer);
		return FALSE;
	}

	return TRUE;
}