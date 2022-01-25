#pragma once

#include "../../Pieces/Pieces/Button/Rect_Button.h"

class Before_Battle_Scene;
class Inventory_UI;

class Inventory_Button : public Rect_Button {
	// 변수
	Before_Battle_Scene*	m_scene = NULL;
	Inventory_UI*			m_inventory_UI = NULL;

	string					m_item_key = "";
	POSITION				m_original_pos = POSITION();
	bool					m_selected = FALSE;
	size_t					m_selecting_boxnum = -1;
	size_t					m_num = -1;
public:
	// 함수
	Inventory_Button();
	Inventory_Button(Inventory_Button & _button);
	~Inventory_Button();

	void		Click();

	string		Get_Item_Key() const									{ return m_item_key; }
	POSITION	Get_Original_Pos() const								{ return m_original_pos; }
	bool		Get_Selected() const									{ return m_selected; }
	void		Set_Scene(Before_Battle_Scene * const _scene)			{ m_scene = _scene; }
	void		Set_Inventory_UI(Inventory_UI * const _inventory_UI)	{ m_inventory_UI = _inventory_UI; }
	void		Set_Item_Key(const string & _item_key)					{ m_item_key = _item_key; }
	void		Set_Original_Pos(const POSITION & _pos)					{ m_original_pos = _pos; }
	void		Set_Num(const size_t _num)								{ m_num = _num; }

	void		Input(const float _deltatime)							{ Rect_Button::Input(_deltatime); }
	void		Late_Update()											{ Rect_Button::Late_Update(); }
	void		Render(const HDC & _hDC)								{ Rect_Button::Render(_hDC); }
};

