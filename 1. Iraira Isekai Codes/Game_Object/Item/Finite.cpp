#pragma once

#include "Finite.h"
#include "../Player/Player.h"

Finite::Finite(const string & _name, const string & _early_clip_key, const EQUIPMENT_TYPE _type,
	const int _usable_number, const POSITION & _icon_lefttop)
	: Equipment(_name, _early_clip_key, FINITE_LATER_CLIP_KEY, _type, _icon_lefttop) {

	m_usable_number = _usable_number;
}
Finite::~Finite() {}

void Finite::Use() {
	Equipment::Use();

	m_usable_number--;
	m_player->Finish_Using();
}