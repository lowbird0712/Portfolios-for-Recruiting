#pragma once

#include "Inventory_UI.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../../Pieces/Pieces/Button/Rect_Button.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Before_Battle_Scene.h"
#include "Inventory_Button.h"
#include "../Item/Weapon/Normal Weapon/Wood.h"
#include "../Item/Weapon/Normal Weapon/Knife.h"
#include "../Item/Weapon/Normal Weapon/Baguette.h"
#include "../Item/Shield/Normal_Shield/Bag.h"
#include "../Item/Finite/Potion/Orange_Bottle.h"

Inventory_UI::Inventory_UI() {}
Inventory_UI::Inventory_UI(Inventory_UI & _ui) : Panal(_ui) {
	m_inventory_top_index = _ui.m_inventory_top_index;

	m_selected_button_indexes[0] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[1] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[2] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[3] = INVENTORY_UI_DEFAULT_INDEX;

	const size_t button_num = INVENTORY_UI_MAX_PAGENUM * INVENTORY_UI_MAX_ITEMNUM;
	m_inventory_buttons = new Inventory_Button*[button_num];
	memset(m_inventory_buttons, NULL, sizeof(Inventory_Button*) * button_num);

	m_up_button = new Rect_Button(*_ui.m_up_button);
	m_down_button = new Rect_Button(*_ui.m_down_button);
}
Inventory_UI::~Inventory_UI() {
	for (size_t i = 0; i < INVENTORY_UI_MAX_PAGENUM * INVENTORY_UI_MAX_ITEMNUM; i++)
		SAFE_RELEASE(m_inventory_buttons[i]);
	delete[] m_inventory_buttons;

	SAFE_RELEASE(m_up_button);
	SAFE_RELEASE(m_down_button);
}

bool Inventory_UI::Clone_Init(const POSITION & _pos, const POSITION & _image_lefttop,
	const FLOATSIZE & _size, const string & _texture_key) {

	if (!Panal::Clone_Init(_pos, _image_lefttop, _size, _texture_key))
		return FALSE;

	const float button_xshift = (3 * INVENTORY_UI_ITEMBUTTON_XSHIFT + UI_ITEMBUTTON_SIZE.x) / 2 - UI_INVENTORY_UPDOWNBUTTON_SIZE.x / 2;

	const POSITION upbutton_lefttop = m_pos + POSITION(
		INVENTORY_UI_ITEMBUTTON1_LEFTTOP.x + button_xshift,
		INVENTORY_UI_ITEMBUTTON1_LEFTTOP.y - INVENTORY_UI_UPDOWNBUTTON_YSHIFT - UI_INVENTORY_UPDOWNBUTTON_SIZE.y
	);
	if (!m_up_button->Clone_Init(UI_QUESTUPBUTTON_IMAGE_LEFTTOP, UI_QUEST_UPDOWNBUTTON_SIZE,
		upbutton_lefttop, this, &Inventory_UI::Inventory_Page_Up))
		return FALSE;

	const POSITION downbutton_lefttop = m_pos + POSITION(
		INVENTORY_UI_ITEMBUTTON1_LEFTTOP.x + button_xshift,
		INVENTORY_UI_ITEMBUTTON1_LEFTTOP.y + 2 * INVENTORY_UI_ITEMBUTTON_YSHIFT + UI_ITEMBUTTON_SIZE.y + INVENTORY_UI_UPDOWNBUTTON_YSHIFT
	);
	if (!m_down_button->Clone_Init(UI_QUESTDOWNBUTTON_IMAGE_LEFTTOP, UI_QUEST_UPDOWNBUTTON_SIZE,
		downbutton_lefttop, this, &Inventory_UI::Inventory_Page_Down))
		return FALSE;

	// 아이템 추가. m_pos의 값이 필요하기 때문에 이 곳에서 아이템을 추가한다.
	Add_Item(KNIFE_KEY);
	Add_Item(BAGUETTE_KEY);
	Add_Item(BAG_KEY);
	Add_Item(ORANGE_BOTTLE_KEY);

	return TRUE;
}

void Inventory_UI::Add_Item(const string _item_key) {
	Texture * icon_texture = GET_SINGLE(Resource_Manager)->Find_Texture(INVENTORY_UI_ITEMICON_TEXTURE_KEY);
	Inventory_Button * item_button = m_inventory_buttons[m_inventory_top_index] = new Inventory_Button;
	item_button->Set_Texture(icon_texture);
	item_button->Set_Item_Key(_item_key);
	SAFE_RELEASE(icon_texture);

	const size_t inpage_num = m_inventory_top_index % 12;
	const size_t row_num = inpage_num / INVENTORY_UI_MAX_COLUMN;
	const size_t col_num = inpage_num % INVENTORY_UI_MAX_COLUMN;

	POSITION image_lefttop = TEXTURE_UNUSE_IMAGE_LEFTTOP;
	if (_item_key == WOOD_KEY)
		image_lefttop = WOOD_ICON_IMAGE_LEFTTOP;
	else if (_item_key == KNIFE_KEY)
		image_lefttop = KNIFE_ICON_IMAGE_LEFTTOP;
	else if (_item_key == BAGUETTE_KEY)
		image_lefttop = BAGUETTE_ICON_IMAGE_LEFTTOP;
	else if (_item_key == BAG_KEY)
		image_lefttop = BAG_ICON_IMAGE_LEFTTOP;
	else if (_item_key == ORANGE_BOTTLE_KEY)
		image_lefttop = ORANGE_BOTTLE_ICON_IMAGE_LEFTTOP;

	POSITION lefttop = m_pos + INVENTORY_UI_ITEMBUTTON1_LEFTTOP;
	lefttop += POSITION(col_num * INVENTORY_UI_ITEMBUTTON_XSHIFT, row_num * INVENTORY_UI_ITEMBUTTON_YSHIFT);
	item_button->Set_Original_Pos(lefttop);

	item_button->Clone_Init(image_lefttop, UI_ITEMBUTTON_SIZE, lefttop,
		item_button, &Inventory_Button::Click);

	item_button->Set_Scene((Before_Battle_Scene*)m_scene);
	item_button->Set_Inventory_UI(this);
	item_button->Set_Num(m_inventory_top_index);

	m_inventory_top_index++;
}

void Inventory_UI::Inventory_Page_Up() {
	if (m_pagenum > 0)
		m_pagenum--;
}

void Inventory_UI::Inventory_Page_Down() {
	if (m_pagenum < INVENTORY_UI_MAX_PAGENUM - 1)
		m_pagenum++;
}

Inventory_Button * Inventory_UI::Get_Selected_Button(const size_t _num) const {
	const size_t button_index = m_selected_button_indexes[_num];

	if (button_index == INVENTORY_UI_DEFAULT_INDEX)
		return NULL;
	else
		return m_inventory_buttons[button_index];
}

bool Inventory_UI::Init() {
	if (!Panal::Init())
		return FALSE;

	m_selected_button_indexes[0] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[1] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[2] = INVENTORY_UI_DEFAULT_INDEX;
	m_selected_button_indexes[3] = INVENTORY_UI_DEFAULT_INDEX;

	// 인벤토리 버튼 배열 생성
	Texture * texture = GET_SINGLE(Resource_Manager)->Load_Texture(INVENTORY_UI_ITEMICON_TEXTURE_KEY, INVENTORY_UI_ITEMICON_TEXTURE_FILENAME,
		RGB_MAGENTA2);
	SAFE_RELEASE(texture);

	const size_t button_num = INVENTORY_UI_MAX_PAGENUM * INVENTORY_UI_MAX_ITEMNUM;
	m_inventory_buttons = new Inventory_Button*[button_num];
	memset(m_inventory_buttons, NULL, sizeof(Inventory_Button*) * button_num);

	// 퀘스트 업다운 버튼 생성
	// 퀘스트 업다운 버튼은 프로토타입과 클론을 생성하지 않으므로 직접 초기화 함수를 호출해 주어야 한다.
	m_up_button = new Rect_Button;
	if (!m_up_button->Init())
		return FALSE;

	m_down_button = new Rect_Button;
	if (!m_down_button->Init())
		return FALSE;

	// 아이템 박스 선택 키 생성
	if (!GET_SINGLE(Input_Manager)->Add_Key(ITEMBOX1_INPUT_COMMAND, ITEMBOX1_INPUT_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(ITEMBOX2_INPUT_COMMAND, ITEMBOX2_INPUT_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(ITEMBOX3_INPUT_COMMAND, ITEMBOX3_INPUT_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(ITEMBOX4_INPUT_COMMAND, ITEMBOX4_INPUT_KEY))
		return FALSE;

	return TRUE;
}

void Inventory_UI::Input(const float _deltatime) {
	Panal::Input(_deltatime);

	if (GET_KEYDOWN(ITEMBOX1_INPUT_KEY))
		m_selecting_boxnum = 0;
	if (GET_KEYDOWN(ITEMBOX2_INPUT_KEY))
		m_selecting_boxnum = 1;
	if (GET_KEYDOWN(ITEMBOX3_INPUT_KEY))
		m_selecting_boxnum = 2;
	if (GET_KEYDOWN(ITEMBOX4_INPUT_KEY))
		m_selecting_boxnum = 3;

	for (size_t i = 0; i < 4; i++) {
		if (m_selected_button_indexes[i] != INVENTORY_UI_DEFAULT_INDEX)
			m_inventory_buttons[m_selected_button_indexes[i]]->Input(_deltatime);
	}

	const size_t start_index = m_pagenum * INVENTORY_UI_MAX_ITEMNUM;
	for (size_t i = start_index; i < start_index + 12 && m_inventory_buttons[i]; i++)
		m_inventory_buttons[i]->Input(_deltatime);

	m_up_button->Input(_deltatime);
	m_down_button->Input(_deltatime);
}

void Inventory_UI::Late_Update() {
	Panal::Late_Update();

	for (size_t i = 0; i < 4; i++) {
		if (m_selected_button_indexes[i] != INVENTORY_UI_DEFAULT_INDEX)
			m_inventory_buttons[m_selected_button_indexes[i]]->Late_Update();
	}

	const size_t start_index = m_pagenum * INVENTORY_UI_MAX_ITEMNUM;
	for (size_t i = start_index; i < start_index + 12 && m_inventory_buttons[i]; i++)
		m_inventory_buttons[i]->Late_Update();

	m_up_button->Late_Update();
	m_down_button->Late_Update();
}

void Inventory_UI::Render(const HDC & _hDC) {
	Panal::Render(_hDC);

	Texture* UI_texture = ((Before_Battle_Scene*)m_scene)->Get_UI_Texture();

	// 아이템 버튼 빈자리 출력
	POSITION pos = POSITION();

	for (size_t i = 0; i < INVENTORY_UI_MAX_COLUMN; i++) {
		for (size_t j = 0; j < INVENTORY_UI_MAX_ROW; j++) {
			pos = m_pos + INVENTORY_UI_ITEMBUTTON1_LEFTTOP +
				POSITION(i * INVENTORY_UI_ITEMBUTTON_XSHIFT, j * INVENTORY_UI_ITEMBUTTON_YSHIFT);
			TransparentBlt(_hDC, pos.x, pos.y, UI_ITEMBUTTON_SIZE.x, UI_ITEMBUTTON_SIZE.y,
				UI_texture->Get_MemDC(), UI_ITEMBUTTON_IMAGE_LEFTTOP.x, UI_ITEMBUTTON_IMAGE_LEFTTOP.y,
				UI_ITEMBUTTON_SIZE.x, UI_ITEMBUTTON_SIZE.y, UI_texture->Get_Colorkey());
		}
	}

	// 아이템 박스 빈자리 출력
	POSITION itembox1_pos = m_pos + ITEMBOX1_LEFTTOP;

	for (size_t i = 0; i < ITEMBOX_MAXNUM; i++) {
		TransparentBlt(_hDC, itembox1_pos.x, itembox1_pos.y + UI_ITEMBOX_SIZE.y * i, UI_ITEMBOX_SIZE.x, UI_ITEMBOX_SIZE.y,
			UI_texture->Get_MemDC(), UI_ITEMBOX_IMAGE_LEFTTOP.x, UI_ITEMBOX_IMAGE_LEFTTOP.y,
			UI_ITEMBOX_SIZE.x, UI_ITEMBOX_SIZE.y, UI_texture->Get_Colorkey());
	}

	// 아이템 버튼 출력
	for (size_t i = 0; i < 4; i++) {
		if (m_selected_button_indexes[i] != INVENTORY_UI_DEFAULT_INDEX)
			m_inventory_buttons[m_selected_button_indexes[i]]->Render(_hDC);
	}

	const size_t start_index = m_pagenum * INVENTORY_UI_MAX_ITEMNUM;
	for (size_t i = start_index; i < start_index + 12 && m_inventory_buttons[i]; i++)
		m_inventory_buttons[i]->Render(_hDC);

	// 인벤토리 페이지 업다운 버튼 출력
	m_up_button->Render(_hDC);
	m_down_button->Render(_hDC);

	// 페이지 라이트 출력
	POSITION light_image_lefttop = POSITION();

	for (size_t i = 0; i < INVENTORY_UI_MAX_PAGENUM; i++) {
		if (i == m_pagenum)
			light_image_lefttop = UI_PAGELIGHT_GREEN_IMAGE_LEFTTOP;
		else
			light_image_lefttop = UI_PAGELIGHT_WHITE_IMAGE_LEFTTOP;

		TransparentBlt(_hDC, m_pos.x + INVENTORY_UI_PAGELIGHT1_LEFTTOP.x, m_pos.y +
			INVENTORY_UI_PAGELIGHT1_LEFTTOP.y + INVENTORY_UI_PAGELIGHT_YSHIFT * i,
			UI_PAGELIGHT_SIZE.x, UI_PAGELIGHT_SIZE.y,
			UI_texture->Get_MemDC(), light_image_lefttop.x, light_image_lefttop.y,
			UI_PAGELIGHT_SIZE.x, UI_PAGELIGHT_SIZE.y, UI_texture->Get_Colorkey());
	}
}
