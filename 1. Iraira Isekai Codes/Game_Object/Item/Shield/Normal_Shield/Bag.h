#pragma once

#include "../../Normal_Shield.h"

#define BAG_KEY						"Bag"
#define BAG_EARLY_CLIP_KEY			"Bag_Early_Clip_Key"
#define BAG_LATER_CLIP_KEY			"Bag_Later_Clip_Key"
#define BAG_EARLY_SPRITE_KEY		"Bag_Early_Sprite_Key"
#define BAG_EARLY_SPRITE_FILENAME	TEXT("Player_Bag_Up.bmp")
#define BAG_LATER_SPRITE_KEY		"Bag_later_Sprite_Key"
#define BAG_LATER_SPRITE_FILENAME	TEXT("Player_Bag_Unuse.bmp")

#define BAG_ICON_IMAGE_LEFTTOP		POSITION(0, 0)
#define BAG_GUARDING_FRAMEPOS		POSITION(8, 0)

class Bag : public Normal_Shield {

public:
	// ÇÔ¼ö
	Bag(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const int _barrier, const POSITION & _icon_lefttop);
	~Bag();

	void	Use();
	void	Unuse() { Shield::Unuse(); }
};