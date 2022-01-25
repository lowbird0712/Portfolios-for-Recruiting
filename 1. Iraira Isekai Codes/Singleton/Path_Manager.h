#pragma once

#include "../Game.h"

#define ROOT_PATH_KEY			"Root_Path"
#define SYSTEM_PATH_KEY			"System_Path"
#define IMAGE_PATH_KEY			"Image_Path"
#define SPRITE_PATH_KEY			"Sprite_Path"
#define SOUND_PATH_KEY			"Sound_Path"
#define DATA_PATH_KEY			"Data_Path"

#define SYSTEM_PATH_VALUE		TEXT("Texture\\System\\")
#define IMAGE_PATH_VALUE		TEXT("Texture\\Image\\")
#define SPRITE_PATH_VALUE		TEXT("Texture\\Sprite\\")
#define SOUND_PATH_VALUE		TEXT("Sound\\")
#define DATA_PATH_VALUE			TEXT("Data\\")

class Path_Manager {
	DECLARE_SINGLE(Path_Manager)
private:
	// Variables
	unordered_map<string, tstring> m_pathmap;
public:
	// Functions
	bool			Create_Path(const string &_path_key, const TCHAR * const _adding_path, const string &_base_key = ROOT_PATH_KEY);
	const TCHAR*	Find_Path(const string &_path_key);

	bool			Init();
};