#pragma once

#include "../../Pieces/Bases/Reference.h"
#include "../../Singleton/Path_Manager.h"
#include "../../Singleton/Resource_Manager/Resource_Manager.h"

#define ANIMATION_TIMETIME_UNUSE		-1.f

class Object;
class Texture;

enum ANIMATION_TYPE {
	AT_ATLAS,
	AT_FRAME,
	AT_END
};

enum ANIMATION_OPTION {
	AO_LOOP,
	AO_ONCE,
	AO_ONCE_RETURN,
	AO_ONCE_DESTROY,
	AO_ONCE_CALLBACK_ONLY,
	AO_ONCE_CALLBACK_RETURN,
	AO_ONCE_CALLBACK_DESTROY,
	AO_ONCE_OBJECT_CALLBACK_RETURN,
	A0_ONCE_OBJECT_CALLBACK_DESTROY,
	AO_ONCE_STRING_CALLBACK_RETURN,
	AO_ONCE_STRING_CALLBACK_DESTROY,
	AO_TIME,
	AO_TIME_RETURN,
	AO_TIME_DESTROY,
	AO_END
};

struct ANIMATIONCLIP {
	ANIMATION_TYPE					type;
	ANIMATION_OPTION				option;
	Texture*						texture = NULL;			// ������ �̹����� ����Ű���� ��������.
	float							passed_oncetime;
	float							oncetime;			// ����Ϸ��� ��� �������� �� ���� ��µǴ� �� �ɸ��� �ϰ� ���� �ð��̴�.
	float							passed_timetime;
	float							timetime;			// �ִϸ��̼��� �󸶳� ��������� �� ������ ����ִ´�.

	int								maxframe_x;
	int								maxframe_y;
	int								nowframe_x;
	int								nowframe_y;
	int								startframe_x;
	int								startframe_y;
	int								lastframe_x;		// startframe���� �� ���� �������� ����� ������ �����Ѵ�.
	int								lastframe_y;

	float							passed_oneframe_time;
	float							oneframe_time;

	size_t							callback_index;		// ANIMATION_OPTION�� �ش�Ǵ� ������ callback_vector���� ����� �Լ��� �ִ� �ε���

	~ANIMATIONCLIP();
};

struct CLIP_INPUT {
	ANIMATION_OPTION	option;
	float				timetime;

	int					maxframe_x;
	int					maxframe_y;
	int					startframe_x;
	int					startframe_y;
	int					lastframe_x;
	int					lastframe_y;
};

class Animation : public Reference {
protected:
	// Variables
	unordered_map<string, ANIMATIONCLIP*>	m_clipmap;
	ANIMATIONCLIP*							m_nowclip;
	string									m_nowclip_key;
	string									m_default_key;
	int										m_FPS;
	bool									m_playing;

	string									m_strtag;
	Object *								m_object;

	vector<function<void()>>				m_void_callback_vector = vector<function<void()>>(0);
	vector<function<void(Object * const)>>	m_object_callback_vector = vector<function<void(Object * const)>>(0);
	vector<function<void(const string &)>>	m_string_callback_vector = vector<function<void(const string &)>>(0);

	Object *								m_target = NULL;
	string									m_input_string = "";
public:
	// Functions
	Animation();
	Animation(const Animation &_animation);
	virtual ~Animation();

	Animation*				Clone();

	virtual bool			Add_Atlas_Clip(const string &_clip_key, const CLIP_INPUT &_clip, const string &_texture_key,
								const TCHAR * const _filename, const COLORREF &_color = TEXTURE_COLORKEY_UNUSE,
								const string &_path_key = SPRITE_PATH_KEY);
	bool					Set_Default(const string &_clip_key);
	bool					Set_Nowclip(const string &_clip_key);
	ANIMATIONCLIP*			Find_Clip(const string &_clip_key);
	void					Stop_To(const POSITION & _frame_pos)							{ Stop_To(_frame_pos.x, _frame_pos.y); }
	void					Stop_To(const int _frame_x, const int _frame_y);

	void					Set_FPS(const float _FPS)										{ m_FPS = _FPS; }
	void					Set_Strtag(const string &_ani_key)								{ m_strtag = _ani_key; }
	void					Set_Object(Object * const _object)								{ m_object = _object; }
	ANIMATIONCLIP*			Get_Nowclip() const												{ return m_nowclip; }
	string					Get_Nowclip_Key() const											{ return m_nowclip_key; }
	string					Get_Strtag() const												{ return m_strtag; }
	Object*					Get_Object() const												{ return m_object; } // ������ ���� ������� �� ��
	FLOATSIZE				Get_Frame_Size() const;

	template<typename T>
	void					Set_Callback(const string &_clip_key, T * const _object, void(T::* const _func)());
	template<typename T>
	void					Set_Callback(const string &_clip_key, T * const _object, void(T::* const _func)(Object * const));
	template<typename T>
	void					Set_Callback(const string & _clip_key, T * const _object, void(T::* const _func)(const string &));

	void					Set_Target(Object * const _object)								{ m_target = _object; }
	void					Set_Input_String(const string & _string)						{ m_input_string = _string; }

	void					Update(const float _deltatime);
};

template<typename T>
inline void Animation::Set_Callback(const string &_clip_key, T * const _object, void(T::* const _func)()) {
	m_void_callback_vector.push_back(bind(_func, _object));
	ANIMATIONCLIP* clip = Find_Clip(_clip_key);
	clip->callback_index = m_void_callback_vector.size() - 1;
}

template<typename T>
inline void Animation::Set_Callback(const string &_clip_key, T * const _object, void(T::* const _func)(Object * const)) {
	m_object_callback_vector.push_back(bind(_func, _object, placeholders::_1));
	ANIMATIONCLIP* clip = Find_Clip(_clip_key);
	clip->callback_index = m_object_callback_vector.size() - 1;
}

template<typename T>
inline void Animation::Set_Callback(const string & _clip_key, T * const _object, void(T::* const _func)(const string &)) {
	m_string_callback_vector.push_back(bind(_func, _object, placeholders::_1));
	ANIMATIONCLIP* clip = Find_Clip(_clip_key);
	clip->callback_index = m_string_callback_vector.size() - 1;
}