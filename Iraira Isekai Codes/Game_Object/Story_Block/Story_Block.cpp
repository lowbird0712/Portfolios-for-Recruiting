#pragma once

#include "Story_Block.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Input_Manager/Input_Manager.h"
#include "../../Singleton/FMOD_Manager/FMOD_Manager.h"
#include "../../Game_Object/Game_Scene/Gamenovel_Ingame_Scene/Gamenovel_Ingame_Scene.h"
#include "Sentence_Panal.h"

STORY_NODE::STORY_NODE() : scene(NULL), cutscene_panal(NULL), sentence_panal(NULL) {
	sound_key_vector.push_back(STORYBLOCK_NEXTNODE_EFFECT_KEY);
}

bool STORY_NODE::Add_Sound(const string & _sound_key, const bool _loop, const TCHAR * const _filename, const string & _path_key) {
	if (!GET_SINGLE(FMOD_Manager)->Load_Sound(_sound_key, _loop, _filename, _path_key))
		return FALSE;
	
	sound_key_vector.push_back(_sound_key);

	return TRUE;
}

bool STORY_NODE::Add_Loaded_Sound(const string & _sound_key) {
	if (!GET_SINGLE(FMOD_Manager)->Find_Sound(_sound_key))
		return FALSE;

	sound_key_vector.push_back(_sound_key);

	return TRUE;
}

void STORY_NODE::Add_BGM_Stop_Sound() {
	sound_key_vector.push_back(STORYBLOCK_BGM_STOP_KEY);
}

bool STORY_NODE::Add_Cutscene_Panal(const string & _texture_key, const TCHAR * const _filename, const COLORREF &_color, const string & _path_key) {
	cutscene_panal = new Panal;
	if (!cutscene_panal->Clone_Init(WIDEPANAL_POS, _texture_key, _filename, _color, _path_key))
		return FALSE;

	return TRUE;
}

bool STORY_NODE::Add_Loaded_Cutscene_Panal(const string & _texture_key) {
	cutscene_panal = new Panal;
	if (!cutscene_panal->Clone_Init(WIDEPANAL_POS, _texture_key))
		return FALSE;

	return TRUE;
}

bool STORY_NODE::Add_Character_Panal(const POSITION &_pos, const string & _texture_key, const TCHAR * const _filename, const COLORREF & _color, const string & _path_key) {
	Panal* panal = new Panal;
	if (!panal->Clone_Init(_pos, _texture_key, _filename, _color, _path_key))
		return FALSE;

	character_panal_vector.push_back(panal);

	return TRUE;
}

bool STORY_NODE::Add_Loaded_Character_Panal(const POSITION &_pos, const string & _texture_key) {
	Panal* panal = new Panal;
	if (!panal->Clone_Init(_pos, _texture_key))
		return FALSE;

	character_panal_vector.push_back(panal);

	return TRUE;
}

bool STORY_NODE::Add_Sentence_Panal(const TCHAR * const _speaker_name, const TCHAR * const _sentence) {
	Object *prototype = scene->Find_Prototype(SENTENCE_PANAL_PROTOTYPE_KEY);
	sentence_panal = (Sentence_Panal*)prototype->Clone();
	SAFE_RELEASE(prototype);

	if (!sentence_panal->Clone_Init(_speaker_name, _sentence))
		return FALSE;

	return TRUE;
}

Story_Block::Story_Block() : m_current_index(0), m_sentence_panal_pos(0, 0) { }
Story_Block::Story_Block(const Story_Block & _block) : Unmoving(_block), m_current_index(0), m_sentence_panal_pos(_block.m_sentence_panal_pos) { }
Story_Block::~Story_Block() {
	for (size_t i = 0; i < m_story_node_vector.size(); i++) {
		STORY_NODE *node_tmp = m_story_node_vector[i];

		SAFE_RELEASE(node_tmp->cutscene_panal);
		SAFE_VECLIST_RELEASE(node_tmp->character_panal_vector);
		SAFE_RELEASE(node_tmp->sentence_panal);

		SAFE_DELETE(node_tmp);
	}
}

bool Story_Block::Goto_Next_Node() {
	if (m_current_index == m_story_node_vector.size() - 1) {
		GET_SINGLE(FMOD_Manager)->Play(STORYBLOCK_NEXTNODE_EFFECT_KEY);
		return FALSE;
	}
	
	m_current_index++;

	vector<string> sound_key_vector = m_story_node_vector[m_current_index]->sound_key_vector;
	if (!sound_key_vector.empty()) {
		for (size_t i = 0; i < sound_key_vector.size(); i++) {
			if (sound_key_vector[i] == STORYBLOCK_BGM_STOP_KEY)
				GET_SINGLE(FMOD_Manager)->Stop(ST_BGM);
			else
				GET_SINGLE(FMOD_Manager)->Play(sound_key_vector[i]);
		}
	}

	return TRUE;
}

STORY_NODE * Story_Block::Add_Node(const TCHAR * _speaker_name, const TCHAR * _sentence) {
	STORY_NODE *node = new STORY_NODE;

	node->scene = m_scene;

	if (!node->Add_Sentence_Panal(_speaker_name, _sentence))
		DestroyWindow(GET_HWND());

	m_story_node_vector.push_back(node);

	return node;
}

void Story_Block::Set_Enable(const bool _enable) {
	Reference::Set_Enable(_enable);

	if (_enable) {
		vector<string> sound_key_vector = m_story_node_vector[0]->sound_key_vector;
		if (!sound_key_vector.empty()) {
			for (size_t i = 0; i < sound_key_vector.size(); i++) {
				if (sound_key_vector[i] == STORYBLOCK_BGM_STOP_KEY)
					GET_SINGLE(FMOD_Manager)->Stop(ST_BGM);
				else
					GET_SINGLE(FMOD_Manager)->Play(sound_key_vector[i]);
			}
		}
	}
}

bool Story_Block::Init() {
	if (!GET_SINGLE(FMOD_Manager)->Load_Sound(STORYBLOCK_NEXTNODE_EFFECT_KEY, FALSE, STORYBLOCK_NEXTNODE_EFFECT_FILENAME))
		return FALSE;

	if (!m_scene->Create_Prototype<Sentence_Panal>(SENTENCE_PANAL_PROTOTYPE_KEY))
		return FALSE;

	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_LBUTTON, STORYBLOCK_NEXTPAGE_KEY))
		return FALSE;

	return TRUE;
}

void Story_Block::Input(const float _deltatime) {
	Unmoving::Input(_deltatime);

	if (GET_KEYDOWN(STORYBLOCK_NEXTPAGE_KEY)) {
		if (!Goto_Next_Node())
			((Gamenovel_Ingame_Scene*)m_scene)->Goto_Next_Storyblock();
	}
}

void Story_Block::Render(const HDC & _hDC) {
	Unmoving::Render(_hDC);

	STORY_NODE *node = m_story_node_vector[m_current_index];
	
	if (node->cutscene_panal)
		node->cutscene_panal->Render(_hDC);

	for (size_t i = 0; i < node->character_panal_vector.size(); i++)
		node->character_panal_vector[i]->Render(_hDC);

	node->sentence_panal->Render(_hDC);
}
