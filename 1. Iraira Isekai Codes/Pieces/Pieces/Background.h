#pragma once

#include "../../Pieces/Bases/Unmoving.h"

#define BACKGROUND_PROTOTYPE_KEY				"Background_Prototype"
#define BACKGROUND_CLONE_KEY					"Background_Clone"		// �� ��鿡 ��׶���� �ϳ��̹Ƿ� ��� ���� Ŭ�е鿡 �������� ���ȴ�
#define BACKGROUND_PIXELCOLL_KEY				"Background_Pixelcoll"	// �������� ������ ��� ���� ��׶��� Ŭ���� �ȼ� �浹ü�� �������� ���ȴ�

class Texture;
class Tile;

// ��� �̹����� �ȼ� �浹�� ������ ��쿡 �������.
// �׷��� �ʴٸ� Panal ��ü�� ��������.
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