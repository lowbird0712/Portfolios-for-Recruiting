#pragma once

#include "../../Singleton/Scene_Manager/Scene.h"
#include "../../Pieces/Pieces/Tilemap/Tilemap.h"

#define EDITSCENE_PANAL_CLONE_KEY				"Editscene_Panal_Clone"
#define EDITSCENE_PANAL_IMAGE_KEY				"Editscene_Panal_Image"
//#define EDITSCENE_PANAL_IMAGE_FILENAME			TEXT("Startscene_Panal.bmp")

#define EDITSCENE_TILEMAP_CLONE_KEY				"Editscene_Tilemap_Clone"
#define EDITSCENE_TILEMAP_TMP_CLONE_KEY			"Editscene_Tilemap_Tmp_Clone"
//#define EDITSCENE_TILEMAP_LEFTTOP				POSITION(100, 100)

#define EDITSCENE_TRIGGER_END_FLOATPOINT		FLOATPOINT(-10.f, -10.f)
#define EDITSCENE_INDEXCHANGES_END_FLOATPOINT	FLOATPOINT(-11.f, -11.f)

#define EDITSCENE_SAVE							"Editscene_Save"
#define EDITSCENE_EVENT_TRIGGER_MODE			"Editscene_Event_Trigger_Mode"
#define EDITSCENE_FIX_EVENT_TRIGGER_MODE		"Editscene_Fix_Event_Trigger_Mode"
#define EDITSCENE_EVENT_TRIGGER_NODESAVE		"Editscene_Event_Trigger_Nodesave"
#define EDITSCENE_TILEMAP_CHANGE_LEFTTOP		"Editscene_Tilemap_Change_Lefttop"
#define EDITSCENE_TILEMAP_LOAD					"Editscene_Tilemap_Load"

#define EDITSCENE_TILEMAP_TEXTUREMODE			"Editscene_Tilemap_Texturemode"
#define EDITSCENE_TILEMAP_OBJECTMODE			"Editscene_Tilemap_Objectmode"
#define EDITSCENE_TILEMAP_OPTIONMODE			"Editscene_Tilemap_Optionmode"
#define EDITSCENE_TILEMAP_TRIGGERMODE			"Editscene_Tilemap_Triggermode"

// 필요 시 추가할 것
#define EDITSCENE_TILEMAP_NUMBER_1				"Editscene_Tilemap_Number_1"
#define EDITSCENE_TILEMAP_NUMBER_2				"Editscene_Tilemap_Number_2"
#define EDITSCENE_TILEMAP_NUMBER_3				"Editscene_Tilemap_Number_3"
#define EDITSCENE_TILEMAP_NUMBER_4				"Editscene_Tilemap_Number_4"
#define EDITSCENE_TILEMAP_NUMBER_5				"Editscene_Tilemap_Number_5"
#define EDITSCENE_TILEMAP_NUMBER_6				"Editscene_Tilemap_Number_6"
#define EDITSCENE_TILEMAP_NUMBER_7				"Editscene_Tilemap_Number_7"
#define EDITSCENE_TILEMAP_NUMBER_8				"Editscene_Tilemap_Number_8"
#define EDITSCENE_TILEMAP_NUMBER_9				"Editscene_Tilemap_Number_9"
#define EDITSCENE_TILEMAP_NUMBER_0				"Editscene_Tilemap_Number_0"
#define EDITSCENE_TILEMAP_NUMBER_P				"Editscene_Tilemap_Number_P"
#define EDITSCENE_TILEMAP_NUMBER_O				"Editscene_Tilemap_Number_O"

enum TILEMAP_EDITING_MODE {
	TEM_TILEMAP,
	TEM_EVENT_TRIGGER
};

enum TILE_EDITING_COMPONENT {
	TEC_TEXTURE,
	TEC_OBJECT,
	TEC_OPTION,
	TEC_EVENT_TRIGGER
};

class Panal;

class Gamenovel_Edit_Scene : public Scene {
	// Variables
	static TCHAR							m_filename[MAX_PATH];
	static TCHAR							m_filename_tmp[MAX_PATH];
	static TCHAR							m_numwidth_str[MAX_PATH];
	static TCHAR							m_numheight_str[MAX_PATH];

	bool									m_same_tile;
	POSITION								m_tileindex_tmp;

	TILEMAP_EDITING_MODE					m_tilemap_editing_mode;
	TILE_EDITING_COMPONENT					m_tile_editing_component;

	unordered_map<string, POSITION>			m_tileselect_map;
	string									m_tile_nowkey;
	unordered_map<string, OBJECT_INFO>		m_objectselect_map;
	string									m_object_nowkey;
	unordered_map<string, POSITION>			m_optionselect_map;
	string									m_option_nowkey;

	// 파일 저장 및 로딩에서 사용될 변수들
	size_t									m_vector_vector_index;
	vector<vector<POSITION>>				m_trigger_vector_vector;
	vector<vector<POSITION>>				m_indexchanges_vector_vector;
	vector<vector<POSITION>>				m_load_trigger_vector_vector;
	vector<vector<POSITION>>				m_load_indexchanges_vector_vector;

	Panal*									m_editpanal;
	Tilemap*								m_tilemap;
	Tilemap*								m_tilemap_tmp; // 이벤트 트리거 모드에서 타일맵이 변경되었는지를 체크하는 과정에서 비교하기 위해 사용된다.
public:
	// Functions
	Gamenovel_Edit_Scene();
	virtual ~Gamenovel_Edit_Scene();

	POSITION					Get_Nowtile_Image_Lefttop() const { return m_tileselect_map.find(m_tile_nowkey)->second; }

	static INT_PTR CALLBACK		Filename_DlgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	static INT_PTR CALLBACK		Floatpoint_DlgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	bool						Event_Trigger_Save(const TCHAR * const _tiledata_filename, const string &_path_key = DATA_PATH_KEY);
	bool						Event_Trigger_Load(const TCHAR * const _tiledata_filename,
									const string &_tileset_key, const TCHAR * const _tileset_filename, const COLORREF &_color = TEXTURE_COLORKEY_UNUSE,
									const string &_tiledata_pathkey = DATA_PATH_KEY, const string &_tileset_pathkey = IMAGE_PATH_KEY);

	virtual bool				Init();
	virtual void				Input(const float _deltatime);
	virtual void				Update(const float _deltatime);
	virtual void				Add() { /*Scene::Add();*/ }
	virtual void				Late_Update() { Scene::Late_Update(); }
	virtual void				Render(const HDC &_hDC);
};