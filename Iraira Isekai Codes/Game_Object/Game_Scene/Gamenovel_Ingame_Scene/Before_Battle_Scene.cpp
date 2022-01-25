#pragma once

#include "Before_Battle_Scene.h"
#include "../../../Singleton/Core.h"
#include "../../../Singleton/Resource_Manager/Texture.h"
#include "../../../Singleton/Input_Manager/Input_Manager.h"
#include "../../../Pieces/Pieces/Background.h"
#include "../../../Pieces/Pieces/Button/Rect_Button.h"
#include "../../UI/Quest_UI.h"
#include "../../UI/Quest_Button.h"
#include "../../UI/Inventory_UI.h"
#include "../../UI/Inventory_Button.h"
#include "Battle_Scene.h"

Before_Battle_Scene::Before_Battle_Scene() {}
Before_Battle_Scene::~Before_Battle_Scene() {
	SAFE_RELEASE(m_UI_texture);
	SAFE_RELEASE(m_paper_texture);
	SAFE_RELEASE(m_background);
	SAFE_RELEASE(m_quest_UI_button);
	SAFE_RELEASE(m_inventory_UI_button);
	SAFE_RELEASE(m_quest_UI);
	SAFE_RELEASE(m_inventory_UI);
	SAFE_RELEASE(m_portraitbox);
	SAFE_RELEASE(m_battle_button);

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++)
		SAFE_RELEASE(m_selected_buttons[i]);
	delete[] m_selected_buttons;
}

void Before_Battle_Scene::Goto_Quest_UI() {
	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		if (m_inventory_UI->Get_Boxnum_Selected(i))
			m_selected_buttons[i] = m_inventory_UI->Get_Selected_Button(i);
	}

	m_quest_UI->Set_Enable(TRUE);
	m_inventory_UI->Set_Enable(FALSE);
}

void Before_Battle_Scene::Goto_Inventory_UI() {
	m_quest_UI->Set_Enable(FALSE);
	m_inventory_UI->Set_Enable(TRUE);
}

void Before_Battle_Scene::Start_Battle() {
	if (!GET_SINGLE(Scene_Manager)->Create_Scene<Battle_Scene>(ST_NEXT))
		DestroyWindow(GET_HWND());
}

POSITION Before_Battle_Scene::Get_Itembox_Icon_Lefttop(const size_t _num) const {
	// 퀘스트 UI와 인벤토리 UI의 레프트탑은 항상 같다. 그러므로 두 가지 UI의 경우 모두 퀘스트 UI의 값을 가져다가 쓴다.
	return m_quest_UI->Get_Pos() + ITEMBOX1_LEFTTOP + POSITION(0, _num * UI_ITEMBOX_SIZE.y) + ITEMBOX_ICON_SHIFT;
}

bool Before_Battle_Scene::Init() {
	if (!Gamenovel_Ingame_Scene::Init())
		return FALSE;

	Layer *layer = Find_Layer(BACKGROUND_LAYER_KEY);

	// 배경
	if (!Create_Prototype<Background>(BACKGROUND_PROTOTYPE_KEY))
		return FALSE;

	m_background = (Background*)Create_Clone(BACKGROUND_PROTOTYPE_KEY, BEFORE_BATTLE_BACKGROUND_CLONE_KEY, layer);
	if (!m_background->Clone_Init(BEFORE_BATTLE_BACKGROUND_TEXTURE_KEY, BEFORE_BATTLE_BACKGROUND_TEXTURE_FILENAME, FALSE))
		return FALSE;

	layer = Find_Layer(UI_LAYER_KEY);

	// UI, 페이퍼 이미지 로드
	m_UI_texture = GET_SINGLE(Resource_Manager)->Load_Texture(UI_TEXTURE_KEY, UI_TEXTURE_FILENAME, RGB_MAGENTA);
	if (!m_UI_texture)
		return FALSE;

	m_paper_texture = GET_SINGLE(Resource_Manager)->Load_Texture(UI_PAPER_TEXTURE_KEY, UI_PAPER_TEXTURE_FILENAME, RGB_MAGENTA);
	if (!m_paper_texture)
		return FALSE;

	// UI 선택 버튼
	if (!Create_Prototype<Rect_Button>(RECT_BUTTON_PROTOTYPE_KEY))
		return FALSE;

	m_quest_UI_button = (Rect_Button*)Create_Clone(RECT_BUTTON_PROTOTYPE_KEY, QUEST_UI_BUTTON_CLONE_KEY, layer);
	if (!m_quest_UI_button->Clone_Init(TEXTURE_UNUSE_IMAGE_LEFTTOP, UI_BUTTON_SIZE, UI_LEFTTOP + QUEST_UI_BUTTON_LEFTTOP,
		this, &Before_Battle_Scene::Goto_Quest_UI))
		return FALSE;

	m_inventory_UI_button = (Rect_Button*)Create_Clone(RECT_BUTTON_PROTOTYPE_KEY, INVENTORY_UI_BUTTON_CLONE_KEY, layer);
	if (!m_inventory_UI_button->Clone_Init(TEXTURE_UNUSE_IMAGE_LEFTTOP, UI_BUTTON_SIZE, UI_LEFTTOP + INVENTORY_UI_BUTTON_LEFTTOP,
		this, &Before_Battle_Scene::Goto_Inventory_UI))
		return FALSE;

	// 퀘스트 UI
	if (!Create_Prototype<Quest_UI>(QUEST_UI_PROTOTYPE_KEY))
		return FALSE;

	m_quest_UI = (Quest_UI*)Create_Clone(QUEST_UI_PROTOTYPE_KEY, QUEST_UI_CLONE_KEY, layer);
	if (!m_quest_UI->Clone_Init(UI_LEFTTOP, UI_QUESTPAPER_IMAGE_LEFTTOP, UI_PAPER_SIZE, UI_PAPER_TEXTURE_KEY))
		return FALSE;

	// 인벤토리 UI
	if (!Create_Prototype<Inventory_UI>(INVENTORY_UI_PROTOTYPE_KEY))
		return FALSE;

	m_inventory_UI = (Inventory_UI*)Create_Clone(INVENTORY_UI_PROTOTYPE_KEY, INVENTORY_UI_CLONE_KEY, layer);
	if (!m_inventory_UI->Clone_Init(UI_LEFTTOP, UI_INVENTORYPAPER_IMAGE_LEFTTOP, UI_PAPER_SIZE, UI_PAPER_TEXTURE_KEY))
		return FALSE;
	
	m_inventory_UI->Set_Enable(FALSE);

	// 플레이어 이름
	m_player_name = "미정이"; // 추후 파일에서 읽는 방식으로 수정할 것

	// 보스 선택 현황 초상화
	if (!Create_Prototype<Panal>(PANAL_PROTOTYPE_KEY))
		return FALSE;

	m_portraitbox = (Panal*)Create_Clone(PANAL_PROTOTYPE_KEY, PORTRAITBOX_CLONE_KEY, layer);
	if (!m_portraitbox->Clone_Init(m_quest_UI->Get_Pos() + PORTRAITBOX_LEFTTOP,
		UI_PORTRAITBOX_NONE_IMAGE_LEFTTOP, UI_PORTRAITBOX_SIZE, UI_TEXTURE_KEY))
		return FALSE;

	// 선택된 아이템 버튼 배열 생성
	m_selected_buttons = new Inventory_Button*[ITEMBOX_MAXNUM];
	memset(m_selected_buttons, NULL, ITEMBOX_MAXNUM * sizeof(Inventory_Button*));

	// 배틀 시작 버튼 생성
	m_battle_button = (Rect_Button*)Create_Clone(RECT_BUTTON_PROTOTYPE_KEY, BATTLE_BUTTON_CLONE_KEY, layer);
	if (!m_battle_button->Clone_Init(UI_BATTLEBUTTON_NORMAL_IMAGE_LEFTTOP, UI_BATTLEBUTTON_SIZE, BATTLEBUTTON_LEFTTOP,
		this, &Before_Battle_Scene::Start_Battle))
		return FALSE;
	m_battle_button->Set_Mouseon_Use(TRUE);
	m_battle_button->Set_Mouseon_Image_Lefttop(UI_BATTLEBUTTON_MOUSEON_IMAGE_LEFTTOP);
	m_battle_button->Set_Mouseon_Alpha(BATTLEBUTTON_MOUSEON_ALPHA);
	m_battle_button->Set_Clicked_Use(TRUE);
	m_battle_button->Set_Clicked_Image_Lefttop(UI_BATTLEBUTTON_CLICKED_IMAGE_LEFTTOP);
	m_battle_button->Set_Clicked_Alpha(BATTLEBUTTON_CLICKED_ALPHA);

	return TRUE;
}

void Before_Battle_Scene::Input(const float _deltatime) {
	Gamenovel_Ingame_Scene::Input(_deltatime);

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		if (m_selected_buttons[i])
			m_selected_buttons[i]->Input(_deltatime);
	}

	//m_battle_button->Input(_deltatime);
}

void Before_Battle_Scene::Late_Update() {
	Gamenovel_Ingame_Scene::Late_Update();

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		if (m_selected_buttons[i])
			m_selected_buttons[i]->Late_Update();
	}

	//m_battle_button->Late_Update();
}

void Before_Battle_Scene::Render(const HDC & _hDC) {
	Gamenovel_Ingame_Scene::Render(_hDC);

	const POSITION m_UI_pos = m_quest_UI->Get_Pos();

	const POSITION m_name_pos = m_UI_pos + PLAYER_NAME_LEFTTOP;
	TextOut(_hDC, m_name_pos.x, m_name_pos.y, m_player_name.c_str(), m_player_name.length());

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		if (m_selected_buttons[i])
			m_selected_buttons[i]->Render(_hDC);
	}

	//m_battle_button->Render(_hDC);
}
