#pragma once

#include "Gamenovel_Edit_Scene.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Input_Manager/Input_Manager.h"
#include "../../Singleton/Path_Manager.h"
#include "../../Singleton/Resource_Manager/Resource_Manager.h"
#include "../../Singleton/FMOD_Manager/FMOD_Manager.h"
#include "../../Singleton/Scene_Manager/Scene_Manager.h"
#include "../../Pieces/Pieces/Panal.h"

TCHAR Gamenovel_Edit_Scene::m_filename[MAX_PATH] = { };
TCHAR Gamenovel_Edit_Scene::m_filename_tmp[MAX_PATH] = { };
TCHAR Gamenovel_Edit_Scene::m_numwidth_str[MAX_PATH] = { };
TCHAR Gamenovel_Edit_Scene::m_numheight_str[MAX_PATH] = { };

Gamenovel_Edit_Scene::Gamenovel_Edit_Scene() : m_same_tile(FALSE), m_tileindex_tmp(-1.f, -1.f), m_tilemap_editing_mode(TEM_TILEMAP), m_tile_editing_component(TEC_TEXTURE),
m_tile_nowkey(TILE_NONE_KEY), m_object_nowkey(OBJECT_NONE_KEY), m_option_nowkey(OPTION_NORMAL_KEY), m_vector_vector_index(0),
m_editpanal(NULL), m_tilemap(NULL), m_tilemap_tmp(NULL) { }
Gamenovel_Edit_Scene::~Gamenovel_Edit_Scene() {
	SAFE_RELEASE(m_editpanal);
	SAFE_RELEASE(m_tilemap);
	SAFE_RELEASE(m_tilemap_tmp);
}

INT_PTR Gamenovel_Edit_Scene::Filename_DlgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	switch (_message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(_wParam)) {
		case IDOK:
			memset(m_filename, 0, sizeof(TCHAR) * MAX_PATH);
			GetDlgItemText(_hWnd, IDC_EDIT1, m_filename, MAX_PATH);
		case IDCANCEL:
			EndDialog(_hWnd, IDOK);
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}

INT_PTR Gamenovel_Edit_Scene::Floatpoint_DlgProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	switch (_message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(_wParam)) {
		case IDOK:
		{
			memset(m_numwidth_str, 0, sizeof(TCHAR) * MAX_PATH);
			GetDlgItemText(_hWnd, IDC_EDIT1, m_numwidth_str, MAX_PATH);
			memset(m_numheight_str, 0, sizeof(TCHAR) * MAX_PATH);
			GetDlgItemText(_hWnd, IDC_EDIT2, m_numheight_str, MAX_PATH);
		}
		case IDCANCEL:
			EndDialog(_hWnd, IDOK);
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}

bool Gamenovel_Edit_Scene::Event_Trigger_Save(const TCHAR * const _tiledata_filename, const string &_path_key) {
	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_path_key);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _tiledata_filename;

	FILE *file = NULL;
	_tfopen_s(&file, strpath.c_str(), "wb");
	if (!file)
		return FALSE;

	if (fwrite(m_filename_tmp, sizeof(TCHAR) * MAX_PATH, 1, file) != 1)
		return FALSE;

	vector<POSITION> trigger_vector;
	vector<POSITION> indexchanges_vector;

	// 트리거 벡터벡터와 인터체인지 벡터벡터는 사이즈가 항상 같으므로 트리거 벡터벡터의 사이즈를 조건으로 for문을 사용한다.
	for (size_t i = 0; i < m_trigger_vector_vector.size(); i++) {
		trigger_vector = m_trigger_vector_vector[i];
		indexchanges_vector = m_indexchanges_vector_vector[i];

		for (size_t j = 0; j < trigger_vector.size(); j++) {
			if (fwrite(&trigger_vector[j], sizeof(POSITION), 1, file) != 1)
				return FALSE;
		}

		for (size_t j = 0; j < indexchanges_vector.size(); j++) {
			if (fwrite(&indexchanges_vector[j], sizeof(POSITION), 1, file) != 1)
				return FALSE;
		}
	}

	fclose(file);

	return TRUE;
}

bool Gamenovel_Edit_Scene::Event_Trigger_Load(const TCHAR * const _tiledata_filename,
	const string & _tileset_key, const TCHAR * const _tileset_filename, const COLORREF & _color,
	const string & _tiledata_pathkey, const string & _tileset_pathkey) {

	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_tiledata_pathkey);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _tiledata_filename;

	FILE *file = NULL;
	_tfopen_s(&file, strpath.c_str(), "rb");
	if (!file)
		return FALSE;

	if (fread(m_filename, sizeof(TCHAR) * MAX_PATH, 1, file) != 1)
		return FALSE;

	if (!m_tilemap->Clone_Load_Init(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
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

	Layer* layer = Find_Layer(DEFAULT_LAYER_KEY);

	SAFE_RELEASE(m_tilemap_tmp);
	m_tilemap_tmp = (Tilemap*)Create_Clone(TILEMAP_PROTOTYPE_KEY, EDITSCENE_TILEMAP_TMP_CLONE_KEY, layer);
	if (!m_tilemap_tmp->Clone_Load_Init(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
		DestroyWindow(GET_HWND());
	m_tilemap_tmp->Set_Enable(FALSE);

	return TRUE;
}

bool Gamenovel_Edit_Scene::Init() {
	if (!Scene::Init())
		return FALSE;

	Layer *layer = Find_Layer(BACKGROUND_LAYER_KEY);

	// 이전 화면에서 프로토타입 전달
	//GET_SINGLE(Scene_Manager)->Send_Prototype(PANAL_PROTOTYPE_KEY);

	// 필요 시 에디터 패널 삽입
	/*m_editpanal = (Panal*)Create_Clone(PANAL_PROTOTYPE_KEY, EDITSCENE_PANAL_CLONE_KEY, layer);
	if (!m_editpanal->Clone_Init(WIDEPANAL_POS, EDITSCENE_PANAL_IMAGE_KEY, EDITSCENE_PANAL_IMAGE_FILENAME))
		return FALSE;*/

	layer = Find_Layer(DEFAULT_LAYER_KEY);

	// 타일맵 크기 결정
	/*if (!Create_Prototype<Tilemap>(TILEMAP_PROTOTYPE_KEY))
		return FALSE;

	ShowCursor(TRUE);
	DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG2), GET_HWND(), Gamenovel_Edit_Scene::Floatpoint_DlgProc);
	ShowCursor(FALSE);

	FLOATPOINT tilemap_tilenum_2D = FLOATPOINT(strtod(m_numwidth_str, NULL), strtod(m_numheight_str, NULL));*/

	// 타일맵 생성
	/*m_tilemap = (Tilemap*)Create_Clone(TILEMAP_PROTOTYPE_KEY, EDITSCENE_TILEMAP_CLONE_KEY, layer);
	if (!m_tilemap->Clone_Init(EDITSCENE_TILEMAP_LEFTTOP, tilemap_tilenum_2D, TILEMAP_TILESIZE,
		Get_Nowtile_Image_Lefttop(), TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
		return FALSE;*/

	// 타일 종류 추가
	/*m_tileselect_map.insert(make_pair(TILE_NONE_KEY, TILE_NONE_IMAGE_LEFTTOP));
	m_tileselect_map.insert(make_pair(TILE_HALL_KEY, TILE_HALL_LEFTTOP));
	m_tileselect_map.insert(make_pair(TILE_RED_SHEET_KEY, TILE_RED_SHEET_LEFTTOP));
	m_tileselect_map.insert(make_pair(TILE_WOOD_STAGE1_KEY, TILE_WOOD_STAGE1_LEFTTOP));
	m_tileselect_map.insert(make_pair(TILE_WOOD_STAGE2_KEY, TILE_WOOD_STAGE2_LEFTTOP));
	m_tileselect_map.insert(make_pair(TILE_STAGE_EDGE_KEY, TILE_STAGE_EDGE_LEFTTOP));*/

	// 오브젝트 종류 추가
	/*m_objectselect_map.insert(make_pair(OBJECT_NONE_KEY, OBJECT_NONE_UNUSING_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_STONE_COLUMN_KEY, OBJECT_STONE_COLUMN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_STAGE_STAIR_KEY, OBJECT_STAGE_STAIR_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_LEFT_STAINED_GLASS_KEY, OBJECT_LEFT_STAINED_GLASS_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_RIGHT_STAINED_GLASS_KEY, OBJECT_RIGHT_STAINED_GLASS_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_NORMAL_CURTAIN_KEY, OBJECT_NORMAL_CURTAIN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_SHAPED_CURTAIN_KEY, OBJECT_SHAPED_CURTAIN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_SIDE_CURTAIN_KEY, OBJECT_SIDE_CURTAIN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_LEFTTOP_CURTAIN_KEY, OBJECT_LEFTTOP_CURTAIN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_RIGHTTOP_CURTAIN_KEY, OBJECT_RIGHTTOP_CURTAIN_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_PRINCIPAL_KEY, OBJECT_PRINCIPAL_INFO));
	m_objectselect_map.insert(make_pair(OBJECT_SITTING_STUDENT_KEY, OBJECT_SITTING_STUDENT_LEFTTOP));*/

	// 타일 옵션 추가
	/*m_optionselect_map.insert(make_pair(OPTION_NORMAL_KEY, OPTION_NORMAL_UNUSING_LEFTTOP));
	m_optionselect_map.insert(make_pair(OPTION_NOMOVE_KEY, OPTION_NOMOVE_IMAGE_LEFTTOP));
	m_optionselect_map.insert(make_pair(OPTION_FRONT_OBJECT_KEY, OPTION_FRONT_OBJECT_IMAGE_LEFTTOP));*/

	// 단축키 생성
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'S', EDITSCENE_SAVE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'L', EDITSCENE_TILEMAP_LOAD))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'C', EDITSCENE_TILEMAP_CHANGE_LEFTTOP))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'T', EDITSCENE_EVENT_TRIGGER_MODE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'F', EDITSCENE_FIX_EVENT_TRIGGER_MODE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_CONTROL, 'N', EDITSCENE_EVENT_TRIGGER_NODESAVE))
		return FALSE;

	// 편집 모드 키 생성
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_F1, EDITSCENE_TILEMAP_TEXTUREMODE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_F2, EDITSCENE_TILEMAP_OBJECTMODE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_F3, EDITSCENE_TILEMAP_OPTIONMODE))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_F4, EDITSCENE_TILEMAP_TRIGGERMODE))
		DestroyWindow(GET_HWND());

	// 선택 키 생성
	if (!GET_SINGLE(Input_Manager)->Add_Key('1', EDITSCENE_TILEMAP_NUMBER_1))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('2', EDITSCENE_TILEMAP_NUMBER_2))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('3', EDITSCENE_TILEMAP_NUMBER_3))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('4', EDITSCENE_TILEMAP_NUMBER_4))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('5', EDITSCENE_TILEMAP_NUMBER_5))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('6', EDITSCENE_TILEMAP_NUMBER_6))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('7', EDITSCENE_TILEMAP_NUMBER_7))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('8', EDITSCENE_TILEMAP_NUMBER_8))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('9', EDITSCENE_TILEMAP_NUMBER_9))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('0', EDITSCENE_TILEMAP_NUMBER_0))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('P', EDITSCENE_TILEMAP_NUMBER_P))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key('O', EDITSCENE_TILEMAP_NUMBER_O))
		return FALSE;

	return TRUE;
}

void Gamenovel_Edit_Scene::Input(const float _deltatime) {
	Scene::Input(_deltatime);

	if (GET_KEYDOWN(EDITSCENE_SAVE)) {
		switch (m_tilemap_editing_mode) {
		case TEM_TILEMAP:
			ShowCursor(TRUE);
			DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG1), GET_HWND(), Gamenovel_Edit_Scene::Filename_DlgProc);
			ShowCursor(FALSE);

			if (!m_tilemap->Save(m_filename))
				DestroyWindow(GET_HWND());
			break;
		case TEM_EVENT_TRIGGER:
			strcpy_s(m_filename_tmp, m_filename);

			ShowCursor(TRUE);
			DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG1), GET_HWND(), Gamenovel_Edit_Scene::Filename_DlgProc);
			ShowCursor(FALSE);

			if (!Event_Trigger_Save(m_filename))
				DestroyWindow(GET_HWND());
			break;
		}
	}

	else if (GET_KEYDOWN(EDITSCENE_EVENT_TRIGGER_MODE)) {
		m_tilemap_editing_mode = TEM_EVENT_TRIGGER;
		m_vector_vector_index = 0;

		ShowCursor(TRUE);
		DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG1), GET_HWND(), Gamenovel_Edit_Scene::Filename_DlgProc);
		ShowCursor(FALSE);

		if (!m_tilemap->Clone_Load_Init(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
			DestroyWindow(GET_HWND());

		Layer* layer = Find_Layer(DEFAULT_LAYER_KEY);

		SAFE_RELEASE(m_tilemap_tmp);
		m_tilemap_tmp = (Tilemap*)Create_Clone(TILEMAP_PROTOTYPE_KEY, EDITSCENE_TILEMAP_TMP_CLONE_KEY, layer);
		if (!m_tilemap_tmp->Clone_Load_Init(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
			DestroyWindow(GET_HWND());
		m_tilemap_tmp->Set_Enable(FALSE);
	}

	else if (GET_KEYDOWN(EDITSCENE_FIX_EVENT_TRIGGER_MODE)) {
		m_tilemap_editing_mode = TEM_EVENT_TRIGGER;
		m_vector_vector_index = 0;

		ShowCursor(TRUE);
		DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG1), GET_HWND(), Gamenovel_Edit_Scene::Filename_DlgProc);
		ShowCursor(FALSE);

		if (!Event_Trigger_Load(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
			DestroyWindow(GET_HWND());
	}

	else if (GET_KEYDOWN(EDITSCENE_EVENT_TRIGGER_NODESAVE) && m_tilemap_editing_mode == TEM_EVENT_TRIGGER) {
		vector<POSITION> trigger_vector;
		vector<POSITION> indexchanges_vector;
		size_t numwidth = m_tilemap->Get_Tilemap_Numwidth();
		size_t numheight = m_tilemap->Get_Tilemap_Numheight();

		for (size_t j = 0; j < numheight; j++) {
			for (size_t i = 0; i < numwidth; i++) {
				if (m_vector_vector_index == 0) {
					if (m_tilemap->Get_Tile_Event_Trigger(i, j))
						trigger_vector.push_back(POSITION(i, j));
				}
				else {
					if (m_tilemap->Get_Tile_Event_Trigger(i, j) != m_tilemap_tmp->Get_Tile_Event_Trigger(i, j))
						trigger_vector.push_back(POSITION(i, j));
				}

				if ((m_tilemap->Get_Tile_Image_Lefttop(i, j) != m_tilemap_tmp->Get_Tile_Image_Lefttop(i, j)) ||
					(m_tilemap->Get_Tile_Object_Lefttop(i, j) != m_tilemap_tmp->Get_Tile_Object_Lefttop(i, j)) ||
					(m_tilemap->Get_Tile_Option_Lefttop(i, j) != m_tilemap_tmp->Get_Tile_Option_Lefttop(i, j))) {

					indexchanges_vector.push_back(POSITION(i, j));
					indexchanges_vector.push_back(m_tilemap->Get_Tile_Image_Lefttop(i, j));
					indexchanges_vector.push_back(m_tilemap->Get_Tile_Object_Lefttop(i, j));
					indexchanges_vector.push_back(m_tilemap->Get_Tile_Option_Lefttop(i, j));
				}
			}
		}

		trigger_vector.push_back(EDITSCENE_TRIGGER_END_FLOATPOINT);
		indexchanges_vector.push_back(EDITSCENE_INDEXCHANGES_END_FLOATPOINT);

		m_trigger_vector_vector.push_back(trigger_vector);
		m_indexchanges_vector_vector.push_back(indexchanges_vector);

		m_vector_vector_index++;
		SAFE_RELEASE(m_tilemap_tmp);
		m_tilemap_tmp = m_tilemap->Clone();
		m_tilemap_tmp->Set_Enable(FALSE);

		// 이벤트 트리거 수정 모드일 경우 실행된다.
		// 다음 이벤트 트리거 노드를 로드해 타일맵을 변경시킨다.
		if (!m_load_trigger_vector_vector.empty() && m_vector_vector_index <= m_load_trigger_vector_vector.size() - 1) {
			for (size_t i = 0; i < m_load_trigger_vector_vector[m_vector_vector_index].size(); i++)
				m_tilemap->Set_Tile_Event_Trigger(m_load_trigger_vector_vector[m_vector_vector_index][i]);

			POSITION tileindex;
			for (size_t i = 0; i < m_load_indexchanges_vector_vector[m_vector_vector_index].size(); i++) {
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
		}

		ShowCursor(TRUE);
		MessageBox(GET_HWND(), "성공적으로 트리거 노드가 저장되었습니다", "알림", MB_OK);
		ShowCursor(FALSE);
	}

	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_CHANGE_LEFTTOP) && m_tilemap_editing_mode == TEM_TILEMAP) {
		ShowCursor(TRUE);
		DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG2), GET_HWND(), Gamenovel_Edit_Scene::Floatpoint_DlgProc);
		ShowCursor(FALSE);

		m_tilemap->Set_Tilemap_Lefttop(strtod(m_numwidth_str, NULL), strtod(m_numheight_str, NULL));
	}

	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_LOAD)) {
		if (m_tile_editing_component == TEC_EVENT_TRIGGER)
			m_tile_editing_component = TEC_TEXTURE;

		m_tilemap_editing_mode = TEM_TILEMAP;

		ShowCursor(TRUE);
		DialogBox(GET_HINST(), MAKEINTRESOURCE(IDD_DIALOG1), GET_HWND(), Gamenovel_Edit_Scene::Filename_DlgProc);
		ShowCursor(FALSE);

		if (!m_tilemap->Clone_Load_Init(m_filename, TILESET_IMAGE_KEY, TILESET_IMAGE_FILENAME, RGB_WHITE))
			DestroyWindow(GET_HWND());
	}

	if (GET_KEYDOWN(EDITSCENE_TILEMAP_TEXTUREMODE))
		m_tile_editing_component = TEC_TEXTURE;
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_OBJECTMODE))
		m_tile_editing_component = TEC_OBJECT;
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_OPTIONMODE))
		m_tile_editing_component = TEC_OPTION;
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_TRIGGERMODE) && m_tilemap_editing_mode == TEM_EVENT_TRIGGER)
		m_tile_editing_component = TEC_EVENT_TRIGGER;

	/*if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_1)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_NONE_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_NONE_KEY;
			break;
		case TEC_OPTION:
			m_option_nowkey = OPTION_NORMAL_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_2)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_HALL_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_STONE_COLUMN_KEY;
			break;
		case TEC_OPTION:
			m_option_nowkey = OPTION_NOMOVE_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_3)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_RED_SHEET_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_STAGE_STAIR_KEY;
			break;
		case TEC_OPTION:
			m_option_nowkey = OPTION_FRONT_OBJECT_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_4)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_WOOD_STAGE1_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_LEFT_STAINED_GLASS_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_5)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_WOOD_STAGE2_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_RIGHT_STAINED_GLASS_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_6)) {
		switch (m_tile_editing_component) {
		case TEC_TEXTURE:
			m_tile_nowkey = TILE_STAGE_EDGE_KEY;
			break;
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_NORMAL_CURTAIN_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_7)) {
		switch (m_tile_editing_component) {
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_SHAPED_CURTAIN_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_8)) {
		switch (m_tile_editing_component) {
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_SIDE_CURTAIN_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_9)) {
		switch (m_tile_editing_component) {
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_LEFTTOP_CURTAIN_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_0)) {
		switch (m_tile_editing_component) {
		case TEC_OBJECT:
			m_object_nowkey = OBJECT_RIGHTTOP_CURTAIN_KEY;
			break;
		}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_P)) {
	switch (m_tile_editing_component) {
	case TEC_OBJECT:
		m_object_nowkey = OBJECT_PRINCIPAL_KEY;
		break;
	}
	}
	else if (GET_KEYDOWN(EDITSCENE_TILEMAP_NUMBER_O)) {
	switch (m_tile_editing_component) {
	case TEC_OBJECT:
		m_object_nowkey = OBJECT_SITTING_STUDENT_KEY;
		break;
	}
	}*/

	if (GET_KEYDOWNPRESSED(MOUSE_LEFT_KEY) && !m_same_tile) {
		POSITION mousepos = GET_MOUSEPOS();

		if (PointRect_Collision(mousepos, m_tilemap->Get_Tilemap_Rect())) {
			POSITION tileindex = m_tilemap->Get_Tile_Index(mousepos);
			if (tileindex == TILEMAP_INDEX_ON_LINE)
				return;

			m_same_tile = TRUE;
			m_tileindex_tmp = tileindex;

			switch (m_tile_editing_component) {
			case TEC_TEXTURE:
				m_tilemap->Set_Tile_Lefttop(tileindex.x, tileindex.y, m_tileselect_map.find(m_tile_nowkey)->second);
				break;
			case TEC_OBJECT:
			{
				const OBJECT_INFO object_info = m_objectselect_map.find(m_object_nowkey)->second;

				if (object_info.Included_To_Tilemap(m_tilemap->Get_Tile_Lefttop(tileindex), m_tilemap->Get_Tilemap_Rect()))
					m_tilemap->Set_Tile_Object_Lefttop(tileindex.x, tileindex.y, object_info);
			}
			break;
			case TEC_OPTION:
				m_tilemap->Set_Tile_Option_Lefttop(tileindex.x, tileindex.y, m_optionselect_map.find(m_option_nowkey)->second);
				break;
			case TEC_EVENT_TRIGGER:
				m_tilemap->Set_Tile_Event_Trigger(tileindex.x, tileindex.y);
				break;
			}
		}
	}
}

void Gamenovel_Edit_Scene::Update(const float _deltatime) {
	Scene::Update(_deltatime);

	if (m_same_tile) {
		POSITION mousepos = GET_MOUSEPOS();
		POSITION tileindex = m_tilemap->Get_Tile_Index(mousepos);
		if (tileindex == TILEMAP_INDEX_ON_LINE)
			return;

		if (m_tileindex_tmp != tileindex)
			m_same_tile = FALSE;
	}
}

void Gamenovel_Edit_Scene::Render(const HDC & _hDC) {
	Scene::Render(_hDC);

	m_tilemap->Object_Render(_hDC, OP_FRONT);
	m_tilemap->Option_Render(_hDC);
	if (m_tilemap_editing_mode == TEM_EVENT_TRIGGER)
		m_tilemap->Event_Trigger_Render(_hDC);

	POSITION mousepos = GET_MOUSEPOS();
	FLOATRECT tile_rect;

	if (PointRect_Collision(mousepos, m_tilemap->Get_Tilemap_Rect())) {
		POSITION mouse_tileindex;
		mouse_tileindex = m_tilemap->Get_Tile_Index(mousepos);
		if (mouse_tileindex == TILEMAP_INDEX_ON_LINE)
			return;

		if (m_tile_editing_component == TEC_OBJECT) {
			const FLOATPOINT tilenum_2D = m_objectselect_map.find(m_object_nowkey)->second.Get_Tilenum_2D();

			POSITION tileindex;
			for (size_t j = 0; j < tilenum_2D.y; j++) {
				for (size_t i = 0; i < tilenum_2D.x; i++) {
					tileindex = POSITION(mouse_tileindex.x + i, mouse_tileindex.y + j);
					tile_rect = m_tilemap->Get_Tile_Rect(tileindex);

					if (!RectRect_Collision(tile_rect, m_tilemap->Get_Tilemap_Rect()))
						continue;

					HPEN red_pen = CreatePen(PS_SOLID, 1, RGB_RED);
					HPEN pen_tmp = (HPEN)SelectObject(_hDC, red_pen);
					MoveToEx(_hDC, tile_rect.left, tile_rect.top, NULL);
					LineTo(_hDC, tile_rect.right, tile_rect.top);
					LineTo(_hDC, tile_rect.right, tile_rect.bottom);
					LineTo(_hDC, tile_rect.left, tile_rect.bottom);
					LineTo(_hDC, tile_rect.left, tile_rect.top);
					SelectObject(_hDC, pen_tmp);
					DeleteObject(red_pen);
				}
			}
		}
		else {
			tile_rect = m_tilemap->Get_Tile_Rect(mouse_tileindex);

			HPEN red_pen = CreatePen(PS_SOLID, 1, RGB_RED);
			HPEN pen_tmp = (HPEN)SelectObject(_hDC, red_pen);
			MoveToEx(_hDC, tile_rect.left, tile_rect.top, NULL);
			LineTo(_hDC, tile_rect.right, tile_rect.top);
			LineTo(_hDC, tile_rect.right, tile_rect.bottom);
			LineTo(_hDC, tile_rect.left, tile_rect.bottom);
			LineTo(_hDC, tile_rect.left, tile_rect.top);
			SelectObject(_hDC, pen_tmp);
			DeleteObject(red_pen);
		}
	}
}