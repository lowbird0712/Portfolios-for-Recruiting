#include "Normal_Shield.h"

Normal_Shield::Normal_Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const int _barrier, const POSITION & _icon_lefttop)
	: Shield(_name, _early_clip_key, _later_clip_key, _barrier, _icon_lefttop) {}
Normal_Shield::~Normal_Shield() {}
