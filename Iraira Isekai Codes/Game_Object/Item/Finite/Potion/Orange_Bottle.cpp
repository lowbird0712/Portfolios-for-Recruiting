#pragma once

#include "Orange_Bottle.h"
#include "../../../Player/Player.h"

Orange_Bottle::Orange_Bottle(const string & _name, const string & _early_clip_key,
	const int _usable_number, const POSITION & _icon_lefttop)
	: Potion(_name, _early_clip_key, _usable_number, _icon_lefttop) {}
Orange_Bottle::~Orange_Bottle() {}

void Orange_Bottle::Use() {
	Potion::Use();

	m_player->Heal(30);
}
