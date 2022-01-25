#include "Path_Manager.h"

DEFINITION_SINGLE(Path_Manager);

Path_Manager::Path_Manager() { }
Path_Manager::~Path_Manager() { }

bool Path_Manager::Create_Path(const string & _path_key, const TCHAR * const _adding_path, const string & _base_key) {
	const TCHAR *basepath = Find_Path(_base_key);
	if (!basepath)
		return FALSE;

	tstring strpath;
	if (basepath)
		strpath = basepath;
	strpath += _adding_path;

	m_pathmap.insert(make_pair(_path_key, strpath));

	return TRUE;
}

const TCHAR * Path_Manager::Find_Path(const string & _path_key) {
	unordered_map<string, tstring>::iterator it = m_pathmap.find(_path_key);
	if (it == m_pathmap.end())
		return NULL;

	return it->second.c_str();
}

bool Path_Manager::Init() {
	size_t count = 0;
	TCHAR operating_filepath[MAX_PATH] = {};
	GetModuleFileName(NULL, operating_filepath, MAX_PATH);

	for (int i = lstrlen(operating_filepath) - 1; i >= 0; i--) {
		if (operating_filepath[i] == '/' || operating_filepath[i] == '\\') {
			count++;
			if (count == 2) {
				operating_filepath[i + 1] = '\0';
				break;
			}
		}
	}

	m_pathmap.insert(make_pair(ROOT_PATH_KEY, operating_filepath));

	if (!Create_Path(SYSTEM_PATH_KEY, SYSTEM_PATH_VALUE))
		return FALSE;
	if (!Create_Path(IMAGE_PATH_KEY, IMAGE_PATH_VALUE))
		return FALSE;
	if (!Create_Path(SPRITE_PATH_KEY, SPRITE_PATH_VALUE))
		return FALSE;
	if (!Create_Path(SOUND_PATH_KEY, SOUND_PATH_VALUE))
		return FALSE;
	if (!Create_Path(DATA_PATH_KEY, DATA_PATH_VALUE))
		return FALSE;

	return TRUE;
}