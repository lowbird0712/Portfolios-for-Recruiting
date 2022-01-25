#pragma once

#include "Battle_Scene.h"
#include "../../../Singleton/Resource_Manager/Texture.h"
#include "../../../Pieces/Pieces/Background.h"
#include "../../../Game_Object/Monster/Monster_Manager.h"
#include "../../../Game_Singleton/Effect_Manager.h"
#include "../../Player/Player.h"
#include "../../Monster/Monster.h"
#include "../../Item/Weapon/Normal Weapon/Wood.h"
#include "../../Item/Weapon/Normal Weapon/Knife.h"
#include "../../Item/Weapon/Normal Weapon/Baguette.h"
#include "../../Item/Shield/Normal_Shield/Bag.h"
#include "../../Item/Finite/Potion/Orange_Bottle.h"
#include "Before_Battle_Scene.h"
#include "../../UI/Quest_UI.h"
#include "../../UI/Inventory_UI.h"
#include "../../UI/Inventory_Button.h"

Battle_Scene::Battle_Scene() : m_player(NULL) {}
Battle_Scene::~Battle_Scene() {
	SAFE_RELEASE(m_background);
	SAFE_RELEASE(m_player);
	SAFE_RELEASE(m_monster);

	DESTROY_SINGLE(Monster_Manager);
	DESTROY_SINGLE(Effect_Manager);
}

bool Battle_Scene::Init() {
	if (!Gamenovel_Ingame_Scene::Init())
		return FALSE;

	Before_Battle_Scene * before_battle_scene = (Before_Battle_Scene*)GET_SINGLE(Scene_Manager)->Get_Scene();

	// BGM 추가
	m_BGM_key = BATTLE_BGM_KEY;
	if (!GET_SINGLE(FMOD_Manager)->Load_Sound(m_BGM_key, TRUE, BATTLE_BGM_FILENAME))
		return FALSE;
	GET_SINGLE(FMOD_Manager)->Play(m_BGM_key);
	GET_SINGLE(FMOD_Manager)->Set_Volume(ST_BGM, 0.1f);

	// 백그라운드 추가
	Layer *layer = Find_Layer(BACKGROUND_LAYER_KEY);

	if (!Create_Prototype<Background>(BACKGROUND_PROTOTYPE_KEY))
		return FALSE;

	m_background = (Background*)Create_Clone(BACKGROUND_PROTOTYPE_KEY, BATTLE_BACKGROUND_CLONE_KEY, layer);
	if (!m_background->Clone_Init(BATTLE_BACKGROUND_TEXTURE_KEY, BATTLE_BACKGROUND_TEXTURE_FILENAME, FALSE))
		return FALSE;

	// 몬스터 추가
	layer = Find_Layer(DEFAULT_LAYER_KEY);

	if (!Create_Prototype<Monster>(MONSTER_PROTOTYPE_KEY))
		return FALSE;

	m_monster = (Monster*)Create_Clone(MONSTER_PROTOTYPE_KEY, MONSTER_CLONE_KEY, layer);

	if (!GET_SINGLE(Monster_Manager)->Init(m_monster))
		return FALSE;

	string monster_skinkey = before_battle_scene->Get_Quest_UI()->Get_Monster_Skinkey();
	if (!m_monster->Clone_Init(monster_skinkey, MONSTER_CLONE_INIT_LEFTTOP))
		return FALSE;

	// 플레이어 추가
	if (!Create_Prototype<Player>(PLAYER_PROTOTYPE_KEY))
		return FALSE;

	m_player = (Player*)Create_Clone(PLAYER_PROTOTYPE_KEY, BATTLE_PLAYER_CLONE_KEY, layer);
	if (!m_player->Clone_Init(PLAYER_NAME, BATTLE_PLAYER_CLONE_INIT_LEFTTOP))
		return FALSE;

	// 플레이어 장비 장착
	Inventory_UI * inventory_UI = before_battle_scene->Get_Inventory_UI();
	Inventory_Button * inventory_button = NULL;
	string bag_key = "";
	string item_key = "";

	for (size_t i = 0; i < 4; i++) {
		inventory_button = inventory_UI->Get_Selected_Button(i);
		if (!inventory_button)
			continue;

		switch (i) {
		case 0:
			bag_key = PLAYER_BAG1_KEY;
			break;
		case 1:
			bag_key = PLAYER_BAG2_KEY;
			break;
		case 2:
			bag_key = PLAYER_BAG3_KEY;
			break;
		case 3:
			bag_key = PLAYER_BAG4_KEY;
			break;
		}

		item_key = inventory_button->Get_Item_Key();

		if (!m_player->Equip(bag_key, item_key))
			return FALSE;
	}

	/*if (!m_player->Equip(PLAYER_BAG1_KEY, BAG_KEY))
		return FALSE;*/
	/*if (!m_player->Equip(PLAYER_BAG2_KEY, KNIFE_KEY))
		return FALSE;*/
	/*if (!m_player->Equip(PLAYER_BAG3_KEY, BAGUETTE_KEY))
		return FALSE;*/
	/*if (!m_player->Equip(PLAYER_BAG4_KEY, ORANGE_BOTTLE_KEY))
		return FALSE;*/

	// 플레이어 클립 콜백 함수 추가
	Animation * animation = m_player->Get_Animation();

	animation->Set_Callback<Player>(PLAYER_FRONTMOVE_CLIP_KEY, m_player, &Player::Use);
	animation->Set_Callback<Player>(PLAYER_BACKMOVE_CLIP_KEY, m_player, &Player::Finish_Using);
	animation->Set_Callback<Player>(PLAYER_JUMP_CLIP_KEY, m_player, &Player::Finish_Dodging);
	animation->Set_Callback<Player>(PLAYER_DOWN_CLIP_KEY, m_player, &Player::Finish_Dodging);
	animation->Set_Callback<Player>(PLAYER_ATTACKED_CLIP_KEY, m_player, &Player::Move);
	animation->Set_Callback<Player>(PLAYER_DIE_CLIP_KEY, m_player, &Player::Die);
	animation->Set_Callback<Player>(PLAYER_WIN_CLIP_KEY, m_player, &Player::Win);

	// 플레이어 콤보 추가
	m_player->Add_Combo(0, 3, KNIFE_KEY, BAGUETTE_KEY, KNIFE_KEY);

	m_player->Set_Target(m_monster);
	m_monster->Set_Target(m_player);

	// 이펙트 매니저 초기화
	if (!GET_SINGLE(Effect_Manager)->Init())
		return FALSE;

	// HP 바 텍스처 추가
	Texture * texture = GET_SINGLE(Resource_Manager)->Load_Texture(HP_BARS_ICON_TEXTURE_KEY, HP_BARS_ICON_TEXTURE_FILENAME, RGB_MAGENTA);
	if (!texture)
		return FALSE;
	SAFE_RELEASE(texture);

	return TRUE;
}

void Battle_Scene::Update(const float _deltatime) {
	Gamenovel_Ingame_Scene::Update(_deltatime);

	GET_SINGLE(Monster_Manager)->Update(_deltatime);
	GET_SINGLE(Effect_Manager)->Update(_deltatime);
}

void Battle_Scene::Render(const HDC & _hDC) {
	Gamenovel_Ingame_Scene::Render(_hDC);

	// 이펙트 렌더링
	GET_SINGLE(Effect_Manager)->Render(_hDC);

	// HP 바 렌더링
	const int HP = m_player->Get_HP();
	const int reducing_HP = m_player->Get_Reducing_HP();
	const float HP_rate = (float)HP / PLAYER_INIT_MAXHP;
	const float reducing_HP_rate = (float)reducing_HP / PLAYER_INIT_MAXHP;
	Texture * HP_bar_texture = m_player->Get_HP_Bar_Texture();

	if (reducing_HP != 0) {
		GdiAlphaBlend(_hDC, PLAYER_HP_BAR_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * HP_rate, PLAYER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * reducing_HP_rate, PLAYER_HP_BAR_SIZE.y,
			m_player->Get_HP_Bar_AlphaDC(), PLAYER_HP_BAR_RED_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate), PLAYER_HP_BAR_RED_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * reducing_HP_rate - 2, PLAYER_HP_BAR_SIZE.y, m_player->Get_Reducing_HP_Bar_Blendfunc());
	}

	if (HP == PLAYER_INIT_MAXHP) {
		TransparentBlt(_hDC, PLAYER_HP_BAR_LEFTTOP.x, PLAYER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y,
			HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_GREEN_LEFTTOP.x, PLAYER_HP_BAR_GREEN_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y, HP_bar_texture->Get_Colorkey());
	}
	else {
		GdiAlphaBlend(_hDC, PLAYER_HP_BAR_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * (HP_rate + reducing_HP_rate), PLAYER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate - HP_rate), PLAYER_HP_BAR_SIZE.y,
			m_player->Get_HP_Bar_AlphaDC(), PLAYER_HP_BAR_GRAY_LEFTTOP.x + 2, PLAYER_HP_BAR_GRAY_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate - HP_rate) - 2, PLAYER_HP_BAR_SIZE.y, m_player->Get_HP_Bar_Blendfunc());

		TransparentBlt(_hDC, PLAYER_HP_BAR_LEFTTOP.x, PLAYER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * HP_rate, PLAYER_HP_BAR_SIZE.y,
			HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_YELLOW_LEFTTOP.x, PLAYER_HP_BAR_YELLOW_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * HP_rate, PLAYER_HP_BAR_SIZE.y, HP_bar_texture->Get_Colorkey());
	}

	TransparentBlt(_hDC, PLAYER_HP_BAR_LEFTTOP.x, PLAYER_HP_BAR_LEFTTOP.y,
		PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y,
		HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_SIDE_LEFTTOP.x, PLAYER_HP_BAR_SIDE_LEFTTOP.y,
		PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y, HP_bar_texture->Get_Colorkey());

	// HP 아이콘 렌더링
	Texture * hp_bars_icon_texture = GET_SINGLE(Resource_Manager)->Find_Texture(HP_BARS_ICON_TEXTURE_KEY);
	const POSITION heart_pos = POSITION(PLAYER_HP_BAR_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x + HP_BARS_ICON_X_SHIFT, PLAYER_HP_BAR_LEFTTOP.y);
	const POSITION skull_pos = POSITION(MONSTER_HP_BAR_LEFTTOP.x - HP_BARS_ICON_SIZE.x - HP_BARS_ICON_X_SHIFT, MONSTER_HP_BAR_LEFTTOP.y);
	TransparentBlt(_hDC, heart_pos.x, heart_pos.y, HP_BARS_ICON_SIZE.x, HP_BARS_ICON_SIZE.y,
		hp_bars_icon_texture->Get_MemDC(), HP_BARS_HEART_IMAGE_LEFTTOP.x, HP_BARS_HEART_IMAGE_LEFTTOP.y, HP_BARS_ICON_SIZE.x, HP_BARS_ICON_SIZE.y,
		hp_bars_icon_texture->Get_Colorkey());
	TransparentBlt(_hDC, skull_pos.x, skull_pos.y, HP_BARS_ICON_SIZE.x, HP_BARS_ICON_SIZE.y,
		hp_bars_icon_texture->Get_MemDC(), HP_BARS_SKULL_IMAGE_LEFTTOP.x, HP_BARS_SKULL_IMAGE_LEFTTOP.y, HP_BARS_ICON_SIZE.x, HP_BARS_ICON_SIZE.y,
		hp_bars_icon_texture->Get_Colorkey());
	SAFE_RELEASE(hp_bars_icon_texture);

	// 스킬 아이콘 및 쿨타임 렌더링
	bool icon_rendercheck_array[4] = {};
	Texture * skill_icon_texture = m_player->Get_Icon_Texture();
	Texture * UI_texture = GET_SINGLE(Resource_Manager)->Find_Texture(UI_TEXTURE_KEY);
	Equipment * equipment = NULL;
	POSITION skill_icon_lefttop = POSITION();
	POSITION skill_icon_image_lefttop = POSITION();
	size_t xshift_num = 0;
	float cooltime_y_jump = 0.f;

	unordered_map<string, Equipment*> bag = m_player->Get_Bag();
	unordered_map<string, Equipment*>::const_iterator it = bag.begin();
	unordered_map<string, Equipment*>::const_iterator it_end = bag.end();

	for (; it != it_end; it++) {
		equipment = it->second;

		if (it->first == PLAYER_BAG1_KEY) {
			icon_rendercheck_array[0] = TRUE;
			xshift_num = 0;
		}
		else if (it->first == PLAYER_BAG2_KEY) {
			icon_rendercheck_array[1] = TRUE;
			xshift_num = 1;
		}
		else if (it->first == PLAYER_BAG3_KEY) {
			icon_rendercheck_array[2] = TRUE;
			xshift_num = 2;
		}
		else if (it->first == PLAYER_BAG4_KEY) {
			icon_rendercheck_array[3] = TRUE;
			xshift_num = 3;
		}

		skill_icon_lefttop = POSITION(PLAYER_BAG1_ICON_LEFTTOP.x + xshift_num * PLAYER_ICON_X_JUMP, PLAYER_BAG1_ICON_LEFTTOP.y);
		skill_icon_image_lefttop = equipment->Get_Icon_Image_Lefttop();
		TransparentBlt(_hDC, skill_icon_lefttop.x, skill_icon_lefttop.y,
			EQUIPMENT_ICON_SIZE.x, EQUIPMENT_ICON_SIZE.y, skill_icon_texture->Get_MemDC(),
			skill_icon_image_lefttop.x, skill_icon_image_lefttop.y, EQUIPMENT_ICON_SIZE.x, EQUIPMENT_ICON_SIZE.y,
			skill_icon_texture->Get_Colorkey());

		if (equipment->Get_Type() == ET_POTION) {
			string usable_number_string = to_string(((Finite*)equipment)->Get_Usable_Number());
			TextOut(_hDC, skill_icon_lefttop.x + EQUIPMENT_ICON_SIZE.x - 15, skill_icon_lefttop.y,
				usable_number_string.c_str(), usable_number_string.length());
		}

		if (equipment->Get_Type() == ET_WEAPON && ((Weapon*)equipment)->Get_Cooltime_State()) {
			cooltime_y_jump = EQUIPMENT_ICON_SIZE.y * (((Weapon*)equipment)->Get_Cooltime_Timer() / ((Weapon*)equipment)->Get_Cooltime());
			GdiAlphaBlend(_hDC, skill_icon_lefttop.x, skill_icon_lefttop.y + cooltime_y_jump,
				EQUIPMENT_ICON_SIZE.x, EQUIPMENT_ICON_SIZE.y - cooltime_y_jump,
				skill_icon_texture->Get_MemDC(), PLAYER_COOLTIME_ICON_LEFTTOP.x, PLAYER_COOLTIME_ICON_LEFTTOP.y,
				EQUIPMENT_ICON_SIZE.x, EQUIPMENT_ICON_SIZE.y - cooltime_y_jump, m_player->Get_Cooltime_Blendfunc());
		}

		for (size_t i = 0; i < 4; i++) {
			if (!icon_rendercheck_array[i]) {
				skill_icon_lefttop = POSITION(PLAYER_BAG1_ICON_LEFTTOP.x + i * PLAYER_ICON_X_JUMP, PLAYER_BAG1_ICON_LEFTTOP.y);
				TransparentBlt(_hDC, skill_icon_lefttop.x, skill_icon_lefttop.y,
					EQUIPMENT_ICON_SIZE.x, EQUIPMENT_ICON_SIZE.y, UI_texture->Get_MemDC(),
					UI_ITEMBUTTON_IMAGE_LEFTTOP.x, UI_ITEMBUTTON_IMAGE_LEFTTOP.y, UI_ITEMBUTTON_SIZE.x, UI_ITEMBUTTON_SIZE.y,
					UI_texture->Get_Colorkey());
			}
		}
	}

	SAFE_RELEASE(UI_texture);
}