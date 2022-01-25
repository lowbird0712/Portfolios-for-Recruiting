#pragma once

#include "../../Bases/Unmoving.h"

#define BUTTONSET_IMAGE_KEY						"Buttonset_Image"
#define BUTTONSET_IMAGE_FILENAME				TEXT("UI_v1.bmp")

enum BUTTON_STATE {
	BS_NORMAL,
	BS_MOUSEON,
	BS_CLICKED
};

class Button : public Unmoving {
protected:
	// Variables
	function<void()>	m_callback = NULL;
	BUTTON_STATE		m_state = BS_NORMAL;

	HDC					m_alpha_DC = NULL;
	HBITMAP				m_alpha_bitmap_tmp = NULL;

	BLENDFUNCTION		m_mouseon_blendfunc = BLENDFUNCTION();
	BLENDFUNCTION		m_clicked_blendfunc = BLENDFUNCTION();

	POSITION			m_mouseon_image_lefttop = POSITION();
	POSITION			m_clicked_image_lefttop = POSITION();
	bool				m_mouseon_use = FALSE;
	bool				m_clicked_use = FALSE;
public:
	// Functions
	Button();
	Button(const Button &_button);
	virtual ~Button() = 0;

	virtual Button*	Clone() = 0;

	void			Mouse_Enter(Collider * const _button_clickrange, Collider * const _mouse_clickrange, const float _deltatime);
	void			Mouse_Leave(Collider * const _button_clickrange, Collider * const _mouse_clickrange, const float _deltatime);

	void			Set_Callback(void(*_func)())								{ m_callback = bind(_func); }
	template<typename T>
	void			Set_Callback(T *_object, void(T::*_func)())					{ m_callback = bind(_func, _object); }
	void			Set_State(const BUTTON_STATE _state)						{ m_state = _state; }
	void			Set_Mouseon_Image_Lefttop(const POSITION & _image_lefttop)	{ m_mouseon_image_lefttop = _image_lefttop; }
	void			Set_Clicked_Image_Lefttop(const POSITION & _image_lefttop)	{ m_clicked_image_lefttop = _image_lefttop; }
	void			Set_Mouseon_Alpha(const BYTE _alpha)						{ m_mouseon_blendfunc.SourceConstantAlpha = _alpha; }
	void			Set_Clicked_Alpha(const BYTE _alpha)						{ m_clicked_blendfunc.SourceConstantAlpha = _alpha; }
	void			Set_Mouseon_Use(const bool _use)							{ m_mouseon_use = _use; }
	void			Set_Clicked_Use(const bool _use)							{ m_clicked_use = _use; }
	BUTTON_STATE	Get_State() const											{ return m_state; }

	virtual bool	Init();
	virtual void	Input(const float _deltatime);
	virtual void	Update(const float _deltatime)				{ Unmoving::Update(_deltatime); }
	virtual void	Late_Update();
	virtual void	Render(const HDC &_hDC)						{ Unmoving::Render(_hDC); }
};