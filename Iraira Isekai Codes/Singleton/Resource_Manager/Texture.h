#pragma once

#include "../../Pieces/Bases/Reference.h"
#include "../Path_Manager.h"
#include "../Resource_Manager/Resource_Manager.h"

class Texture : public Reference {
	// Veriables
	string		m_strtag;
	HDC			m_hmemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmap_tmp;
	BITMAP		m_info;
	COLORREF	m_colorkey;
public:
	// Functions
	Texture();
	virtual ~Texture();

	bool		Load_Texture(HINSTANCE _hInst, HDC _hDC, const string &_texture_key, const TCHAR * const _filename,
					const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_path_key = IMAGE_PATH_KEY);

	COLORREF	Get_Colorkey() const { return m_colorkey; }
	HDC			Get_MemDC() const { return m_hmemDC; }
	FLOATSIZE	Get_Frame_Size() const { return FLOATSIZE(m_info.bmWidth, m_info.bmHeight); }
};