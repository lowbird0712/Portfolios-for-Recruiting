#pragma once

#include "Bag.h"
#include "../../../Player/Player.h"

Bag::Bag(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const int _barrier, const POSITION & _icon_lefttop)
	: Normal_Shield(_name, _early_clip_key, _later_clip_key, _barrier, _icon_lefttop) {}
Bag::~Bag() {}

void Bag::Use() {
	m_player->Get_Animation()->Stop_To(BAG_GUARDING_FRAMEPOS);

	Normal_Shield::Use();
}