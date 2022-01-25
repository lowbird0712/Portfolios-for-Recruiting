#pragma once

#include "../../../Singleton/Scene_Manager/Scene.h"
#include "../../../Singleton/Input_Manager/Input_Manager.h"
#include "../../../Singleton/FMOD_Manager/FMOD_Manager.h"
#include "../../../Singleton/Scene_Manager/Scene_Manager.h"
#include "../../../Pieces/Pieces/Panal.h"
#include "../../../Pieces/Pieces/Tilemap/Tilemap.h"
#include "../../../Game_Object/Story_Block/Story_Block.h"

#define VISUAL_NOVEL_LAYER_KEY		"Visual_Novel_Layer"

class Gamenovel_Ingame_Scene : public Scene {
protected:
	// Variables
	string						m_BGM_key;
	Panal*						m_ingamepanal;
	Tilemap*					m_tilemap;
	vector<Story_Block*>		m_storyblock_vector;
	size_t						m_storyblock_index;

	vector<vector<POSITION>>	m_load_trigger_vector_vector;
	vector<vector<POSITION>>	m_load_indexchanges_vector_vector;
public:
	// Functions
	Gamenovel_Ingame_Scene();
	virtual ~Gamenovel_Ingame_Scene();

	bool						Event_Trigger_Load(const TCHAR * const _tiledata_filename,
									const string &_tileset_key, const TCHAR * const _tileset_filename, const COLORREF &_color = TEXTURE_COLORKEY_UNUSE,
									const string &_tiledata_pathkey = DATA_PATH_KEY, const string &_tileset_pathkey = IMAGE_PATH_KEY);

	virtual void				Goto_Next_Storyblock();

	Tilemap*					Get_Tilemap() const						{ return m_tilemap; }
	size_t						Get_Storyblock_Index() const			{ return m_storyblock_index; }
	size_t						Get_Storyblock_Vector_Size() const		{ return m_storyblock_vector.size(); }
	Story_Block*				Get_Current_Story_Block() const			{ return m_storyblock_vector[m_storyblock_index]; }

	virtual bool				Init();
	virtual void				Input(const float _deltatime)			{ Scene::Input(_deltatime); }
	virtual void				Update(const float _deltatime)			{ Scene::Update(_deltatime); }
	virtual void				Add()									{ /*Scene::Add();*/ }
	virtual void				Late_Update()							{ Scene::Late_Update(); }
	virtual void				Render(const HDC &_hDC)					{ Scene::Render(_hDC); }
};