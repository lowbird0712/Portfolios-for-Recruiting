#pragma once

#include "Inventory_Button.h"
#include "../../Singleton/Core.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Before_Battle_Scene.h"
#include "Inventory_UI.h"

Inventory_Button::Inventory_Button() {
	if (!Rect_Button::Init())
		DestroyWindow(GET_HWND());
}
Inventory_Button::Inventory_Button(Inventory_Button & _button) : Rect_Button(_button) {
	m_original_pos = _button.m_original_pos;
	m_num = _button.m_num;
}
Inventory_Button::~Inventory_Button() {}

void Inventory_Button::Click() {
	const size_t selecting_boxnum = m_inventory_UI->Get_Selecting_Boxnum();

	if (!m_selected && !m_inventory_UI->Get_Boxnum_Selected(selecting_boxnum)) {
		m_pos = m_scene->Get_Itembox_Icon_Lefttop(selecting_boxnum);
		m_inventory_UI->Set_Boxnum_Selected(selecting_boxnum, TRUE);
		m_inventory_UI->Set_Selected_Button_Index(selecting_boxnum, m_num);
		m_selected = TRUE;
		m_selecting_boxnum = selecting_boxnum;
	}
	else if (m_selected) {
		if (!m_inventory_UI->Get_Enable())
			m_scene->Flush_Selected_Buttons(m_selecting_boxnum);

		m_pos = m_original_pos;
		m_inventory_UI->Set_Boxnum_Selected(m_selecting_boxnum, FALSE);
		m_inventory_UI->Set_Selected_Button_Index(m_selecting_boxnum, INVENTORY_UI_DEFAULT_INDEX);
		m_selected = FALSE;
		m_selecting_boxnum = -1;
	}
}
