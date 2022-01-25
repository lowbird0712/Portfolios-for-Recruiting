#pragma once

#include "Weapon.h"

class Normal_Weapon : public Weapon {

public:
	// ÇÔ¼ö
	Normal_Weapon(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop);
	virtual ~Normal_Weapon();

	virtual void	Use()								{ Weapon::Use(); }
};

