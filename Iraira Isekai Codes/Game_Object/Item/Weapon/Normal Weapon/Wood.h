#pragma once

#include "../../Normal_Weapon.h"

#define WOOD_KEY						"Wood"
#define WOOD_EARLY_CLIP_KEY				"Wood_Early_Clip_Key"
#define WOOD_LATER_CLIP_KEY				"Wood_Later_Clip_Key"
#define WOOD_SPRITE_KEY					"Wood_Sprite_Key"
#define WOOD_SPRITE_FILENAME			TEXT("Player_WoodKnifeBaguette.bmp")

#define WOOD_ICON_IMAGE_LEFTTOP			POSITION(-1, -1)

class Wood : public Normal_Weapon {

public:
	Wood(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop);
	~Wood();

	void	Use() { Normal_Weapon::Use(); }
};