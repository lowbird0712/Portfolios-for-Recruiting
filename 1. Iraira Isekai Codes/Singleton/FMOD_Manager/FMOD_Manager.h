#pragma once

#include "../../Game.h"
#include "../Path_Manager.h"

#define FMOD_MAX_CHANNAL_NUM	10

enum SOUND_TYPE {
	ST_BGM,
	ST_EFFECT,
	ST_END
};

struct SOUNDINFO {
	FMOD::Sound* sound;
	bool loop;
};

class FMOD_Manager {
	DECLARE_SINGLE(FMOD_Manager)
private:
	// Variables
	FMOD::System*							m_system;
	FMOD::Channel*							m_channels[ST_END];
	unordered_map<string, SOUNDINFO*>		m_soundmap;
	string									m_playing_BGM_key;
	string									m_playing_effect_key;
public:
	// Functions
	bool			Load_Sound(const string &_sound_key, const bool _loop, const TCHAR * const _filename, const string &_path_key = SOUND_PATH_KEY);
	SOUNDINFO*		Find_Sound(const string &_sound_key);

	void			Play(const string &_sound_key);
	void			Stop(const SOUND_TYPE _type);
	void			Set_Volume(const SOUND_TYPE _type, const float _volume); // 볼륨은 0.f부터 1.f까지 조절 가능하다

	string			Get_Playing_BGM_Key() const			{ return m_playing_BGM_key; }
	string			Get_Playing_Effect_Key() const		{ return m_playing_effect_key; }

	bool			Init();
	void			Update();
};