#pragma once

#include "Button.h"
#include "../../../Singleton/Collision_Manager/Circle_Collider.h"

#define CIRCLE_BUTTON_PROTOTYPE_KEY				"Circle_Button_Prototype"
#define CIRCLE_BUTTON_CLICKRANGE_KEY			"Circle_Button_Clickrange"

class Circle_Button : public Button {
public:
	// Functions
	Circle_Button();
	Circle_Button(const Circle_Button &_button);
	virtual ~Circle_Button();

	Circle_Button*	Clone()							{ return new Circle_Button(*this); }
	template <typename T>
	bool			Clone_Init(const POSITION &_image_lefttop, const FLOATSIZE &_size, const POSITION &_pos, T *_object, void(T::*_func)());

	virtual bool	Init();
	virtual void	Input(const float _deltatime)	{ Button::Input(_deltatime); }
	virtual void	Update(const float _deltatime)	{ Button::Update(_deltatime); }
	virtual void	Late_Update()					{ Button::Late_Update(); }
	virtual void	Render(const HDC &_hDC)			{ Button::Render(_hDC); }
};

template<typename T>
inline bool Circle_Button::Clone_Init(const POSITION & _image_lefttop, const FLOATSIZE & _size, const POSITION & _pos, T * _object, void(T::* _func)()) {
	FLOATRECT rect = Get_Rect();

	if (rect.right - rect.left != rect.bottom - rect.top)
		return FALSE;

	FLOATCIRCLE circle;
	circle.radius = (rect.right - rect.left) / 2;
	circle.x = rect.left + circle.radius;
	circle.y = rect.top + circle.radius;

	m_image_lefttop = _image_lefttop;
	m_size = _size;
	((Circle_Collider*)(*m_collist.begin()))->Set_Origin_Circle(circle);
	m_pos = _pos;
	m_callback = bind(_func, _object);

	return TRUE;
}
