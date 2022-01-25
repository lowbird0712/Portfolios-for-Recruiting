#pragma once

#include "Texture.h"

Texture::Texture() : m_strtag(""), m_hmemDC(NULL), m_hBitmap(NULL), m_hBitmap_tmp(NULL),
	m_info(BITMAP()), m_colorkey(NULL) { }
Texture::~Texture() {
	SelectObject(m_hmemDC, m_hBitmap_tmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hmemDC);
}

bool Texture::Load_Texture(HINSTANCE _hInst, HDC _hDC, const string &_texture_key, const TCHAR * const _filename,
	const COLORREF &_color, const string &_path_key) {

	m_strtag = _texture_key;
	m_hmemDC = CreateCompatibleDC(_hDC);

	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_path_key);
	if (!path)
		return FALSE;
	tstring strpath = path;
	strpath += _filename;

	m_hBitmap = (HBITMAP)LoadImage(_hInst, strpath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!m_hBitmap)
		return FALSE;
	m_hBitmap_tmp = (HBITMAP)SelectObject(m_hmemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_info), &m_info);

	m_colorkey = _color;

	return TRUE;
}