#pragma once

#include "../../Pieces/Bases/Unmoving.h"

#define MOUSE_IMAGE_KEY			"Mouse_Image"
#define MOUSE_IMAGE_FILENAME	TEXT("Umaru_Potato_Cursor_Test.bmp")
#define MOUSE_CLICKRANGE_KEY	"Mouse_Clickrange"
#define MOUSE_PIVOT				PIVOT(0.5f, 0.5f)

class Collider;

class Mouse : public Unmoving {
	// Variables
	FLOATSIZE	m_mousedir;
public:
	// Functions
	Mouse();
	Mouse(const Mouse &_mouse);
	virtual ~Mouse();

	virtual Mouse*	Clone()								{ return new Mouse(*this); }

	Collider*		Get_Mouse_Clickrange() const		{ return *m_collist.begin(); }	// 변수에 절대 집어넣지 말 것.
	FLOATSIZE		Get_Mousedir() const				{ return m_mousedir; }

	virtual bool	Init();
	virtual void	Input(const float _deltatime)		{ Unmoving::Input(_deltatime); }
	virtual void	Update(const float _deltatime);
	virtual void	Late_Update()						{ Unmoving::Late_Update(); }
	virtual void	Render(const HDC &_hDC);
};