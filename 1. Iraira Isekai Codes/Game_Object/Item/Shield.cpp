#pragma once

#include "Shield.h"
#include "../../Singleton/Input_Manager/Input_Manager.h"
#include "../Player/Player.h"

Shield::Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const int _barrier, const POSITION & _icon_lefttop)
	: Equipment(_name, _early_clip_key, _later_clip_key, ET_SHIELD, _icon_lefttop), m_barrier(_barrier) {}
Shield::~Shield() {}

void Shield::Use() {
	// 방어 상태를 켠다
	if (GET_SINGLE(Input_Manager)->Get_Pressed(m_player->Get_Pressed_Itemkey()))
		m_player->Set_Shield_State(TRUE);
	else
		m_player->Get_Animation()->Set_Nowclip(m_later_clip_key);

	Equipment::Use();
}

void Shield::Unuse() {
	// 방어 상태를 끈다
	if (m_player->Get_Shield_State()) {
		m_player->Set_Damaging(FALSE);
		m_player->Set_Shield_State(FALSE);
		m_player->Get_Animation()->Set_Nowclip(m_later_clip_key);
	}
}
