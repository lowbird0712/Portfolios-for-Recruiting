#pragma once

#include "Weapon.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Battle_Scene.h"
#include "../Monster/Monster.h"

Weapon::Weapon(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const float _cooltime, const int _minatk, const int _maxatk, const POSITION & _icon_lefttop)
	: Equipment(_name, _early_clip_key, _later_clip_key, ET_WEAPON, _icon_lefttop), \
	m_cooltime(_cooltime), m_minatk(_minatk), m_maxatk(_maxatk) {}
Weapon::~Weapon() {}

void Weapon::Use() {
	Equipment::Use();

	Object * target = m_player->Get_Target();

	if (target->Get_Strtag() == MONSTER_CLONE_KEY)
		m_player->Attack(Get_Random_Damage());

	m_cooltime_state = TRUE;
}

int Weapon::Get_Random_Damage() {
	return rand() % (m_maxatk - m_minatk + 1) + m_minatk;
}

void Weapon::Set_Cooltime_State(const int _state) {
	if (!_state)
		m_cooltime_timer = 0.f;

	m_cooltime_state = _state;
}

void Weapon::Update(const float _deltatime) {
	if (m_cooltime_state) {
		m_cooltime_timer += _deltatime;
		if (m_cooltime_timer >= m_cooltime) {
			m_cooltime_state = FALSE;
			m_cooltime_timer = 0.f;
		}
	}
}
