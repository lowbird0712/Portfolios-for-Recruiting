#pragma once

#include "../../Pieces/Pieces/Button/Rect_Button.h"

#define QUEST_BUTTON_STRINGSHIFT			POSITION(5, 10);
#define QUEST_BUTTON_MOUSEON_IMAGE_LEFTTOP	POSITION(0, 284)
#define QUEST_BUTTON_CLICKED_IMAGE_LEFTTOP	POSITION(0, 206)
#define QUEST_BUTTON_MOUSEON_ALPHA			100
#define QUEST_BUTTON_CLICKED_ALPHA			200

class Quest_UI;

struct QUEST {
	string	quest_string;
	string	monster_skinkey;
	bool	selected = FALSE;
};

// 사용 전 Button.h의 버튼셋 파일명을 수정해주자.
class Quest_Button : public Rect_Button {
protected:
	// 변수
	Quest_UI*	m_quest_UI = NULL;
	size_t		m_number = -1;
	QUEST*		m_quest = NULL;
public:
	// 함수
	Quest_Button();
	Quest_Button(const Quest_Button & _button);
	~Quest_Button();

	Quest_Button*	Clone()										{ return new Quest_Button(*this); }

	void			Click();

	void			Set_Quest_UI(Quest_UI * const _quest_UI)	{ m_quest_UI = _quest_UI; }
	void			Set_Number(const size_t _num)				{ m_number = _num; }
	void			Set_Selected(const bool _selected);
	QUEST*			Get_Quest() const							{ return m_quest; }

	bool			Init();
	void			Input(const float _deltatime)				{ Rect_Button::Input(_deltatime); }
	void			Late_Update()								{ Rect_Button::Late_Update(); }
	void			Render(const HDC &_hDC);
};

