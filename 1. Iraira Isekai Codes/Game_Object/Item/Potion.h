#pragma once

#include "Finite.h"

class Potion : public Finite {

public:
	// ÇÔ¼ö
	Potion(const string & _name, const string & _early_clip_key,
		const int _usable_number, const POSITION & _icon_lefttop);
	virtual ~Potion();

	virtual void	Use()									{ Finite::Use(); }
};

