#pragma once

#include "../../Potion.h"

#define ORANGE_BOTTLE_KEY					"Orange_Bottle"
#define ORANGE_BOTTLE_EARLY_CLIP_KEY		"Orange_Bottle_Early_Clip_Key"
#define ORANGE_BOTTLE_SPRITE_KEY			"Orange_Bottle_Sprite_Key"
#define ORANGE_BOTTLE_SPRITE_FILENAME		TEXT("Player_Orange_Bottle.bmp")

#define ORANGE_BOTTLE_ICON_IMAGE_LEFTTOP	POSITION(180, 0)

class Orange_Bottle : public Potion {

public:
	// ÇÔ¼ö
	Orange_Bottle(const string & _name, const string & _early_clip_key,
		const int _usable_number, const POSITION & _icon_lefttop);
	~Orange_Bottle();

	void	Use();
};