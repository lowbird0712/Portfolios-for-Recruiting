#pragma once

#include "../../Pieces/Bases/Moving.h"
#include "../Item/Equipment.h"

// 프로토타입, 애니메이션 키, 텍스쳐 키, 파일명
#define PLAYER_PROTOTYPE_KEY				"Player_Prototype"
#define PLAYER_ANIMATION_KEY				"Player_Animation"
#define PLAYER_HP_BAR_TEXTURE_KEY			"Player_HP_Bar_Texture_Key"
#define PLAYER_HP_BAR_TEXTURE_FILENAME		TEXT("HP_Bars.bmp")

// 애니메이션 클립 키
#define PLAYER_BATTLE_IDLE_CLIP_KEY			"Player_Battle_Idle_Clip_Key"
#define PLAYER_BATTLE_IDLE_SPRITE_KEY		"Player_Battle_Idle_Sprite_Key"
#define PLAYER_BATTLE_IDLE_SPRITE_FILENAME	TEXT("Player_Battle_Idle.bmp")

#define PLAYER_JUMP_CLIP_KEY				"Player_Jump_Clip_Key"
#define PLAYER_JUMP_SPRITE_KEY				"Player_Jump_Sprite_Key"
#define PLAYER_JUMP_SPRITE_FILENAME			TEXT("Player_Jump.bmp")

#define PLAYER_DOWN_CLIP_KEY				"Player_Down_Clip_Key"
#define PLAYER_DOWN_SPRITE_KEY				"Player_Down_Sprite_Key"
#define PLAYER_DOWN_SPRITE_FILENAME			TEXT("Player_Down.bmp")

#define PLAYER_FRONTMOVE_CLIP_KEY			"Player_Frontmove_Clip_Key"
#define PLAYER_FRONTMOVE_SPRITE_KEY			"Player_Frontmove_Sprite_Key"
#define PLAYER_FRONTMOVE_SPRITE_FILENAME	TEXT("Player_Front_Moving.bmp")

#define PLAYER_BACKMOVE_CLIP_KEY			"Player_Backmove_Clip_Key"
#define PLAYER_BACKMOVE_SPRITE_KEY			"Player_Backmove_Sprite_Key"
#define PLAYER_BACKMOVE_SPRITE_FILENAME		TEXT("Player_Back_Moving.bmp")

#define PLAYER_ATTACKED_CLIP_KEY			"Player_Attacked_Clip_Key"
#define PLAYER_ATTACKED_SPRITE_KEY			"Player_Attacked_Sprite_Key"
#define PLAYER_ATTACKED_SPRITE_FILENAME		TEXT("Player_Attacked.bmp")

#define PLAYER_DIE_CLIP_KEY					"Player_Die_Clip_Key"
#define PLAYER_DIE_SPRITE_KEY				"Player_Die_Sprite_Key"
#define PLAYER_DIE_SPRITE_FILENAME			TEXT("Player_Die.bmp")

#define PLAYER_WIN_CLIP_KEY					"Player_Win_Clip_Key"
#define PLAYER_WIN_SPRITE_KEY				"Player_Win_Sprite_Key"
#define PLAYER_WIN_SPRITE_FILENAME			TEXT("Player_Win.bmp")

#define PLAYER_WIN_IDLE_CLIP_KEY			"Player_Win_Idle_Clip_Key"
#define PLAYER_WIN_IDLE_SPRITE_KEY			"Player_Win_Idle_Sprite_Key"
#define PLAYER_WIN_IDLE_SPRITE_FILENAME		TEXT("Player_Win_Idle.bmp")

// 조작 키
#define PLAYER_NOTPRESSED_KEY				"Player_Notpressed_Key"
#define PLAYER_BAG1_KEY						"Player_Bag1_Key"
#define PLAYER_BAG1_COMMAND					'1'
#define PLAYER_BAG2_KEY						"Player_Bag2_Key"
#define PLAYER_BAG2_COMMAND					'2'
#define PLAYER_BAG3_KEY						"Player_Bag3_Key"
#define PLAYER_BAG3_COMMAND					'3'
#define PLAYER_BAG4_KEY						"Player_Bag4_Key"
#define PLAYER_BAG4_COMMAND					'4'
#define PLAYER_JUMPDODGE_KEY				"Player_Jumpdodge_Key"
#define PLAYER_JUMPDODGE_COMMAND			'W'
#define PLAYER_DOWNDODGE_KEY				"Player_Downdodge_Key"
#define PLAYER_DOWNDODGE_COMMAND			'S'

// 기타 수치
#define PLAYER_PIVOT						PIVOT(0.5f, 0.5f)
#define PLAYER_INIT_MAXHP					100
#define PLAYER_ATTACK_X_SHIFT				145
#define PLAYER_FRONTMOVE_TELEPORT_RATE		(5.f / 8.f)
#define PLAYER_BACKMOVE_TELEPORT_RATE		(5.f / 8.f)
#define PLAYER_SUPERARMOR_TIME				2.f
#define PLAYER_ICON_X_GAP					20
#define PLAYER_ICON_X_JUMP					(EQUIPMENT_ICON_SIZE.x + PLAYER_ICON_X_GAP)
#define PLAYER_BAG1_ICON_LEFTTOP			POSITION(70, 470)
#define PLAYER_COOLTIME_ALPHA				100
#define PLAYER_COOLTIME_ICON_LEFTTOP		POSITION(240, 0)
#define PLAYER_HP_BAR_LEFTTOP				POSITION(50, 50)
#define PLAYER_HP_BAR_ALPHA					100
#define PLAYER_REDUCING_HP_BAR_ALPHA		200
#define PLAYER_HP_BAR_SIZE					FLOATSIZE(354, 33)
#define PLAYER_HP_BAR_SIDE_LEFTTOP			POSITION(0, 0)
#define PLAYER_HP_BAR_YELLOW_LEFTTOP		POSITION(0, 33)
#define PLAYER_HP_BAR_RED_LEFTTOP			POSITION(0, 66)
#define PLAYER_HP_BAR_GRAY_LEFTTOP			POSITION(0, 99)
#define PLAYER_HP_BAR_GREEN_LEFTTOP			POSITION(0, 165)
#define PLAYER_FINITE_FONT_WIDTH			20
#define PLAYER_FINITE_FONT_WEIGHT			1000
#define PLAYER_FINITE_FONT_SHAPE			"맑은고딕"

class Monster;

enum MOVE_TYPE {
	MT_STOPPED,
	MT_FRONT,
	MT_BACK
};

enum DODGE_TYPE {
	DT_JUMP,
	DT_DOWN
};

class Player : public Moving {
	// 변수
	const float							m_reducing_HP_bar_onetime;
	const float							m_superarmor_trans_onetime;

	string								m_name = "";
	int									m_HP = PLAYER_INIT_MAXHP;
	int									m_reducing_HP = 0;	// 체력이 점점 줄어드는 연출을 위한 변수이다. 빨간색으로 표시될 HP의 양.
	unordered_map<string, Equipment*>	m_bag = unordered_map<string, Equipment*>(4);

	Object*								m_target = NULL;
	string								m_pressed_itemkey = PLAYER_NOTPRESSED_KEY;
	POSITION							m_init_pos = POSITION(0, 0);

	Texture*							m_icon_texture = NULL;
	BLENDFUNCTION						m_cooltime_blendfunc = BLENDFUNCTION();
	HFONT								m_finite_font_tmp = NULL;

	HDC									m_HP_bar_alphaDC = NULL;
	HBITMAP								m_HP_bar_alphabitmap_tmp = NULL;
	Texture*							m_HP_bar_texture = NULL;
	BLENDFUNCTION						m_HP_bar_blendfunc = BLENDFUNCTION();
	BLENDFUNCTION						m_reducing_HP_bar_blendfunc = BLENDFUNCTION();
	float								m_reducing_HP_bar_timer = 0.f;

	MOVE_TYPE							m_moving = MT_STOPPED;
	bool								m_movable = TRUE;
	bool								m_dodging = FALSE;
	bool								m_shield_state = FALSE;
	bool								m_damaging = FALSE;

	float								m_moving_timer = 0.f;
	float								m_damage_dodging_timer = 0.f;
	float								m_superarmor_trans_timer = 0.f;

	// 플레이어가 2명 이상이 될 일이 없으므로 플레이어 객체 자체가 콤보 매니저 역할도 수행한다.
	// 또한 현재 사용 가능한 콤보만 추가되어 있다는 점을 염두하자.
	// 콤보 해금 조건을 달성하면 Player::Add_Combo를 통해 새로운 콤보를 더해준다.
	bool								m_combo_possible = FALSE;
	vector<vector<string>>				m_command_vector_vector = vector<vector<string>>(0);
	vector<size_t>						m_command_outer_index_vector = vector<size_t>(0);
	size_t								m_command_inner_index = -1;
public:
	// 함수
	Player();
	Player(Player &_player);
	virtual ~Player();

	Player*								Clone()																{ return new Player(*this); }
	bool								Clone_Init(const string _name, const POSITION& _init_lefttop);

	bool								Equip(const string & _input_key, const string & _item_key);
	Equipment*							Find_Equipment(const string & _key);
	void								Move();
	void								Use();
	void								Finish_Using();
	void								Dodge(const DODGE_TYPE _dodge_type);
	void								Finish_Dodging();
	void								Attack(const int _damage);
	void								Damaged(const int _damage);
	void								Heal(const int _heal);
	void								Add_Combo(const size_t _outer_index, const size_t _itemkey_num, ...);
	void								Die();
	void								Win();

	void								Set_Icon_Texture(Texture * const _texture)							{ m_icon_texture = _texture; }
	void								Set_Target(Object * const _target)									{ m_target = _target; }
	void								Set_Pressed_Key(const string & _input_key)							{ m_pressed_itemkey = _input_key; }
	void								Set_Moving(const MOVE_TYPE _type)									{ m_moving = _type; }
	void								Set_Movable(const bool _movable)									{ m_movable = _movable; }
	void								Set_Shield_State(const bool _state)									{ m_shield_state = _state; }
	void								Set_Combo_Possible(const bool _possibility)							{ m_combo_possible = _possibility; }
	void								Set_Damaging(const bool _damaging)									{ m_damaging = _damaging; }
	int									Get_HP() const														{ return m_HP; }
	int									Get_Reducing_HP() const												{ return m_reducing_HP; }
	Object*								Get_Target() const													{ return m_target; } // 변수에 절대 집어넣지 말 것
	string								Get_Name(const string & _input_key) const							{ return m_bag.find(_input_key)->second->Get_Name(); }
	unordered_map<string, Equipment*>	Get_Bag() const														{ return m_bag; }
	string								Get_Pressed_Itemkey() const											{ return m_pressed_itemkey; }
	EQUIPMENT_TYPE						Get_Type(const string & _input_key) const							{ return m_bag.find(_input_key)->second->Get_Type(); }
	Texture*							Get_Icon_Texture() const											{ return m_icon_texture; }
	BLENDFUNCTION						Get_Cooltime_Blendfunc() const										{ return m_cooltime_blendfunc; }
	Texture*							Get_HP_Bar_Texture() const											{ return m_HP_bar_texture; }
	HDC									Get_HP_Bar_AlphaDC() const											{ return m_HP_bar_alphaDC; }
	BYTE								Get_HP_Bar_SourceConstantAlpha() const								{ return m_HP_bar_blendfunc.SourceConstantAlpha; }
	BLENDFUNCTION						Get_HP_Bar_Blendfunc() const										{ return m_HP_bar_blendfunc; }
	BYTE								Get_Reducing_HP_Bar_SourceConstantAlpha() const						{ return m_reducing_HP_bar_blendfunc.SourceConstantAlpha; }
	BLENDFUNCTION						Get_Reducing_HP_Bar_Blendfunc() const								{ return m_reducing_HP_bar_blendfunc; }
	float								Get_Reducing_HP_Bar_Onetime() const									{ return m_reducing_HP_bar_onetime; }
	bool								Get_Dodging() const													{ return m_dodging; }
	bool								Get_Shield_State() const											{ return m_shield_state; }
	string								Get_Now_Command_Itemname();
	size_t								Get_Command_Inner_Index() const										{ return m_command_inner_index; }

	virtual bool						Init();
	virtual void						Input(const float _deltatime);
	virtual void						Update(const float _deltatime);
	virtual void						Late_Update()														{ Moving::Late_Update(); }
	virtual void						Render(const HDC &_hDC)												{ Moving::Render(_hDC); }
};