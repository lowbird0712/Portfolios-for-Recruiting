#pragma once

#include "../../Normal_Weapon.h"

#define KNIFE_KEY						"Knife"
#define KNIFE_EARLY_CLIP_KEY			"Knife_Early_Clip_Key"
#define KNIFE_LATER_CLIP_KEY			"Knife_Later_Clip_Key"
#define KNIFE_SPRITE_KEY				"Knife_Sprite_Key"
#define KNIFE_SPRITE_FILENAME			TEXT("Player_WoodKnifeBaguette.bmp")

#define KNIFE_ICON_IMAGE_LEFTTOP		POSITION(60, 0)

class Knife : public Normal_Weapon {

public:
	Knife(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop);
	~Knife();

	void	Use() { Normal_Weapon::Use(); }
};