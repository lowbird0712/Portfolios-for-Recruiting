#pragma once

#include "Object.h"

class Unmoving : public Object {
public:
	// Functions
	Unmoving();
	Unmoving(const Unmoving &_unmoving);
	virtual ~Unmoving() = 0;

	virtual Unmoving*	Clone() = 0;

	virtual bool		Init() = 0;
	virtual void		Input(const float _deltatime)		{ Object::Input(_deltatime); }
	virtual void		Update(const float _deltatime)		{ Object::Update(_deltatime); }
	virtual void		Late_Update()						{ Object::Late_Update(); }
	virtual void		Render(const HDC &_hDC)				{ Object::Render(_hDC); }
};