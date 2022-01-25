// 미완성 클래스

#pragma once

#include "Panal.h"
#include "../../Singleton/Scene_Manager/Scene.h"
#include "../Pieces/Button/Rect_Button.h"
#include "../Pieces/Button/Circle_Button.h"

class Button;

//struct Button_On_Panal {
//	POSITION	lefttop;
//	Button*		button;
//};

class Select_Panal : public Panal {
	// Variables
	vector<Button*>		m_button_vector;
public:
	// Functions
	Select_Panal();
	Select_Panal(const Select_Panal &_panal);
	virtual ~Select_Panal();

	virtual Select_Panal*	Clone()							{ return new Select_Panal(*this); }

	template <typename T>
	bool					Add_Rect_Button(const string &_proto_key, const string &_clone_key,
								const POSITION &_image_lefttop, const FLOATSIZE &_size, const POSITION &_inpanal_pos,
								T * const _object, void(T::*_func)());
	template <typename T>
	bool					Add_Circle_Button(const string &_proto_key, const string &_clone_key,
								const POSITION &_image_lefttop, const FLOATSIZE &_size, const POSITION &_inpanal_pos,
								T * const _object, void(T::*_func)());

	virtual bool			Init()							{ return TRUE; }
	virtual void			Input(const float _deltatime)	{ Unmoving::Input(_deltatime); }
	virtual void			Update(const float _deltatime)	{ Unmoving::Update(_deltatime); }
	virtual void			Late_Update()					{ Unmoving::Late_Update(); }
	virtual void			Render(const HDC &_hDC)			{ Unmoving::Render(_hDC); }
};

template<typename T>
inline bool Select_Panal::Add_Rect_Button(const string & _proto_key, const string & _clone_key,
	const POSITION & _image_lefttop, const FLOATSIZE & _size, const POSITION & _inpanal_pos,
	T * const _object, void(T::* _func)()) {

	const POSITION pos = m_pos + _inpanal_pos;

	Rect_Button* button = m_scene->Create_Clone(_proto_key, _clone_key, m_layer);
	if (!button->Clone_Init(_image_lefttop, _size, pos, _object, _func)) {
		SAFE_RELEASE(button);
		return FALSE;
	}

	SAFE_RELEASE(button);

	return TRUE;
}

template<typename T>
inline bool Select_Panal::Add_Circle_Button(const string & _proto_key, const string & _clone_key,
	const POSITION & _image_lefttop, const FLOATSIZE & _size, const POSITION & _inpanal_pos,
	T * const _object, void(T::* _func)()) {

	const POSITION pos = m_pos + _inpanal_pos;

	Circle_Button* button = m_scene->Create_Clone(_proto_key, _clone_key, m_layer);
	if (!button->Clone_Init(_image_lefttop, _size, pos, _object, _func)) {
		SAFE_RELEASE(button);
		return FALSE;
	}

	SAFE_RELEASE(button);

	return TRUE;
}
