#pragma once

#include "../Bases/Unmoving.h"

#define PANAL_PROTOTYPE_KEY		"Panal_Prototype"
#define WIDEPANAL_POS			POSITION(0, 0)

class Panal : public Unmoving {
public:
	// Functions
	Panal();
	Panal(const Panal &_panal);
	virtual ~Panal();

	virtual Panal*	Clone()							{ return new Panal(*this); }
	bool			Clone_Init(const POSITION &_pos, const string &_texture_key, const TCHAR *_filename,
						 const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_base_key = IMAGE_PATH_KEY);
	bool			Clone_Init(const POSITION &_pos, const string &_texture_key);
	bool			Clone_Init(const POSITION &_pos, const POSITION & _image_lefttop, const FLOATSIZE & _size,
						const string &_texture_key, const TCHAR *_filename,
						const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_base_key = IMAGE_PATH_KEY);
	bool			Clone_Init(const POSITION &_pos, const POSITION & _image_lefttop, const FLOATSIZE & _size, const string &_texture_key);

	virtual bool	Init()							{ return TRUE; }
	virtual void	Input(const float _deltatime)	{ Unmoving::Input(_deltatime); }
	virtual void	Update(const float _deltatime)	{ Unmoving::Update(_deltatime); }
	virtual void	Late_Update()					{ Unmoving::Late_Update(); }
	virtual void	Render(const HDC &_hDC)			{ Unmoving::Render(_hDC); }
};