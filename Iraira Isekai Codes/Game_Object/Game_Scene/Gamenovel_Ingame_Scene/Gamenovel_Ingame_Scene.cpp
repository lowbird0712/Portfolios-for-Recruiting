#pragma once

#include "Gamenovel_Ingame_Scene.h"
#include "../Gamenovel_Edit_Scene.h"

Gamenovel_Ingame_Scene::Gamenovel_Ingame_Scene() : m_BGM_key(""), m_ingamepanal(NULL), m_tilemap(NULL), m_storyblock_index(0) { }
Gamenovel_Ingame_Scene::~Gamenovel_Ingame_Scene() {
	SAFE_RELEASE(m_ingamepanal);
	SAFE_RELEASE(m_tilemap);
	SAFE_VECLIST_RELEASE(m_storyblock_vector);
}

bool Gamenovel_Ingame_Scene::Event_Trigger_Load(const TCHAR * const _tiledata_filename, const string & _tileset_key,
	const TCHAR * const _tileset_filename, const COLORREF & _color, const string & _tiledata_pathkey, const string & _tileset_pathkey) {

	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_tiledata_pathkey);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _tiledata_filename;

	FILE *file = NULL;
	_tfopen_s(&file, strpath.c_str(), "rb");
	if (!file)
		return FALSE;

	TCHAR filename[MAX_PATH];
	if (fread(filename, sizeof(TCHAR) * MAX_PATH, 1, file) != 1)
		return FALSE;

	if (!m_tilemap->Clone_Load_Init(filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
		return FALSE;


	POSITION position;
	vector<POSITION> position_vector;
	while (fread(&position, sizeof(POSITION), 1, file) == 1) {
		if (position == EDITSCENE_TRIGGER_END_FLOATPOINT) {
			m_load_trigger_vector_vector.push_back(position_vector);
			position_vector.clear();
		}
		else if (position == EDITSCENE_INDEXCHANGES_END_FLOATPOINT) {
			m_load_indexchanges_vector_vector.push_back(position_vector);
			position_vector.clear();
		}
		else
			position_vector.push_back(position);
	}

	for (size_t i = 0; i < m_load_trigger_vector_vector[0].size(); i++)
		m_tilemap->Set_Tile_Event_Trigger(m_load_trigger_vector_vector[0][i]);

	// 처음부터 로드한 타일맵과 다른 부분이 있을 경우 실행된다.
	POSITION tileindex;
	for (size_t i = 0; i < m_load_indexchanges_vector_vector[0].size(); i++) {
		int remainder = i % 4;

		switch (remainder) {
		case 0:
			if (m_load_indexchanges_vector_vector[0][i] != EDITSCENE_INDEXCHANGES_END_FLOATPOINT) {
				tileindex = m_load_indexchanges_vector_vector[0][i]; // 수정 사항이 있는 경우 이 라인을 반드시 지난다.
				m_tilemap->Set_Tile_Lefttop(tileindex, m_load_indexchanges_vector_vector[0][i]);
			}
			break;
		case 1: // m_load_indexchanges_vector_vector[0][i]가 EDITSCENE_INDEXCHANGES_END_FLOATPOINT인 경우에는 벡터벡터의 사이즈가 1이므로 여기부터의
					// case는 실행되지 않는다.
			m_tilemap->Set_Tile_Object_Lefttop(tileindex, m_load_indexchanges_vector_vector[0][i]);
			break;
		case 2:
			m_tilemap->Set_Tile_Option_Lefttop(tileindex, m_load_indexchanges_vector_vector[0][i]);
			break;
		case 3: // 이 경우는 m_load_indexchanges_vector_vector[0][i]가 EDITSCENE_INDEXCHANGES_END_FLOATPOINT인 경우밖에 없다.
			break;
		}
	}

	return TRUE;
}

void Gamenovel_Ingame_Scene::Goto_Next_Storyblock() {
	m_storyblock_vector[m_storyblock_index]->Set_Enable(FALSE);
	m_storyblock_index++;

	if (!m_load_trigger_vector_vector.empty() && m_storyblock_index < m_load_trigger_vector_vector.size()) {
		for (size_t i = 0; i < m_load_trigger_vector_vector[m_storyblock_index - 1].size(); i++)
			m_tilemap->Set_Tile_Event_Trigger(m_load_trigger_vector_vector[m_storyblock_index - 1][i]);

		for (size_t i = 0; i < m_load_trigger_vector_vector[m_storyblock_index].size(); i++)
			m_tilemap->Set_Tile_Event_Trigger(m_load_trigger_vector_vector[m_storyblock_index][i]);

		POSITION tileindex;
		for (size_t i = 0; i < m_load_indexchanges_vector_vector[m_storyblock_index].size(); i++) {
			int remainder = i % 4;

			switch (remainder) {
			case 0:
				tileindex = m_load_indexchanges_vector_vector[m_storyblock_index][i];
				break;
			case 1:
				m_tilemap->Set_Tile_Lefttop(tileindex, m_load_indexchanges_vector_vector[m_storyblock_index][i]);
				break;
			case 2:
				m_tilemap->Set_Tile_Object_Lefttop(tileindex, m_load_indexchanges_vector_vector[m_storyblock_index][i]);
				break;
			case 3:
				m_tilemap->Set_Tile_Option_Lefttop(tileindex, m_load_indexchanges_vector_vector[m_storyblock_index][i]);
				break;
			}
		}
	}

	if (m_BGM_key != GET_SINGLE(FMOD_Manager)->Get_Playing_BGM_Key())
		GET_SINGLE(FMOD_Manager)->Play(m_BGM_key);

	m_tilemap->Set_Freeze(FALSE);
}

bool Gamenovel_Ingame_Scene::Init() {
	if (!Scene::Init())
		return FALSE;

	if (!Create_Layer(VISUAL_NOVEL_LAYER_KEY, 2))
		return FALSE;

	return TRUE;
}