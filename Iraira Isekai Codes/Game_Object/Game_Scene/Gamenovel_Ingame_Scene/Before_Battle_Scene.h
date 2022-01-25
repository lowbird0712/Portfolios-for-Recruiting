#pragma once

#include "Gamenovel_Ingame_Scene.h"

#define BEFORE_BATTLE_BACKGROUND_TEXTURE_KEY		"Before_Battle_Background_Texture_Key"
#define BEFORE_BATTLE_BACKGROUND_TEXTURE_FILENAME	TEXT("Background_Sky.bmp")
#define BEFORE_BATTLE_BACKGROUND_CLONE_KEY			"Before_Battle_Background_Clone_Key"

#define QUEST_UI_BUTTON_CLONE_KEY					"Quest_UI_Button_Clone_Key"
#define INVENTORY_UI_BUTTON_CLONE_KEY				"Inventory_UI_Button_Clone_Key"
#define QUEST_UI_CLONE_KEY							"Quest_UI_Clone_Key"
#define INVENTORY_UI_CLONE_KEY						"Inventory_UI_Clone_Key"
#define PORTRAITBOX_CLONE_KEY						"Portraitbox_Clone_Key"
#define BATTLE_BUTTON_CLONE_KEY						"Battle_Button_Clone_Key"

#define UI_TEXTURE_KEY								"UI_Texture_Key"
#define UI_TEXTURE_FILENAME							TEXT("UI_v1.bmp")
#define UI_PAPER_TEXTURE_KEY						"UI_Paper_Texture_Key"
#define UI_PAPER_TEXTURE_FILENAME					TEXT("UI_Paper.bmp")

#define UI_QUESTPAPER_IMAGE_LEFTTOP					POSITION(0, 0)
#define UI_INVENTORYPAPER_IMAGE_LEFTTOP				POSITION(0, 368)
#define UI_ITEMBOX_IMAGE_LEFTTOP					POSITION(0, 144)
#define UI_ITEMBUTTON_IMAGE_LEFTTOP					POSITION(65, 147)
#define UI_QUESTBUTTON_UNSELECT_IMAGE_LEFTTOP		POSITION(0, 245)
#define UI_QUESTBUTTON_SELECT_IMAGE_LEFTTOP			POSITION(0, 284)
#define UI_PORTRAITBOX_NONE_IMAGE_LEFTTOP			POSITION(0, 758)
#define UI_PORTRAITBOX_GOLEM_IMAGE_LEFTTOP			POSITION(0, 934)
#define UI_PAGELIGHT_WHITE_IMAGE_LEFTTOP			POSITION(0, 128)
#define UI_PAGELIGHT_GREEN_IMAGE_LEFTTOP			POSITION(14, 128)
#define UI_QUESTUPBUTTON_IMAGE_LEFTTOP				POSITION(0, 84)
#define UI_QUESTDOWNBUTTON_IMAGE_LEFTTOP			POSITION(196, 84)
#define UI_SIDEBAR_IMAGE_LEFTTOP					POSITION(0, 370)
#define UI_BATTLEBUTTON_NORMAL_IMAGE_LEFTTOP		POSITION(352, 624)
#define UI_BATTLEBUTTON_MOUSEON_IMAGE_LEFTTOP		POSITION(0, 624)
#define UI_BATTLEBUTTON_CLICKED_IMAGE_LEFTTOP		POSITION(176, 624)

#define UI_PAPER_SIZE								FLOATSIZE(672, 368)
#define UI_ITEMBOX_SIZE								FLOATSIZE(62, 62)
#define UI_ITEMBUTTON_SIZE							FLOATSIZE(56, 56)
#define UI_QUESTBUTTON_SIZE							FLOATSIZE(322, 39)
#define UI_PORTRAITBOX_SIZE							FLOATSIZE(176, 176)
#define UI_PAGELIGHT_SIZE							FLOATSIZE(14, 16)
#define UI_QUEST_UPDOWNBUTTON_SIZE					FLOATSIZE(49, 7)
#define UI_INVENTORY_UPDOWNBUTTON_SIZE				UI_QUEST_UPDOWNBUTTON_SIZE
#define UI_SIDEBAR_SIZE								FLOATSIZE(578, 8)
#define UI_BATTLEBUTTON_SIZE						FLOATSIZE(176, 62)

#define UI_LEFTTOP									POSITION(100, 100)
#define	PLAYER_NAME_LEFTTOP							POSITION(80, 80)
#define PORTRAITBOX_LEFTTOP							POSITION(30, 100)
#define ITEMBOX1_LEFTTOP							POSITION(230, 80)
#define ITEMBOX_MAXNUM								4
#define ITEMBOX_ICON_SHIFT							FLOATSIZE(4, 4)
#define QUEST_UI_BUTTON_LEFTTOP						POSITION(49, 0)
#define INVENTORY_UI_BUTTON_LEFTTOP					POSITION(177, 0)
#define UI_BUTTON_SIZE								FLOATSIZE(126, 38)
#define BATTLEBUTTON_LEFTTOP						POSITION(600, 400)
#define BATTLEBUTTON_MOUSEON_ALPHA					100
#define BATTLEBUTTON_CLICKED_ALPHA					200

class Texture;
class Background;
class Rect_Button;
class Quest_UI;
class Inventory_UI;
class Inventory_Button;

enum UI_TYPE {
	UT_QUEST,
	UT_INVENTORY
};

class Before_Battle_Scene : public Gamenovel_Ingame_Scene {
	// 변수
	Texture*			m_UI_texture = NULL;
	Texture*			m_paper_texture = NULL;
	Background*			m_background = NULL;

	UI_TYPE				m_UI_type = UT_QUEST;
	Rect_Button*		m_quest_UI_button = NULL;
	Rect_Button*		m_inventory_UI_button = NULL;
	Quest_UI*			m_quest_UI = NULL;
	Inventory_UI*		m_inventory_UI = NULL;
	Rect_Button*		m_battle_button = NULL;

	string				m_player_name = "";
	Panal*				m_portraitbox = NULL;
	Inventory_Button**	m_selected_buttons = NULL;
public:
	// 변수
	Before_Battle_Scene();
	~Before_Battle_Scene();

	void			Goto_Quest_UI();
	void			Goto_Inventory_UI();
	void			Flush_Selected_Buttons(const size_t _index)			{ m_selected_buttons[_index] = NULL; }
	void			Start_Battle();
	
	Texture*		Get_UI_Texture() const								{ return m_UI_texture; }
	Quest_UI*		Get_Quest_UI() const								{ return m_quest_UI; }
	Inventory_UI*	Get_Inventory_UI() const							{ return m_inventory_UI; }
	Panal*			Get_Portraitbox() const								{ return m_portraitbox; }
	POSITION		Get_Itembox_Icon_Lefttop(const size_t _num) const;

	bool			Init();
	void			Input(const float _deltatime);
	void			Late_Update();
	void			Render(const HDC & _hDC);
};

