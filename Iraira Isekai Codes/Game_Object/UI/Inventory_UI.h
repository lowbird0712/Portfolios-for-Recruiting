#pragma once

#include "../../Pieces/Pieces/Panal.h"

#define INVENTORY_UI_PROTOTYPE_KEY				"Inventory_UI_Prototype_Key"

#define INVENTORY_UI_ITEMICON_TEXTURE_KEY		"Inventory_UI_Itemicon_Texture_Key"
#define INVENTORY_UI_ITEMICON_TEXTURE_FILENAME	"Equipment_Icon.bmp"

// 인벤토리 UI를 구성하는 오브젝트들의 Lefttop은 퀘스트 UI 페이퍼의 Lefttop 기준임을 주의하자
#define INVENTORY_UI_MAX_PAGENUM				4
#define INVENTORY_UI_MAX_ITEMNUM				12
#define INVENTORY_UI_MAX_COLUMN					4
#define INVENTORY_UI_MAX_ROW					3
#define INVENTORY_UI_DEFAULT_INDEX				100
#define INVENTORY_UI_ITEMBUTTON1_LEFTTOP		POSITION(329, 100)
#define INVENTORY_UI_ITEMBUTTON_XSHIFT			69
#define INVENTORY_UI_ITEMBUTTON_YSHIFT			64
#define INVENTORY_UI_UPDOWNBUTTON_YSHIFT		10
#define INVENTORY_UI_PAGELIGHT1_LEFTTOP			POSITION(630, 127)
#define INVENTORY_UI_PAGELIGHT_YSHIFT			20

#define ITEMBOX1_INPUT_KEY						"Itembox1_Input_Key"
#define ITEMBOX1_INPUT_COMMAND					'1'
#define ITEMBOX2_INPUT_KEY						"Itembox2_Input_Key"
#define ITEMBOX2_INPUT_COMMAND					'2'
#define ITEMBOX3_INPUT_KEY						"Itembox3_Input_Key"
#define ITEMBOX3_INPUT_COMMAND					'3'
#define ITEMBOX4_INPUT_KEY						"Itembox4_Input_Key"
#define ITEMBOX4_INPUT_COMMAND					'4'

class Rect_Button;
class Inventory_Button;

class Inventory_UI : public Panal {
	// 변수
	size_t				m_pagenum = 0; // 배열과의 헷갈림을 방지하기 위해 내부적으로는 0페이지부터 시작하는 것으로 생각한다.
	size_t				m_inventory_top_index = 0;
	Inventory_Button**	m_inventory_buttons = NULL;

	size_t				m_selecting_boxnum = 0;
	bool				m_boxnum_selected[4] = {};
	size_t				m_selected_button_indexes[4] = {};

	Rect_Button*		m_up_button = NULL;
	Rect_Button*		m_down_button = NULL;
public:
	// 함수
	Inventory_UI();
	Inventory_UI(Inventory_UI & _ui);
	~Inventory_UI();

	Inventory_UI*		Clone()																{ return new Inventory_UI(*this); }
	bool				Clone_Init(const POSITION &_pos, const POSITION & _image_lefttop,
							const FLOATSIZE & _size, const string &_texture_key);

	void				Add_Item(const string _item_key);
	void				Inventory_Page_Up();
	void				Inventory_Page_Down();

	void				Set_Boxnum_Selected(const size_t _num, const bool _selected)		{ m_boxnum_selected[_num] = _selected; }
	void				Set_Selected_Button_Index(const size_t _UI_index, const size_t _button_index)
																							{ m_selected_button_indexes[_UI_index] = _button_index; }
	size_t				Get_Selecting_Boxnum() const										{ return m_selecting_boxnum; }
	bool				Get_Boxnum_Selected(const size_t _num) const						{ return m_boxnum_selected[_num]; }
	Inventory_Button*	Get_Selected_Button(const size_t _num) const;

	bool				Init();
	void				Input(const float _deltatime);
	void				Late_Update();
	void				Render(const HDC & _hDC);
};

