#pragma once

#include "Button.h"
#include "../../../Singleton/Collision_Manager/Rect_Collider.h"

#define RECT_BUTTON_PROTOTYPE_KEY				"Rect_Button_Prototype"
#define RECT_BUTTON_CLICKRANGE_KEY				"Rect_Button_Clickrange"

class Rect_Button : public Button {
public:
	Rect_Button();
	Rect_Button(const Rect_Button &_button);
	virtual ~Rect_Button();

	Rect_Button*	Clone()							{ return new Rect_Button(*this); }
	template <typename T>
	bool			Clone_Init(const POSITION &_image_lefttop, const FLOATSIZE &_size, const POSITION &_pos, T * const _object, void(T::* const _func)());

	virtual bool	Init();
	virtual void	Input(const float _deltatime)	{ Button::Input(_deltatime); }
	virtual void	Update(const float _deltatime)	{ Button::Update(_deltatime); }
	virtual void	Late_Update()					{ Button::Late_Update(); }
	virtual void	Render(const HDC &_hDC);
};

template<typename T>
inline bool Rect_Button::Clone_Init(const POSITION & _image_lefttop, const FLOATSIZE & _size, const POSITION & _pos, T * const _object, void(T::* const _func)()) {
	m_image_lefttop = _image_lefttop;
	m_size = _size;
	((Rect_Collider*)(*m_collist.begin()))->Set_Origin_Rect(Get_Rect());
	m_pos = _pos;
	m_callback = bind(_func, _object);

	return TRUE;
}
