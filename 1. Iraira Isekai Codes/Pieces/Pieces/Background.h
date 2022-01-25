#pragma once

#include "../../Pieces/Bases/Unmoving.h"

#define BACKGROUND_PROTOTYPE_KEY				"Background_Prototype"
#define BACKGROUND_CLONE_KEY					"Background_Clone"		// 한 장면에 백그라운드는 하나이므로 모든 씬의 클론들에 공통으로 사용된다
#define BACKGROUND_PIXELCOLL_KEY				"Background_Pixelcoll"	// 마찬가지 이유로 모든 씬의 백그라운드 클론의 픽셀 충돌체에 공통으로 사용된다

class Texture;
class Tile;

// 배경 이미지에 픽셀 충돌을 적용할 경우에 사용하자.
// 그렇지 않다면 Panal 객체를 생성하자.
class Background : public Unmoving {
	// Variables
	POSITION			m_camera_pos;
public:
	// Functions
	Background();
	Background(const Background &_background);
	virtual ~Background();

	virtual Unmoving*	Clone()							{ return new Background(*this); }
	bool				Clone_Init(const string &_texture_key, const TCHAR *_filename, const bool _using_camera,
							const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_base_key = IMAGE_PATH_KEY);

	bool				Pixel_Collider_Init(const TCHAR *_filename, const string &_coll_key);

	virtual bool		Init()							{ return TRUE; }
	virtual void		Input(const float _deltatime)	{ Unmoving::Input(_deltatime); }
	virtual void		Update(const float _deltatime);
	virtual void		Late_Update()					{ Unmoving::Late_Update(); }
	virtual void		Render(const HDC &_hDC);
};