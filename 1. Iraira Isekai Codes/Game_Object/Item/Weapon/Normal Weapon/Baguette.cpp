#pragma once

#include "Baguette.h"
#include "../../../Player/Player.h"

Baguette::Baguette(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop)
	: Normal_Weapon(_name, _early_clip_key, _later_clip_key, _cooltime, _minatk, _maxatk, _icon_lefttop) {}
Baguette::~Baguette() {}