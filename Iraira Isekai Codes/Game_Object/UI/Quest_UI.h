#pragma once

#include "../../Pieces/Pieces/Panal.h"

#define QUEST_UI_PROTOTYPE_KEY				"Quest_UI_Prototype_Key"

// 퀘스트 UI를 구성하는 오브젝트들의 Lefttop은 퀘스트 UI 페이퍼의 Lefttop 기준임을 주의하자
#define QUEST_UI_MAX_PAGENUM				4
#define QUEST_UI_MAX_QUESTNUM				4
#define QUEST_UI_QUESTBUTTON1_LEFTTOP		POSITION(300, 100)
#define QUEST_UI_QUESTBUTTON_YSHIFT			49
#define QUEST_UI_UPDOWNBUTTON_YSHIFT		10
#define QUEST_UI_PAGELIGHT1_LEFTTOP			POSITION(630, 127)
#define QUEST_UI_PAGELIGHT_YSHIFT			20

class Rect_Button;
class Quest_Button;

// 플레이어 이름과 초상화, 아이템박스, 사이드바를 제외한 UI의 모든 부분이다.
// m_texture로 퀘스트 UI 페이퍼를 가진다.
class Quest_UI : public Panal {
	// 변수
	string			m_monster_skinkey = "";
	size_t			m_pagenum = 0; // 배열과의 헷갈림을 방지하기 위해 내부적으로는 0페이지부터 시작하는 것으로 생각한다.
	size_t			m_selected_num = -1;
	Quest_Button***	m_quest_pages = NULL;
	Rect_Button*	m_up_button = NULL;
	Rect_Button*	m_down_button = NULL;
public:
	Quest_UI();
	Quest_UI(Quest_UI & _ui);
	~Quest_UI();

	Quest_UI*		Clone()																{ return new Quest_UI(*this); }
	bool			Clone_Init(const POSITION &_pos, const POSITION & _image_lefttop,
						const FLOATSIZE & _size, const string &_texture_key);

	void			Add_Quest(const string & _string, const string & _skinkey);
	void			Quest_Page_Up();
	void			Quest_Page_Down();

	void			Set_Monster_Skinkey(const string & _skinkey)						{ m_monster_skinkey = _skinkey; }
	void			Set_Selected_Num(const size_t _num)									{ m_selected_num = _num; }
	string			Get_Monster_Skinkey() const											{ return m_monster_skinkey; }
	size_t			Get_Selected_Num() const											{ return m_selected_num; }
	Quest_Button*	Get_Selected_Quest_Button() const									{ return m_quest_pages[m_pagenum][m_selected_num]; }

	bool			Init();
	void			Input(const float _deltatime);
	void			Late_Update();
	void			Render(const HDC & _hDC);
};

