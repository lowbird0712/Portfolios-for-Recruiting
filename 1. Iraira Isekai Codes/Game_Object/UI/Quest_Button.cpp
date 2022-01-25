#pragma once

#include "Quest_Button.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Before_Battle_Scene.h"
#include "../../Singleton/Core.h"
#include "Quest_UI.h"
#include "../Monster/Monster_Manager.h"

Quest_Button::Quest_Button() {
	// 퀘스트 버튼은 씬에 직접적으로 속해 있지 않으므로 프로토타입과 클론 대신에 사용할 객체를 바로 생성한다.
	// 그래서 생성자에서 직접 Init 함수를 호출해주어야 한다.
	if (!Rect_Button::Init())
		DestroyWindow(GET_HWND());
	if (!Init())
		DestroyWindow(GET_HWND());
}
Quest_Button::Quest_Button(const Quest_Button & _button) : Rect_Button(_button) {
	m_quest = new QUEST;
	*m_quest = *_button.m_quest;
}
Quest_Button::~Quest_Button() {
	if (m_quest)
		delete m_quest;
}

void Quest_Button::Click() {
	if (m_quest->quest_string == "")
		return;

	Set_Selected(!m_quest->selected);

	if (m_quest->selected) {
		if (m_quest_UI->Get_Selected_Num() != -1) {
			Quest_Button * selected_button = m_quest_UI->Get_Selected_Quest_Button();
			selected_button->Set_Selected(FALSE);
		}

		m_quest_UI->Set_Selected_Num(m_number);

		Before_Battle_Scene * scene = (Before_Battle_Scene*)m_quest_UI->Get_Scene();
		if (m_quest->monster_skinkey == GOLEM_KEY) {
			m_quest_UI->Set_Monster_Skinkey(GOLEM_KEY);
			scene->Get_Portraitbox()->Set_Image_Lefttop(UI_PORTRAITBOX_GOLEM_IMAGE_LEFTTOP);
		}
	}

	// 보스 선택 초상화를 변경할 것.
}

void Quest_Button::Set_Selected(const bool _selected) {
	m_quest->selected = _selected;

	if (m_quest->selected)
		m_image_lefttop = UI_QUESTBUTTON_SELECT_IMAGE_LEFTTOP;
	else {
		m_quest_UI->Set_Selected_Num(-1);
		m_image_lefttop = UI_QUESTBUTTON_UNSELECT_IMAGE_LEFTTOP;

		((Before_Battle_Scene*)m_quest_UI->Get_Scene())->Get_Portraitbox()->Set_Image_Lefttop(UI_PORTRAITBOX_NONE_IMAGE_LEFTTOP);
	}
}

bool Quest_Button::Init() {
	if (!Rect_Button::Init())
		return FALSE;

	m_quest = new QUEST;

	return TRUE;
}

void Quest_Button::Render(const HDC & _hDC) {
	Rect_Button::Render(_hDC);

	const string quest_string = m_quest->quest_string;
	const POSITION string_lefttop = m_pos + QUEST_BUTTON_STRINGSHIFT;

	if (quest_string != "")
		TextOut(_hDC, string_lefttop.x, string_lefttop.y, quest_string.c_str(), quest_string.length());
}
