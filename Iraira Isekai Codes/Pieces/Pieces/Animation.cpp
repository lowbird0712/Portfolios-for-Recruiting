#pragma once

#include "Animation.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../../Game_Singleton/Effect_Manager.h"
#include "../Bases/Object.h"

ANIMATIONCLIP::~ANIMATIONCLIP() { SAFE_RELEASE(texture); }

Animation::Animation() : m_nowclip(NULL), m_nowclip_key(""), m_default_key(""), m_FPS(12), m_playing(TRUE), m_strtag(""), m_object(NULL) { }
Animation::Animation(const Animation & _animation)
	: m_nowclip(NULL), m_nowclip_key(""), m_default_key(""), m_FPS(_animation.m_FPS), m_playing(TRUE),
	m_strtag(_animation.m_strtag), m_object(NULL) {

	SAFE_MAP_DELETE(m_clipmap);
	m_clipmap.clear();

	if (_animation.m_clipmap.size() == 0)
		return;

	unordered_map<string, ANIMATIONCLIP*>::const_iterator it;
	unordered_map<string, ANIMATIONCLIP*>::const_iterator it_end = _animation.m_clipmap.end();
	for (it = _animation.m_clipmap.begin(); it != it_end; it++) {
		it->second->texture->Add_Ref();

		ANIMATIONCLIP *clip = new ANIMATIONCLIP;
		*clip = *(it->second);
		m_clipmap.insert(make_pair(it->first, clip));
	}

	it = m_clipmap.find(_animation.m_nowclip_key);
	if (it != m_clipmap.end()) {
		m_nowclip = it->second;
		m_nowclip_key = _animation.m_nowclip_key;
	}

	it = m_clipmap.find(_animation.m_default_key);
	if (it != m_clipmap.end())
		m_default_key = _animation.m_default_key;

	m_void_callback_vector = _animation.m_void_callback_vector;
	m_object_callback_vector = _animation.m_object_callback_vector;
	m_string_callback_vector = _animation.m_string_callback_vector;

	SAFE_RELEASE(m_target);
}
Animation::~Animation() { SAFE_MAP_DELETE(m_clipmap); }

Animation * Animation::Clone() {
	return new Animation(*this);
}

bool Animation::Add_Atlas_Clip(const string &_clip_key, const CLIP_INPUT &_clip, const string &_texture_key,
	const TCHAR * const _filename, const COLORREF &_color, const string &_path_key) {

	ANIMATIONCLIP *clip = new ANIMATIONCLIP;

	clip->type = AT_ATLAS;
	clip->option = _clip.option;

	Texture *texture = GET_SINGLE(Resource_Manager)->Load_Texture(_texture_key, _filename, _color, _path_key);
	if (!texture) {
		SAFE_DELETE(clip);
		return FALSE;
	}
	clip->texture = texture;

	clip->passed_oncetime = 0.f;
	clip->passed_timetime = 0.f;
	clip->timetime = _clip.timetime;

	clip->maxframe_x = _clip.maxframe_x;
	clip->maxframe_y = _clip.maxframe_y;
	clip->nowframe_x = _clip.startframe_x;
	clip->nowframe_y = _clip.startframe_y;
	clip->startframe_x = _clip.startframe_x;
	clip->startframe_y = _clip.startframe_y;
	clip->lastframe_x = _clip.lastframe_x;
	clip->lastframe_y = _clip.lastframe_y;

	clip->passed_oneframe_time = 0.f;
	size_t frame_number = clip->maxframe_x * (clip->lastframe_y - clip->startframe_y) -
		clip->startframe_x +
		clip->lastframe_x + 1;
	clip->oncetime = (float)frame_number / m_FPS;
	clip->oneframe_time = clip->oncetime / frame_number;

	m_clipmap.insert(make_pair(_clip_key, clip));

	if (m_clipmap.size() == 1) {
		Set_Default(_clip_key);
		Set_Nowclip(_clip_key);
	}

	return TRUE;
}

bool Animation::Set_Default(const string & _clip_key) {
	if (m_default_key == _clip_key)
		return TRUE;

	unordered_map<string, ANIMATIONCLIP*>::iterator it = m_clipmap.find(_clip_key);
	if (it == m_clipmap.end())
		return FALSE;

	m_default_key = _clip_key;

	return TRUE;
}

bool Animation::Set_Nowclip(const string & _clip_key) {
	if (m_nowclip) {
		m_nowclip->passed_oncetime = 0.f;
		m_nowclip->passed_timetime = 0.f;
		m_nowclip->nowframe_x = m_nowclip->startframe_x;
		m_nowclip->nowframe_y = m_nowclip->startframe_y;
		m_nowclip->passed_oneframe_time = 0.f;

		if (m_nowclip_key != _clip_key) {
			unordered_map<string, ANIMATIONCLIP*>::iterator it = m_clipmap.find(_clip_key);
			if (it == m_clipmap.end())
				return FALSE;

			m_nowclip = it->second;
			m_nowclip_key = _clip_key;

			if (m_object && !m_object->Set_Texture(m_nowclip->texture)) {
				return FALSE;

				m_object->Set_Size(Get_Frame_Size());
			}
		}

		m_playing = TRUE;
	}
	else {
		unordered_map<string, ANIMATIONCLIP*>::iterator it = m_clipmap.find(_clip_key);
		if (it == m_clipmap.end())
			return FALSE;

		m_nowclip = it->second;
		m_nowclip_key = _clip_key;

		if (m_object && !m_object->Set_Texture(m_nowclip->texture))
			return FALSE;

		m_playing = TRUE;
	}

	return TRUE;
}

ANIMATIONCLIP*	Animation::Find_Clip(const string &_clip_key) {
	unordered_map<string, ANIMATIONCLIP*>::iterator it = m_clipmap.find(_clip_key);

	if (it == m_clipmap.end())
		return NULL;
	else
		return it->second;
}

void Animation::Stop_To(const int _frame_x, const int _frame_y) {
	m_nowclip->nowframe_x = _frame_x;
	m_nowclip->nowframe_y = _frame_y;
	m_playing = FALSE;
}

FLOATSIZE Animation::Get_Frame_Size() const {
	if (!m_nowclip)
		return FAILED_FLOATPOINT;
	FLOATSIZE size = m_nowclip->texture->Get_Frame_Size();

	if (m_nowclip->type == AT_ATLAS) {
		size.x /= m_nowclip->maxframe_x;
		size.y /= m_nowclip->maxframe_y;
	}

	return size;
}

void Animation::Update(const float _deltatime) {
	if (!m_nowclip)
		return;

	if (m_playing) {
		m_nowclip->passed_oneframe_time += _deltatime;
		while (m_nowclip->passed_oneframe_time >= m_nowclip->oneframe_time) {
			m_nowclip->nowframe_x++;

			if (m_nowclip->nowframe_x == m_nowclip->maxframe_x) {
				m_nowclip->nowframe_y++;
				m_nowclip->nowframe_x = 0;
			}

			if ((m_nowclip->nowframe_x == m_nowclip->lastframe_x + 1 && m_nowclip->nowframe_y == m_nowclip->lastframe_y) ||
				(m_nowclip->nowframe_x == 0 && m_nowclip->nowframe_y == m_nowclip->lastframe_y + 1)) {

				m_nowclip->nowframe_x = m_nowclip->startframe_x;
				m_nowclip->nowframe_y = m_nowclip->startframe_y;
			}

			m_nowclip->passed_oneframe_time -= m_nowclip->oneframe_time;
		}

		switch (m_nowclip->option) {
		case AO_LOOP:
			break;
		case AO_ONCE:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime)
				GET_SINGLE(Effect_Manager)->Set_Visible(FALSE);
			break;
		case AO_ONCE_RETURN:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime)
				Set_Nowclip(m_default_key);
			break;
		case AO_ONCE_DESTROY:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime)
				m_object->Die();
			break;
		case AO_ONCE_CALLBACK_ONLY:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime)
				m_void_callback_vector[m_nowclip->callback_index]();
			break;
		case AO_ONCE_CALLBACK_RETURN:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_void_callback_vector[m_nowclip->callback_index]();
				Set_Nowclip(m_default_key);
			}
			break;
		case AO_ONCE_CALLBACK_DESTROY:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_void_callback_vector[m_nowclip->callback_index]();
				m_object->Die();
			}
			break;
		case AO_ONCE_OBJECT_CALLBACK_RETURN:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_object_callback_vector[m_nowclip->callback_index](m_target);
				Set_Nowclip(m_default_key);
			}
			break;
		case A0_ONCE_OBJECT_CALLBACK_DESTROY:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_object_callback_vector[m_nowclip->callback_index](m_target);
				m_object->Die();
			}
			break;
		case AO_ONCE_STRING_CALLBACK_RETURN:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_string_callback_vector[m_nowclip->callback_index](m_input_string);
				Set_Nowclip(m_default_key);
			}
			break;
		case AO_ONCE_STRING_CALLBACK_DESTROY:
			m_nowclip->passed_oncetime += _deltatime;
			if (m_nowclip->passed_oncetime >= m_nowclip->oncetime) {
				m_string_callback_vector[m_nowclip->callback_index](m_input_string);
				m_object->Die();
			}
			break;
		case AO_TIME:
			m_nowclip->passed_timetime += _deltatime;
			if (m_nowclip->passed_timetime >= m_nowclip->timetime)
				GET_SINGLE(Effect_Manager)->Set_Visible(FALSE);
			break;
		case AO_TIME_RETURN:
			m_nowclip->passed_timetime += _deltatime;
			if (m_nowclip->passed_timetime >= m_nowclip->timetime)
				Set_Nowclip(m_default_key);
			break;
		case AO_TIME_DESTROY:
			m_nowclip->passed_timetime += _deltatime;
			if (m_nowclip->passed_timetime >= m_nowclip->timetime)
				m_object->Die();
			break;
		}
	}
}