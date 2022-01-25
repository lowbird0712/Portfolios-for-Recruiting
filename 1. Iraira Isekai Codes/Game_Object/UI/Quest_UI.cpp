#pragma once

#include "Quest_UI.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Before_Battle_Scene.h"
#include "../Monster/Monster_Manager.h"
#include "Quest_Button.h"

Quest_UI::Quest_UI() {}
Quest_UI::Quest_UI(Quest_UI & _ui) : Panal(_ui) {
	m_quest_pages = new Quest_Button**[QUEST_UI_MAX_PAGENUM];
	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		m_quest_pages[i] = new Quest_Button*[QUEST_UI_MAX_QUESTNUM];
		for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++)
			m_quest_pages[i][j] = new Quest_Button(*_ui.m_quest_pages[i][j]);
	}

	m_up_button = new Rect_Button(*_ui.m_up_button);
	m_down_button = new Rect_Button(*_ui.m_down_button);
}
Quest_UI::~Quest_UI() {
	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++)
			SAFE_RELEASE(m_quest_pages[i][j]);
		delete[] m_quest_pages[i];
	}
	delete[] m_quest_pages;

	SAFE_RELEASE(m_up_button);
	SAFE_RELEASE(m_down_button);
}

bool Quest_UI::Clone_Init(const POSITION &_pos, const POSITION & _image_lefttop,
	const FLOATSIZE & _size, const string &_texture_key) {

	if (!Panal::Clone_Init(_pos, _image_lefttop, _size, _texture_key))
		return FALSE;

	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++) {
			const POSITION pos = m_pos + POSITION(QUEST_UI_QUESTBUTTON1_LEFTTOP.x,
				QUEST_UI_QUESTBUTTON1_LEFTTOP.y + j * QUEST_UI_QUESTBUTTON_YSHIFT);

			if (!m_quest_pages[i][j]->Clone_Init(UI_QUESTBUTTON_UNSELECT_IMAGE_LEFTTOP, UI_QUESTBUTTON_SIZE,
				pos, m_quest_pages[i][j], &Quest_Button::Click))
				return FALSE;

			m_quest_pages[i][j]->Set_Quest_UI(this);
			m_quest_pages[i][j]->Set_Number(j);
		}
	}

	const POSITION upbutton_lefttop = {
		m_pos.x + QUEST_UI_QUESTBUTTON1_LEFTTOP.x + UI_QUESTBUTTON_SIZE.x / 2 - UI_QUEST_UPDOWNBUTTON_SIZE.x / 2,
		m_pos.y + QUEST_UI_QUESTBUTTON1_LEFTTOP.y - QUEST_UI_UPDOWNBUTTON_YSHIFT - UI_QUEST_UPDOWNBUTTON_SIZE.y
	};
	if (!m_up_button->Clone_Init(UI_QUESTUPBUTTON_IMAGE_LEFTTOP, UI_QUEST_UPDOWNBUTTON_SIZE,
		upbutton_lefttop, this, &Quest_UI::Quest_Page_Up))
		return FALSE;

	const POSITION downbutton_lefttop = {
		upbutton_lefttop.x,
		m_pos.y + QUEST_UI_QUESTBUTTON1_LEFTTOP.y + QUEST_UI_QUESTBUTTON_YSHIFT * 3 + UI_QUESTBUTTON_SIZE.y + QUEST_UI_UPDOWNBUTTON_YSHIFT
	};
	if (!m_down_button->Clone_Init(UI_QUESTDOWNBUTTON_IMAGE_LEFTTOP, UI_QUEST_UPDOWNBUTTON_SIZE,
		downbutton_lefttop, this, &Quest_UI::Quest_Page_Down))
		return FALSE;

	return TRUE;
}

void Quest_UI::Add_Quest(const string & _string, const string & _skinkey) {
	if (_string == "" || _skinkey == "")
		return;

	// 퀘스트는 페이지의 앞쪽부터 순차적으로 채워져있는 상태임을 가정.
	// 매번 사용자 실수를 체크하므로 퀘스트 스트링과 몬스터 스킨키가 동시에 ""일 가능성은 없다.
	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++) {
			QUEST* quest = m_quest_pages[i][j]->Get_Quest();

			if (quest->quest_string == "") {
				quest->quest_string = _string;
				quest->monster_skinkey = _skinkey;
				return;
			}
		}
	}
}

void Quest_UI::Quest_Page_Up() {
	if (m_pagenum > 0) {
		if (m_selected_num != -1)
			Get_Selected_Quest_Button()->Set_Selected(FALSE);

		((Before_Battle_Scene*)Get_Scene())->Get_Portraitbox()->Set_Image_Lefttop(UI_PORTRAITBOX_NONE_IMAGE_LEFTTOP);

		m_pagenum--;
	}
}

void Quest_UI::Quest_Page_Down() {
	if (m_pagenum < QUEST_UI_MAX_PAGENUM - 1) {
		if (m_selected_num != -1)
			Get_Selected_Quest_Button()->Set_Selected(FALSE);

		((Before_Battle_Scene*)Get_Scene())->Get_Portraitbox()->Set_Image_Lefttop(UI_PORTRAITBOX_NONE_IMAGE_LEFTTOP);

		m_pagenum++;
	}
}

bool Quest_UI::Init() {
	// 퀘스트 페이지 생성
	m_quest_pages = new Quest_Button**[QUEST_UI_MAX_PAGENUM];
	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		m_quest_pages[i] = new Quest_Button*[QUEST_UI_MAX_QUESTNUM];

		for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++) {
			m_quest_pages[i][j] = new Quest_Button;

			Texture * texture = GET_SINGLE(Resource_Manager)->Find_Texture(UI_TEXTURE_KEY);
			if (!m_quest_pages[i][j]->Set_Texture(texture))
				return FALSE;
			SAFE_RELEASE(texture);

			m_quest_pages[i][j]->Set_Mouseon_Image_Lefttop(QUEST_BUTTON_MOUSEON_IMAGE_LEFTTOP);
			m_quest_pages[i][j]->Set_Clicked_Image_Lefttop(QUEST_BUTTON_CLICKED_IMAGE_LEFTTOP);
			m_quest_pages[i][j]->Set_Mouseon_Alpha(QUEST_BUTTON_MOUSEON_ALPHA);
			m_quest_pages[i][j]->Set_Clicked_Alpha(QUEST_BUTTON_CLICKED_ALPHA);
			m_quest_pages[i][j]->Set_Mouseon_Use(TRUE);
			m_quest_pages[i][j]->Set_Clicked_Use(TRUE);
		}
	}

	// 퀘스트 업다운 버튼 생성
	// 퀘스트 업다운 버튼은 프로토타입과 클론을 생성하지 않으므로 직접 초기화 함수를 호출해 주어야 한다.
	m_up_button = new Rect_Button;
	if (!m_up_button->Init())
		return FALSE;

	m_down_button = new Rect_Button;
	if (!m_down_button->Init())
		return FALSE;

	// 퀘스트 추가
	// 추후 파일로부터 불러오는 방식으로 수정할 것
	Add_Quest("골렘을 쓰러뜨려라", GOLEM_KEY);
	Add_Quest("두번째 퀘스트 추가를 확인하기 위한 테스트", "None_Test");
	Add_Quest("세번째 퀘스트 추가를 확인하기 위한 테스트", "None_Test");
	Add_Quest("네번째 퀘스트 추가를 확인하기 위한 테스트", "None_Test");
	Add_Quest("골렘을 3번 쓰러뜨려라", GOLEM_KEY);

	return TRUE;
}

void Quest_UI::Input(const float _deltatime) {
	Panal::Input(_deltatime);

	for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++)
		m_quest_pages[m_pagenum][j]->Input(_deltatime);

	m_up_button->Input(_deltatime);
	m_down_button->Input(_deltatime);
}

void Quest_UI::Late_Update() {
	Panal::Late_Update();

	for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++)
		m_quest_pages[m_pagenum][j]->Late_Update();

	m_up_button->Late_Update();
	m_down_button->Late_Update();
}

void Quest_UI::Render(const HDC & _hDC) {
	Panal::Render(_hDC);

	// 퀘스트 페이지 출력
	for (size_t j = 0; j < QUEST_UI_MAX_QUESTNUM; j++)
		m_quest_pages[m_pagenum][j]->Render(_hDC);

	// 아이템 박스 빈자리 출력
	Texture * UI_texture = ((Before_Battle_Scene*)m_scene)->Get_UI_Texture();
	POSITION itembox1_pos = m_pos + ITEMBOX1_LEFTTOP;

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		TransparentBlt(_hDC, itembox1_pos.x, itembox1_pos.y + UI_ITEMBOX_SIZE.y * i, UI_ITEMBOX_SIZE.x, UI_ITEMBOX_SIZE.y,
			UI_texture->Get_MemDC(), UI_ITEMBOX_IMAGE_LEFTTOP.x, UI_ITEMBOX_IMAGE_LEFTTOP.y,
			UI_ITEMBOX_SIZE.x, UI_ITEMBOX_SIZE.y, UI_texture->Get_Colorkey());
	}

	// 퀘스트 페이지 업다운 버튼 출력
	m_up_button->Render(_hDC);
	m_down_button->Render(_hDC);

	// 페이지 라이트 출력
	POSITION light_image_lefttop = POSITION();

	for (size_t i = 0; i < QUEST_UI_MAX_PAGENUM; i++) {
		if (i == m_pagenum)
			light_image_lefttop = UI_PAGELIGHT_GREEN_IMAGE_LEFTTOP;
		else
			light_image_lefttop = UI_PAGELIGHT_WHITE_IMAGE_LEFTTOP;

		TransparentBlt(_hDC, m_pos.x + QUEST_UI_PAGELIGHT1_LEFTTOP.x, m_pos.y + QUEST_UI_PAGELIGHT1_LEFTTOP.y + QUEST_UI_PAGELIGHT_YSHIFT * i,
			UI_PAGELIGHT_SIZE.x, UI_PAGELIGHT_SIZE.y,
			UI_texture->Get_MemDC(), light_image_lefttop.x, light_image_lefttop.y,
			UI_PAGELIGHT_SIZE.x, UI_PAGELIGHT_SIZE.y, UI_texture->Get_Colorkey());
	}
}
