#pragma once

#include "../../Game.h"
#include "../Path_Manager.h"

#define TEXTURE_COLORKEY_UNUSE			RGB(-10, -10, -10)
#define RGB_MAGENTA						RGB(255, 0, 255)
#define RGB_MAGENTA2					RGB(242, 0, 255)
#define COLOR_MAGENTA					COLOR(255, 0, 255)
#define RGB_BLACK						RGB(0, 0, 0)
#define RGB_WHITE						RGB(255, 255, 255)
#define RGB_RED							RGB(255, 0, 0)

#define BACKBUFFER_TEXTURE_KEY			"Backbuffer_Texture"
#define BACKBUFFER_TEXTURE_FILENAME		TEXT("Backbuffer.bmp")

class Texture;

class Resource_Manager {
	DECLARE_SINGLE(Resource_Manager)
private:
	// Variables
	unordered_map<string, Texture *>	m_texturemap;

	HINSTANCE							m_hInst;
	HDC									m_hDC;
	Texture*							m_backbuffer;
public:
	// Functions
	// Object::m_texture은 Resource_Manager::Load_Texture의 반환값을 필요로 하기 때문에, 반환형은 Texture*이다.
	Texture*		Load_Texture(const string &_texture_key, const TCHAR * const _filename,
		const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_path_key = IMAGE_PATH_KEY);
	Texture*		Find_Texture(const string &_texture_key);

	Texture*		Get_BackBuffer() const;

	bool			Init(HINSTANCE _hInst, HDC _hDC);
};