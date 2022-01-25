#pragma once

#include "Equipment.h"
#include "../Player/Player.h"

const string Equipment::early_clip_key_part = "_Early_Clip_Key";
const string Equipment::later_clip_key_part = "_Later_Clip_Key";

Equipment::Equipment(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
	const EQUIPMENT_TYPE _type, const POSITION & _icon_lefttop)
	: m_name(_name), m_early_clip_key(_early_clip_key), m_later_clip_key(_later_clip_key), m_type(_type), m_icon_image_lefttop(_icon_lefttop) {}
Equipment::Equipment(Equipment & _equipment)
	: m_name(_equipment.m_name), m_early_clip_key(_equipment.m_early_clip_key), m_later_clip_key(_equipment.m_later_clip_key)
		, m_type(_equipment.m_type) {}	// m_player는 NULL인 채로 유지한다.
Equipment::~Equipment() {}

void Equipment::Use() {
	if (m_command_pressed) {
		m_command_pressed = FALSE;

		const string next_command_early_clip_key = \
			m_player->Get_Now_Command_Itemname() + Equipment::early_clip_key_part;
		m_player->Get_Animation()->Set_Nowclip(next_command_early_clip_key);
		m_player->Set_Combo_Possible(TRUE);
	}
	else if (m_type == ET_WEAPON) {
		m_player->Set_Moving(MT_BACK);
		m_player->Get_Animation()->Set_Nowclip(m_later_clip_key);
	}
}