#pragma once

#include "Player.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Input_Manager/Input_Manager.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Battle_Scene.h"
#include "../Monster/Monster.h"

// 무기
#include "../Item/Weapon/Normal Weapon/Wood.h"
#include "../Item/Weapon/Normal Weapon/Knife.h"
#include "../Item/Weapon/Normal Weapon/Baguette.h"

// 방패
#include "../Item/Shield/Normal_Shield/Bag.h"

// 물약
#include "../Item/Finite/Potion/Orange_Bottle.h"

Player::Player() : m_reducing_HP_bar_onetime(1.f / 24), m_superarmor_trans_onetime(0.13f) {}
Player::Player(Player & _player) : Moving(_player),
m_reducing_HP_bar_onetime(_player.m_reducing_HP_bar_onetime), m_superarmor_trans_onetime(_player.m_superarmor_trans_onetime) {

	SAFE_MAP_DELETE(m_bag);

	m_name = _player.m_name;
	m_HP = _player.m_HP;

	unordered_map<string, Equipment*>::iterator it;
	unordered_map<string, Equipment*>::iterator it_end = _player.m_bag.end();
	for (it = _player.m_bag.begin(); it != it_end; it++)
		m_bag.insert(make_pair(it->first, new Equipment(*(it->second))));

	m_icon_texture = _player.m_icon_texture;
	m_icon_texture->Add_Ref();
	m_cooltime_blendfunc = _player.m_cooltime_blendfunc;

	m_HP_bar_alphaDC = _player.m_HP_bar_alphaDC;
	m_HP_bar_alphabitmap_tmp = _player.m_HP_bar_alphabitmap_tmp;
	m_HP_bar_texture = _player.m_HP_bar_texture;
	m_HP_bar_texture->Add_Ref();
	m_HP_bar_blendfunc = _player.m_HP_bar_blendfunc;
	m_reducing_HP_bar_blendfunc = _player.m_reducing_HP_bar_blendfunc;

	m_finite_font_tmp = _player.m_finite_font_tmp;

	m_command_vector_vector = _player.m_command_vector_vector;
}
Player::~Player() {
	SAFE_MAP_DELETE(m_bag);
	SAFE_RELEASE(m_icon_texture);
	SAFE_RELEASE(m_HP_bar_texture);

	Texture * back_buffer = GET_SINGLE(Resource_Manager)->Get_BackBuffer();
	HDC back_DC = back_buffer->Get_MemDC();

	DeleteObject(SelectObject(m_HP_bar_alphaDC, m_HP_bar_alphabitmap_tmp));
	DeleteDC(m_HP_bar_alphaDC);

	DeleteObject(SelectObject(back_DC, m_finite_font_tmp));

	SAFE_RELEASE(back_buffer);
}

bool Player::Clone_Init(const string _name, const POSITION& _init_lefttop) {
	m_name = _name;
	m_pos = _init_lefttop + Get_Pivotsize();
	m_init_pos = m_pos;

	return TRUE;
}

bool Player::Equip(const string & _input_key, const string & _item_key) {
	Equipment * equipment = NULL;

	if (_item_key == WOOD_KEY) {
		equipment = new Wood(_item_key, WOOD_EARLY_CLIP_KEY, WOOD_LATER_CLIP_KEY, 3.f, 20, 40, WOOD_ICON_IMAGE_LEFTTOP);
		equipment->Set_Player(this);

		CLIP_INPUT wood_front_clip;
		wood_front_clip.option = AO_ONCE_CALLBACK_ONLY;
		wood_front_clip.maxframe_x = 7;
		wood_front_clip.maxframe_y = 3;
		wood_front_clip.startframe_x = 0;
		wood_front_clip.startframe_y = 0;
		wood_front_clip.lastframe_x = 4;
		wood_front_clip.lastframe_y = 0;
		if (!m_animation->Add_Atlas_Clip(WOOD_EARLY_CLIP_KEY, wood_front_clip,
			WOOD_SPRITE_KEY, WOOD_SPRITE_FILENAME, RGB_MAGENTA)) {
			return FALSE;
		}

		CLIP_INPUT wood_later_clip;
		wood_later_clip.option = AO_ONCE_CALLBACK_ONLY;
		wood_later_clip.maxframe_x = 7;
		wood_later_clip.maxframe_y = 3;
		wood_later_clip.startframe_x = 5;
		wood_later_clip.startframe_y = 0;
		wood_later_clip.lastframe_x = 6;
		wood_later_clip.lastframe_y = 0;
		if (!m_animation->Add_Atlas_Clip(WOOD_LATER_CLIP_KEY, wood_later_clip,
			WOOD_SPRITE_KEY, WOOD_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		m_animation->Set_Callback<Wood>(WOOD_EARLY_CLIP_KEY, (Wood*)equipment,
			&Wood::Use);
		m_animation->Set_Callback<Player>(WOOD_LATER_CLIP_KEY, this, &Player::Move);
	}
	else if (_item_key == KNIFE_KEY) {
		equipment = new Knife(_item_key, KNIFE_EARLY_CLIP_KEY, KNIFE_LATER_CLIP_KEY, 5.f, 60, 80, KNIFE_ICON_IMAGE_LEFTTOP);
		equipment->Set_Player(this);

		CLIP_INPUT knife_front_clip;
		knife_front_clip.option = AO_ONCE_CALLBACK_ONLY;
		knife_front_clip.maxframe_x = 7;
		knife_front_clip.maxframe_y = 3;
		knife_front_clip.startframe_x = 0;
		knife_front_clip.startframe_y = 1;
		knife_front_clip.lastframe_x = 3;
		knife_front_clip.lastframe_y = 1;
		if (!m_animation->Add_Atlas_Clip(KNIFE_EARLY_CLIP_KEY, knife_front_clip,
			KNIFE_SPRITE_KEY, KNIFE_SPRITE_FILENAME, RGB_MAGENTA)) {
			return FALSE;
		}

		CLIP_INPUT knife_later_clip;
		knife_later_clip.option = AO_ONCE_CALLBACK_ONLY;
		knife_later_clip.maxframe_x = 7;
		knife_later_clip.maxframe_y = 3;
		knife_later_clip.startframe_x = 4;
		knife_later_clip.startframe_y = 1;
		knife_later_clip.lastframe_x = 5;
		knife_later_clip.lastframe_y = 1;
		if (!m_animation->Add_Atlas_Clip(KNIFE_LATER_CLIP_KEY, knife_later_clip,
			KNIFE_SPRITE_KEY, KNIFE_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		m_animation->Set_Callback<Knife>(KNIFE_EARLY_CLIP_KEY, (Knife*)equipment,
			&Knife::Use);
		m_animation->Set_Callback<Player>(KNIFE_LATER_CLIP_KEY, this, &Player::Move);
	}
	else if (_item_key == BAGUETTE_KEY) {
		equipment = new Baguette(_item_key, BAGUETTE_EARLY_CLIP_KEY, BAGUETTE_LATER_CLIP_KEY, 1.f, 40, 60, BAGUETTE_ICON_IMAGE_LEFTTOP);
		equipment->Set_Player(this);

		CLIP_INPUT baguette_front_clip;
		baguette_front_clip.option = AO_ONCE_CALLBACK_ONLY;
		baguette_front_clip.maxframe_x = 7;
		baguette_front_clip.maxframe_y = 3;
		baguette_front_clip.startframe_x = 0;
		baguette_front_clip.startframe_y = 2;
		baguette_front_clip.lastframe_x = 4;
		baguette_front_clip.lastframe_y = 2;
		if (!m_animation->Add_Atlas_Clip(BAGUETTE_EARLY_CLIP_KEY, baguette_front_clip,
			BAGUETTE_SPRITE_KEY, BAGUETTE_SPRITE_FILENAME, RGB_MAGENTA)) {
			return FALSE;
		}

		CLIP_INPUT baguette_later_clip;
		baguette_later_clip.option = AO_ONCE_CALLBACK_ONLY;
		baguette_later_clip.maxframe_x = 7;
		baguette_later_clip.maxframe_y = 3;
		baguette_later_clip.startframe_x = 5;
		baguette_later_clip.startframe_y = 2;
		baguette_later_clip.lastframe_x = 6;
		baguette_later_clip.lastframe_y = 2;
		if (!m_animation->Add_Atlas_Clip(BAGUETTE_LATER_CLIP_KEY, baguette_later_clip,
			BAGUETTE_SPRITE_KEY, BAGUETTE_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		m_animation->Set_Callback<Baguette>(BAGUETTE_EARLY_CLIP_KEY, (Baguette*)equipment,
			&Baguette::Use);
		m_animation->Set_Callback<Player>(BAGUETTE_LATER_CLIP_KEY, this, &Player::Move);
	}
	else if (_item_key == BAG_KEY) {
		equipment = new Bag(_item_key, BAG_EARLY_CLIP_KEY, BAG_LATER_CLIP_KEY, 10, BAG_ICON_IMAGE_LEFTTOP);
		equipment->Set_Player(this);

		CLIP_INPUT shield_early_clip;
		shield_early_clip.option = AO_ONCE_CALLBACK_ONLY;
		shield_early_clip.maxframe_x = 9;
		shield_early_clip.maxframe_y = 1;
		shield_early_clip.startframe_x = 0;
		shield_early_clip.startframe_y = 0;
		shield_early_clip.lastframe_x = 8;
		shield_early_clip.lastframe_y = 0;
		if (!m_animation->Add_Atlas_Clip(BAG_EARLY_CLIP_KEY, shield_early_clip,
			BAG_EARLY_SPRITE_KEY, BAG_EARLY_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		CLIP_INPUT shield_later_clip;
		shield_later_clip.option = AO_ONCE_CALLBACK_RETURN;
		shield_later_clip.maxframe_x = 7;
		shield_later_clip.maxframe_y = 1;
		shield_later_clip.startframe_x = 0;
		shield_later_clip.startframe_y = 0;
		shield_later_clip.lastframe_x = 6;
		shield_later_clip.lastframe_y = 0;
		if (!m_animation->Add_Atlas_Clip(BAG_LATER_CLIP_KEY, shield_later_clip,
			BAG_LATER_SPRITE_KEY, BAG_LATER_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		m_animation->Set_Callback<Bag>(BAG_EARLY_CLIP_KEY, (Bag*)equipment,
			&Bag::Use);
		m_animation->Set_Callback<Player>(BAG_LATER_CLIP_KEY, this, &Player::Finish_Using);
	}
	else if (_item_key == ORANGE_BOTTLE_KEY) {
		equipment = new Orange_Bottle(_item_key, ORANGE_BOTTLE_EARLY_CLIP_KEY, 3, ORANGE_BOTTLE_ICON_IMAGE_LEFTTOP);
		equipment->Set_Player(this);

		CLIP_INPUT potion_early_clip;
		potion_early_clip.option = AO_ONCE_CALLBACK_RETURN;
		potion_early_clip.maxframe_x = 9;
		potion_early_clip.maxframe_y = 1;
		potion_early_clip.startframe_x = 0;
		potion_early_clip.startframe_y = 0;
		potion_early_clip.lastframe_x = 8;
		potion_early_clip.lastframe_y = 0;
		if (!m_animation->Add_Atlas_Clip(ORANGE_BOTTLE_EARLY_CLIP_KEY, potion_early_clip,
			ORANGE_BOTTLE_SPRITE_KEY, ORANGE_BOTTLE_SPRITE_FILENAME, RGB_MAGENTA))
			return FALSE;

		m_animation->Set_Callback<Orange_Bottle>(ORANGE_BOTTLE_EARLY_CLIP_KEY, (Orange_Bottle*)equipment, &Orange_Bottle::Use);
	}

	m_bag.insert(pair<string, Equipment*>(_input_key, equipment));
	///////////////////////////////// 테스트 ////////////////////////////////////////////////////////////////////////

	// if(_item_key == ....)-....-else // 아이템 클래스 생성
	// if(_input_key == ....)-....-else // 가방에 아이템 넣기
	// if-else 문에서 아이템 종류에 따라 아이템 클래스를 생성하고 가방의 적절한 위치(인덱스 아님, 키임)에 넣자.
	// 공격, 후딜레이 애니메이션의 클립의 생성과 콜백 함수 등록은 Equip을 호출한 각 씬의 Init에서 따로 해주자.

	const string early_clip_key = _item_key + Equipment::Get_Early_Clip_Key_Part();
	const string later_clip_key = _item_key + Equipment::Get_Later_Clip_Key_Part();

	return TRUE;
}

Equipment * Player::Find_Equipment(const string & _key) {
	unordered_map<string, Equipment*>::iterator it;
	unordered_map<string, Equipment*>::iterator it_end = m_bag.end();

	for (it = m_bag.begin(); it != it_end; it++) {
		if (_key == it->first)
			return it->second;
	}

	return NULL;
}

void Player::Move() {
	m_movable = FALSE;

	// 피격 애니메이션 클립의 콜백 함수가 Player::Move이므로
	// 피격 애니메이션 출력 직후에 죽는 애니메이션을 출력하려면
	// Player::Move에서 Nowclip을 설정 해주어야 한다.
	if (m_HP <= 0) {
		m_animation->Set_Nowclip(PLAYER_DIE_CLIP_KEY);
		
		// 몬스터가 아닌 몬스터의 부수 오브젝트를 공격했을 때에는 오브젝트의 본체를 찾아 처리해주어야 한다.
		if (m_target->Get_Strtag() == MONSTER_CLONE_KEY)
			((Monster*)m_target)->Set_Skill_Active(FALSE);

		return;
	}

	switch (m_moving) {
	case MT_STOPPED:
		m_combo_possible = FALSE;
		Finish_Using();	// 뒤로 이동하는 애니메이션 후에 호출되는 함수이다.
		m_animation->Set_Nowclip(PLAYER_BATTLE_IDLE_CLIP_KEY);
		break;
	case MT_FRONT:
		m_animation->Set_Nowclip(PLAYER_FRONTMOVE_CLIP_KEY);
		break;
	case MT_BACK:
		m_combo_possible = FALSE;
		m_animation->Set_Nowclip(PLAYER_BACKMOVE_CLIP_KEY);
		break;
	}
}

void Player::Use() {
	Equipment * equipment = Find_Equipment(m_pressed_itemkey);
	if (!equipment)
		return;

	if (equipment->Get_Type() == ET_POTION && ((Finite*)equipment)->Get_Usable_Number() == 0)
		return;

	m_moving_timer = 0.f;
	m_moving = MT_STOPPED;
	m_movable = FALSE;
	
	const string pressed_itemname = equipment->Get_Name();

	if (!m_combo_possible) {
		vector<vector<string>>::iterator it;
		vector<vector<string>>::iterator it_end = m_command_vector_vector.end();
		size_t index = 0;
		for (it = m_command_vector_vector.begin(); it != it_end; it++, index++) {
			if ((*it)[0] == pressed_itemname) {
				m_combo_possible = TRUE;
				m_command_outer_index_vector.push_back(index);
			}
		}

		if (m_command_outer_index_vector.size() != 0)
			m_command_inner_index = 1;
	}

	m_animation->Set_Nowclip(equipment->Get_Early_Clip_Key());
}

void Player::Finish_Using() {
	m_pressed_itemkey = PLAYER_NOTPRESSED_KEY;
	m_moving_timer = 0.f;
	m_moving = MT_STOPPED;
	m_movable = TRUE;
	m_command_outer_index_vector.clear();
}

void Player::Dodge(const DODGE_TYPE _dodge_type) {
	m_movable = FALSE;
	m_dodging = TRUE;

	switch (_dodge_type) {
	case DT_JUMP:
		m_animation->Set_Nowclip(PLAYER_JUMP_CLIP_KEY);
		break;
	case DT_DOWN:
		m_animation->Set_Nowclip(PLAYER_DOWN_CLIP_KEY);
		break;
	}
}

void Player::Finish_Dodging() {
	m_movable = TRUE;
	m_dodging = FALSE;
}

void Player::Attack(const int _damage) {
	if (m_target->Get_Strtag() == MONSTER_CLONE_KEY) {
		if (((Monster*)m_target)->Get_HP() <= 0)
			return;

		((Monster*)m_target)->Damaged(_damage);
	}
}

void Player::Damaged(const int _damage) {
	// 회피 중이거나 무적 상태일 때는 이 함수 자체가 호출되지 않는다.
	m_damaging = TRUE;

	int damage = _damage;

	if (m_shield_state)
		damage -= ((Shield*)Find_Equipment(m_pressed_itemkey))->Get_Barrier();
	else {
		m_moving_timer = 0.f;
		m_movable = FALSE;
		m_dodging = TRUE;

		m_animation->Set_Nowclip(PLAYER_ATTACKED_CLIP_KEY);

		if (m_pos.x == m_target->Get_Lefttop_Pos().x + Get_Pivotsize().x + PLAYER_ATTACK_X_SHIFT)
			m_moving = MT_BACK;
		else if (m_pos.x == BATTLE_PLAYER_CLONE_INIT_LEFTTOP.x + Get_Pivotsize().x && m_moving == MT_BACK)
			m_moving = MT_STOPPED;
		else if (m_moving == MT_FRONT)
			m_moving = MT_STOPPED;
	}

	if (damage >= m_HP) {
		damage = m_HP;
		m_damaging = FALSE;
		m_visible = TRUE;
		m_moving = MT_STOPPED;
	}

	m_HP -= damage;
	m_reducing_HP += damage;
}

void Player::Heal(const int _heal) {
	if (_heal <= 0)
		return;

	m_HP += _heal;
	if (m_HP > PLAYER_INIT_MAXHP)
		m_HP = PLAYER_INIT_MAXHP;

	m_reducing_HP -= _heal;
	if (m_reducing_HP < 0)
		m_reducing_HP = 0;
}

void Player::Add_Combo(const size_t _outer_index, const size_t _itemkey_num, ...) {
	va_list ap;
	va_start(ap, _itemkey_num);

	string itemkey_tmp = "";	// string 변수를 선언해 문자열을 먼저 받아주는 과정이 없으면 런타임 에러가 난다.
	vector<string> command = vector<string>(0);
	for (size_t i = 1; i <= _itemkey_num; i++) {
		itemkey_tmp = va_arg(ap, char *);
		command.push_back(itemkey_tmp);
	}
	m_command_vector_vector.push_back(command);

	va_end(ap);
}

void Player::Die() {
	///////////////// 추후 게임 오버 장면 출력할 것
	m_animation->Stop_To(8, 0);
}

void Player::Win() {
	///////////////// 추후 게임 승리 장면 출력할 것
	m_animation->Set_Nowclip(PLAYER_WIN_IDLE_CLIP_KEY);
}

string Player::Get_Now_Command_Itemname() {
	string debug = m_command_vector_vector[m_command_outer_index_vector[0]][m_command_inner_index - 1];

	return m_command_vector_vector[m_command_outer_index_vector[0]][m_command_inner_index - 1];
}

bool Player::Init() {
	Create_Animation(PLAYER_ANIMATION_KEY);

	CLIP_INPUT battle_idle_clip;
	battle_idle_clip.option = AO_LOOP;
	battle_idle_clip.maxframe_x = 8;
	battle_idle_clip.maxframe_y = 1;
	battle_idle_clip.startframe_x = 0;
	battle_idle_clip.startframe_y = 0;
	battle_idle_clip.lastframe_x = 7;
	battle_idle_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_BATTLE_IDLE_CLIP_KEY, battle_idle_clip,
		PLAYER_BATTLE_IDLE_SPRITE_KEY, PLAYER_BATTLE_IDLE_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT jump_clip;
	jump_clip.option = AO_ONCE_CALLBACK_RETURN;
	jump_clip.maxframe_x = 7;
	jump_clip.maxframe_y = 1;
	jump_clip.startframe_x = 0;
	jump_clip.startframe_y = 0;
	jump_clip.lastframe_x = 6;
	jump_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_JUMP_CLIP_KEY, jump_clip,
		PLAYER_JUMP_SPRITE_KEY, PLAYER_JUMP_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT down_clip;
	down_clip.option = AO_ONCE_CALLBACK_RETURN;
	down_clip.maxframe_x = 7;
	down_clip.maxframe_y = 1;
	down_clip.startframe_x = 0;
	down_clip.startframe_y = 0;
	down_clip.lastframe_x = 6;
	down_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_DOWN_CLIP_KEY, down_clip,
		PLAYER_DOWN_SPRITE_KEY, PLAYER_DOWN_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}
	
	CLIP_INPUT frontmove_clip;
	frontmove_clip.option = AO_ONCE_CALLBACK_ONLY;
	frontmove_clip.maxframe_x = 8;
	frontmove_clip.maxframe_y = 1;
	frontmove_clip.startframe_x = 0;
	frontmove_clip.startframe_y = 0;
	frontmove_clip.lastframe_x = 7;
	frontmove_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_FRONTMOVE_CLIP_KEY, frontmove_clip,
		PLAYER_FRONTMOVE_SPRITE_KEY, PLAYER_FRONTMOVE_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT backmove_clip;
	backmove_clip.option = AO_ONCE_CALLBACK_RETURN;
	backmove_clip.maxframe_x = 8;
	backmove_clip.maxframe_y = 1;
	backmove_clip.startframe_x = 0;
	backmove_clip.startframe_y = 0;
	backmove_clip.lastframe_x = 7;
	backmove_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_BACKMOVE_CLIP_KEY, backmove_clip,
		PLAYER_BACKMOVE_SPRITE_KEY, PLAYER_BACKMOVE_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT attacked_clip;
	attacked_clip.option = AO_ONCE_CALLBACK_ONLY;
	attacked_clip.maxframe_x = 5;
	attacked_clip.maxframe_y = 1;
	attacked_clip.startframe_x = 0;
	attacked_clip.startframe_y = 0;
	attacked_clip.lastframe_x = 4;
	attacked_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_ATTACKED_CLIP_KEY, attacked_clip,
		PLAYER_ATTACKED_SPRITE_KEY, PLAYER_ATTACKED_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT die_clip;
	die_clip.option = AO_ONCE_CALLBACK_ONLY;
	die_clip.maxframe_x = 9;
	die_clip.maxframe_y = 1;
	die_clip.startframe_x = 0;
	die_clip.startframe_y = 0;
	die_clip.lastframe_x = 8;
	die_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_DIE_CLIP_KEY, die_clip,
		PLAYER_DIE_SPRITE_KEY, PLAYER_DIE_SPRITE_FILENAME, RGB_MAGENTA)) {
		return FALSE;
	}

	CLIP_INPUT win_clip;
	win_clip.option = AO_ONCE_CALLBACK_ONLY;
	win_clip.maxframe_x = 18;
	win_clip.maxframe_y = 1;
	win_clip.startframe_x = 0;
	win_clip.startframe_y = 0;
	win_clip.lastframe_x = 17;
	win_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_WIN_CLIP_KEY, win_clip,
		PLAYER_WIN_SPRITE_KEY, PLAYER_WIN_SPRITE_FILENAME, RGB_MAGENTA2)) {
		return FALSE;
	}

	CLIP_INPUT win_idle_clip;
	win_idle_clip.option = AO_LOOP;
	win_idle_clip.maxframe_x = 16;
	win_idle_clip.maxframe_y = 1;
	win_idle_clip.startframe_x = 0;
	win_idle_clip.startframe_y = 0;
	win_idle_clip.lastframe_x = 15;
	win_idle_clip.lastframe_y = 0;
	if (!m_animation->Add_Atlas_Clip(PLAYER_WIN_IDLE_CLIP_KEY, win_idle_clip,
		PLAYER_WIN_IDLE_SPRITE_KEY, PLAYER_WIN_IDLE_SPRITE_FILENAME, RGB_MAGENTA2)) {
		return FALSE;
	}

	m_animation->Set_Nowclip(PLAYER_BATTLE_IDLE_CLIP_KEY);

	m_size = m_animation->Get_Frame_Size();
	m_pivot = PLAYER_PIVOT;

	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_BAG1_COMMAND, PLAYER_BAG1_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_BAG2_COMMAND, PLAYER_BAG2_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_BAG3_COMMAND, PLAYER_BAG3_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_BAG4_COMMAND, PLAYER_BAG4_KEY))
		return FALSE;

	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_JUMPDODGE_COMMAND, PLAYER_JUMPDODGE_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(PLAYER_DOWNDODGE_COMMAND, PLAYER_DOWNDODGE_KEY))
		return FALSE;

	Texture * back_buffer = GET_SINGLE(Resource_Manager)->Get_BackBuffer();
	HDC back_DC = back_buffer->Get_MemDC();

	m_icon_texture = GET_SINGLE(Resource_Manager)->Load_Texture(EQUIPMENT_ICON_IMAGE_KEY, EQUIPMENT_ICON_IMAGE_FILENAME, RGB_MAGENTA2);
	if (!m_icon_texture)
		return FALSE;

	m_cooltime_blendfunc.BlendOp = 0;
	m_cooltime_blendfunc.BlendFlags = 0;
	m_cooltime_blendfunc.AlphaFormat = AC_SRC_OVER;
	m_cooltime_blendfunc.SourceConstantAlpha = PLAYER_COOLTIME_ALPHA;

	m_HP_bar_texture = GET_SINGLE(Resource_Manager)->Load_Texture(PLAYER_HP_BAR_TEXTURE_KEY, PLAYER_HP_BAR_TEXTURE_FILENAME, RGB_MAGENTA);
	if (!m_HP_bar_texture)
		return FALSE;

	FLOATSIZE HP_bar_texture_size = m_HP_bar_texture->Get_Frame_Size();

	m_HP_bar_alphaDC = CreateCompatibleDC(back_DC);
	HBITMAP HP_bar_alphabitmap = CreateCompatibleBitmap(back_DC, HP_bar_texture_size.x, HP_bar_texture_size.y);
	m_HP_bar_alphabitmap_tmp = (HBITMAP)SelectObject(m_HP_bar_alphaDC, HP_bar_alphabitmap);
	TransparentBlt(m_HP_bar_alphaDC, 0, 0, HP_bar_texture_size.x, HP_bar_texture_size.y,
		m_HP_bar_texture->Get_MemDC(), 0, 0, HP_bar_texture_size.x, HP_bar_texture_size.y, RGB_MAGENTA);

	m_HP_bar_blendfunc.BlendOp = 0;
	m_HP_bar_blendfunc.BlendFlags = 0;
	m_HP_bar_blendfunc.AlphaFormat = AC_SRC_OVER;
	m_HP_bar_blendfunc.SourceConstantAlpha = PLAYER_HP_BAR_ALPHA;

	m_reducing_HP_bar_blendfunc.BlendOp = 0;
	m_reducing_HP_bar_blendfunc.BlendFlags = 0;
	m_reducing_HP_bar_blendfunc.AlphaFormat = AC_SRC_OVER;
	m_reducing_HP_bar_blendfunc.SourceConstantAlpha = PLAYER_REDUCING_HP_BAR_ALPHA;

	m_finite_font_tmp = (HFONT)SelectObject(back_DC, CreateFont(PLAYER_FINITE_FONT_WIDTH, 0, 0, 0, PLAYER_FINITE_FONT_WEIGHT,
								0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, PLAYER_FINITE_FONT_SHAPE));

	SAFE_RELEASE(back_buffer);

	return TRUE;
}

void Player::Input(const float _deltatime) {
	if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_JUMPDODGE_KEY) && m_movable) {
		Dodge(DT_JUMP);
		return;
	}
	else if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_DOWNDODGE_KEY) && m_movable) {
		Dodge(DT_DOWN);
		return;
	}

	Equipment * equipment = NULL;
	const string pressed_itemkey_tmp = m_pressed_itemkey;
	bool itemkey_pressed = FALSE;

	if (GET_SINGLE(Input_Manager)->Get_Up(m_pressed_itemkey)) {
		equipment = Find_Equipment(m_pressed_itemkey);
		if (equipment->Get_Type() == ET_SHIELD)
			((Shield*)equipment)->Unuse();

		equipment = NULL;
	}

	if (!m_movable && !m_combo_possible)
		return;

	if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_BAG1_KEY)) {
		m_pressed_itemkey = PLAYER_BAG1_KEY;
		itemkey_pressed = TRUE;
	}
	else if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_BAG2_KEY)) {
		m_pressed_itemkey = PLAYER_BAG2_KEY;
		itemkey_pressed = TRUE;
	}
	else if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_BAG3_KEY)) {
		m_pressed_itemkey = PLAYER_BAG3_KEY;
		itemkey_pressed = TRUE;
	}
	else if (GET_SINGLE(Input_Manager)->Get_Down(PLAYER_BAG4_KEY)) {
		m_pressed_itemkey = PLAYER_BAG4_KEY;
		itemkey_pressed = TRUE;
	}

	equipment = Find_Equipment(m_pressed_itemkey);
	if (!equipment) {
		m_pressed_itemkey = PLAYER_NOTPRESSED_KEY;
		return;
	}

	if (equipment && equipment->Get_Type() == ET_WEAPON && ((Weapon*)equipment)->Get_Cooltime_State()) {
		equipment = NULL;
		return;
	}

	if (m_combo_possible && m_pressed_itemkey != pressed_itemkey_tmp) {
		string now_command_itemname = "";
		string pressed_itemname = m_bag.find(m_pressed_itemkey)->second->Get_Name();
		bool found = FALSE;
		vector<size_t> changed_index_vector = vector<size_t>(0);

		vector<size_t>::iterator it;
		vector<size_t>::iterator it_end = m_command_outer_index_vector.end();

		for (it = m_command_outer_index_vector.begin(); it != it_end; it++) {
			now_command_itemname = m_command_vector_vector[*it][m_command_inner_index];

			if (now_command_itemname == pressed_itemname) {
				if (!found) {
					m_bag.find(pressed_itemkey_tmp)->second->Set_Command_Pressed(TRUE);
					m_combo_possible = FALSE;
					found = TRUE;
				}

				changed_index_vector.push_back(*it);
			}
		}

		if (found)
			m_command_inner_index++;

		m_command_outer_index_vector = changed_index_vector;
	}

	if (!m_movable)
		return;

	if (itemkey_pressed) {
		if (Get_Type(m_pressed_itemkey) != ET_WEAPON)
			Use();
		else {
			m_moving = MT_FRONT;
			Move();
		}

		return;
	}
}

void Player::Update(const float _deltatime) {
	Moving::Update(_deltatime);

	switch (m_moving) {
	case MT_FRONT:
		m_moving_timer += _deltatime;
		if (m_moving_timer >= m_animation->Find_Clip(PLAYER_FRONTMOVE_CLIP_KEY)->oncetime * PLAYER_FRONTMOVE_TELEPORT_RATE) {
			m_pos.x = m_target->Get_Lefttop_Pos().x + Get_Pivotsize().x + PLAYER_ATTACK_X_SHIFT;
			m_moving_timer = 0.f;
		}
		break;
	case MT_BACK:
	{
		bool backmove_condition = FALSE;
		m_moving_timer += _deltatime;

		if (m_damaging) {
			float debug = m_animation->Find_Clip(PLAYER_ATTACKED_CLIP_KEY)->oncetime;
			backmove_condition = m_moving_timer >= m_animation->Find_Clip(PLAYER_BACKMOVE_CLIP_KEY)->oncetime
			* PLAYER_BACKMOVE_TELEPORT_RATE + m_animation->Find_Clip(PLAYER_ATTACKED_CLIP_KEY)->oncetime;
		}
		else {
			backmove_condition = m_moving_timer >= m_animation->Find_Clip(PLAYER_BACKMOVE_CLIP_KEY)->oncetime
			* PLAYER_BACKMOVE_TELEPORT_RATE + m_animation->Find_Clip(Find_Equipment(m_pressed_itemkey)->Get_Later_Clip_Key())->oncetime;
		}

		if (backmove_condition) {
			m_pos.x = m_init_pos.x;
			m_moving_timer = 0.f;
		}
	}
		break;
	}

	if (m_damaging && m_dodging) {
		m_damage_dodging_timer += _deltatime;

		if (m_damage_dodging_timer >= PLAYER_SUPERARMOR_TIME) {
			const string nowclip_key = m_animation->Get_Nowclip_Key();
			if (nowclip_key != PLAYER_JUMP_CLIP_KEY && nowclip_key != PLAYER_DOWN_CLIP_KEY)
				m_dodging = FALSE;

			m_damaging = FALSE;
			m_damage_dodging_timer = 0.f;
			m_visible = TRUE;
		}
	}

	Equipment * equipment = NULL;
	unordered_map<string, Equipment*>::iterator it;
	unordered_map<string, Equipment*>::iterator it_end = m_bag.end();

	for (it = m_bag.begin(); it != it_end; it++) {
		equipment = it->second;
		if (equipment->Get_Type() == ET_WEAPON)
			((Weapon*)equipment)->Update(_deltatime);
	}

	if (m_reducing_HP != 0) {
		m_reducing_HP_bar_timer += _deltatime;

		if (m_reducing_HP_bar_timer >= m_reducing_HP_bar_onetime) {
			m_reducing_HP -= 1;
			m_reducing_HP_bar_timer -= m_reducing_HP_bar_onetime;
		}
	}

	if (m_damaging && !m_shield_state) {
		m_superarmor_trans_timer += _deltatime;

		if (m_superarmor_trans_timer >= m_superarmor_trans_onetime) {
			if (m_visible)
				m_visible = FALSE;
			else
				m_visible = TRUE;

			m_superarmor_trans_timer -= m_superarmor_trans_onetime;
		}
	}
}