#pragma once

#include "../../Normal_Weapon.h"

#define BAGUETTE_KEY						"Baguette"
#define BAGUETTE_EARLY_CLIP_KEY				"Baguette_Early_Clip_Key"
#define BAGUETTE_LATER_CLIP_KEY				"Baguette_Later_Clip_Key"
#define BAGUETTE_SPRITE_KEY					"Baguette_Sprite_Key"
#define BAGUETTE_SPRITE_FILENAME			TEXT("Player_WoodKnifeBaguette.bmp")

#define BAGUETTE_ICON_IMAGE_LEFTTOP			POSITION(120, 0)

class Baguette : public Normal_Weapon {

public:
	Baguette(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop);
	~Baguette();

	void	Use() { Normal_Weapon::Use(); }
};