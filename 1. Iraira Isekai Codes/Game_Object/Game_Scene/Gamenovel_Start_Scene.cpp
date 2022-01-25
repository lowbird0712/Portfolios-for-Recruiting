#pragma once

#include "Gamenovel_Start_Scene.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Scene_Manager/Scene_Manager.h"
#include "../../Singleton/FMOD_Manager/FMOD_Manager.h"
#include "../../Pieces/Pieces/Panal.h"
#include "../../Pieces/Pieces/Button/Rect_Button.h"
#include "../../Game_Object/Game_Scene/Gamenovel_Edit_Scene.h"
//#include "../../Game_Object/Game_Scene/Gamenovel_Ingame_Scene/Test_Ingame_Scene.h"

Gamenovel_Start_Scene::Gamenovel_Start_Scene() : m_mainpanal(NULL), m_edit_button(NULL), m_start_button(NULL) { }
Gamenovel_Start_Scene::~Gamenovel_Start_Scene() {
	SAFE_RELEASE(m_mainpanal);
	SAFE_RELEASE(m_edit_button);
	SAFE_RELEASE(m_start_button);
}

void Gamenovel_Start_Scene::Start_Editing() {
	GET_SINGLE(FMOD_Manager)->Stop(ST_BGM);

	if (!GET_SINGLE(Scene_Manager)->Create_Scene<Gamenovel_Edit_Scene>(ST_NEXT))
		DestroyWindow(GET_HWND());
}

void Gamenovel_Start_Scene::Start_Game() {
	GET_SINGLE(FMOD_Manager)->Stop(ST_BGM);

	/*if (!GET_SINGLE(Scene_Manager)->Create_Scene<Test_Ingame_Scene>(ST_NEXT))
		DestroyWindow(GET_HWND());*/
}

bool Gamenovel_Start_Scene::Init() {
	if (!Scene::Init())
		return FALSE;

	// 메인 BGM 삽입
	/*if (!GET_SINGLE(FMOD_Manager)->Load_Sound(STARTSCENE_BGM_KEY, TRUE, STARTSCENE_BGM_FILENAME))
		return FALSE;
	GET_SINGLE(FMOD_Manager)->Play(STARTSCENE_BGM_KEY);*/

	Layer *layer = Find_Layer(BACKGROUND_LAYER_KEY);
	if (!layer)
		return FALSE;

	// 메인 패널 삽입
	/*if (!Create_Prototype<Panal>(PANAL_PROTOTYPE_KEY))
		return FALSE;

	m_mainpanal = (Panal*)Create_Clone(PANAL_PROTOTYPE_KEY, STARTSCENE_MAINPANAL_CLONE_KEY, layer);
	if (!m_mainpanal->Clone_Init(WIDEPANAL_POS, STARTSCENE_MAINPANAL_IMAGE_KEY, STARTSCENE_MAINPANAL_IMAGE_FILENAME))
		return FALSE;*/

	layer = Find_Layer(UI_LAYER_KEY);
	if (!layer)
		return FALSE;

	// 여러 버튼들 삽입
	/*if (!Create_Prototype<Rect_Button>(RECT_BUTTON_PROTOTYPE_KEY))
		return FALSE;

	m_edit_button = (Rect_Button*)Create_Clone(RECT_BUTTON_PROTOTYPE_KEY, STARTSCENE_EDIT_BUTTON_CLONE_KEY, layer);
	if (!m_edit_button->Clone_Init(STARTSCENE_EDIT_BUTTON_IMAGE_LEFTTOP, STARTSCENE_EDIT_BUTTON_SIZE, STARTSCENE_EDIT_BUTTON_POS, this, &Gamenovel_Start_Scene::Start_Editing))
		return FALSE;

	m_start_button = (Rect_Button*)Create_Clone(RECT_BUTTON_PROTOTYPE_KEY, STARTSCENE_START_BUTTON_CLONE_KEY, layer);
	if (!m_start_button->Clone_Init(STARTSCENE_START_BUTTON_IMAGE_LEFTTOP, STARTSCENE_START_BUTTON_SIZE, STARTSCENE_START_BUTTON_POS, this, &Gamenovel_Start_Scene::Start_Game))
		return FALSE;*/

	return TRUE;
}