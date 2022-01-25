#include "Potion.h"

Potion::Potion(const string & _name, const string & _early_clip_key,
	const int _usable_number, const POSITION & _icon_lefttop)
	: Finite(_name, _early_clip_key, ET_POTION, _usable_number, _icon_lefttop) {}
Potion::~Potion() {}
