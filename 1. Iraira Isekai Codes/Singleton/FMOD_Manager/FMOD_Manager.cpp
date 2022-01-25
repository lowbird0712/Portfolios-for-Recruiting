#pragma once

#include "FMOD_Manager.h"
#include "../Core.h"

DEFINITION_SINGLE(FMOD_Manager);

FMOD_Manager::FMOD_Manager() : m_system(NULL), m_playing_BGM_key(""), m_playing_effect_key("") { }
FMOD_Manager::~FMOD_Manager() {
	unordered_map<string, SOUNDINFO*>::iterator it;
	unordered_map<string, SOUNDINFO*>::iterator it_end = m_soundmap.end();
	for (it = m_soundmap.begin(); it != it_end; it++) {
		it->second->sound->release();
		SAFE_DELETE(it->second);
	}

	m_system->close();
	m_system->release ();
}

bool FMOD_Manager::Load_Sound(const string & _sound_key, const bool _loop, const TCHAR * const _filename, const string & _path_key) {
	if (Find_Sound(_sound_key))
		return FALSE;

	SOUNDINFO *soundinfo = new SOUNDINFO;
	soundinfo->loop = _loop;
	m_soundmap.insert(make_pair(_sound_key, soundinfo));

	const TCHAR * const path = GET_SINGLE(Path_Manager)->Find_Path(_path_key);
	if (!path)
		return FALSE;
	tstring strpath = path;
	strpath += _filename;

	FMOD_MODE mode;
	if (_loop)
		mode = FMOD_LOOP_NORMAL;
	else
		mode = FMOD_DEFAULT;

	if (m_system->createSound(strpath.c_str(), mode, NULL, &soundinfo->sound) != FMOD_OK)
		return FALSE;

	return TRUE;
}

SOUNDINFO * FMOD_Manager::Find_Sound(const string & _sound_key) {
	unordered_map<string, SOUNDINFO*>::const_iterator it;
	it = m_soundmap.find(_sound_key);

	if (it == m_soundmap.end())
		return NULL;
	else
		return it->second;
}

void FMOD_Manager::Play(const string & _sound_key) {
	SOUNDINFO *soundinfo = Find_Sound(_sound_key);
	if (!soundinfo)
		DestroyWindow(GET_HWND());

	SOUND_TYPE type;
	if (soundinfo->loop) {
		type = ST_BGM;
		Stop(ST_BGM);
		m_playing_BGM_key = _sound_key;
	}
	else {
		type = ST_EFFECT;
		m_playing_effect_key = _sound_key;
	}

	if (m_system->playSound(soundinfo->sound, NULL, FALSE, &m_channels[type]) != FMOD_OK)
		DestroyWindow(GET_HWND());
}

void FMOD_Manager::Stop(const SOUND_TYPE _type) {
	switch (_type) {
	case ST_BGM :
		m_playing_BGM_key = "";
	case ST_EFFECT :
		m_playing_effect_key = "";
	}

	m_channels[_type]->stop();
}

void FMOD_Manager::Set_Volume(const SOUND_TYPE _type, const float _volume) {
	if (m_channels[_type]->setVolume(_volume) != FMOD_OK)
		DestroyWindow(GET_HWND());
}

bool FMOD_Manager::Init() {
	FMOD::System_Create(&m_system);

	if (m_system->init(FMOD_MAX_CHANNAL_NUM, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
		return FALSE;

	return TRUE;
}

void FMOD_Manager::Update() {
	if (m_system) {
		if (m_system->update() != FMOD_OK)
			DestroyWindow(GET_HWND());

		bool effect_playing;
		m_channels[ST_EFFECT]->isPlaying(&effect_playing);
		if (!effect_playing)
			m_playing_effect_key = "";
	}
}
