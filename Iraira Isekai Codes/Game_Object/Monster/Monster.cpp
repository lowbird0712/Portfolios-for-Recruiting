#pragma once

#include "Monster.h"
#include "../../Singleton/Resource_Manager/Texture.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Battle_Scene.h"
#include "Monster_Manager.h"
#include "../Player/Player.h"

Monster::Monster() {}
Monster::Monster(Monster & _monster) : Moving(_monster) {
	m_skinkey = _monster.m_skinkey;
	m_HP = _monster.m_HP;

	m_HP_bar_alphaDC = _monster.m_HP_bar_alphaDC;
	m_HP_bar_alphabitmap_tmp = _monster.m_HP_bar_alphabitmap_tmp;
	m_HP_bar_texture = _monster.m_HP_bar_texture;
	m_HP_bar_texture->Add_Ref();
	m_HP_bar_blendfunc = _monster.m_HP_bar_blendfunc;
	m_reducing_HP_bar_blendfunc = _monster.m_reducing_HP_bar_blendfunc;
}
Monster::~Monster() {
	DeleteObject(SelectObject(m_HP_bar_alphaDC, m_HP_bar_alphabitmap_tmp));
	DeleteDC(m_HP_bar_alphaDC);

	SAFE_RELEASE(m_HP_bar_texture);
}

bool Monster::Clone_Init(const string & _skinkey, const POSITION & _init_lefttop) {
	if (_skinkey == "")
		return FALSE;

	Set_Skinkey(_skinkey);

	m_size = m_animation->Get_Frame_Size();
	m_pos = _init_lefttop + Get_Pivotsize();

	return TRUE;
}

void Monster::Draw_Skill() {
	unordered_map<string, MONSTER_SKIN*> skinmap = GET_SINGLE(Monster_Manager)->Get_Skinmap();
	unordered_map<string, MONSTER_SKIN*>::iterator it = skinmap.find(m_skinkey);
	if (it == skinmap.end())
		return;

	vector<MONSTER_SKILL*> skills = it->second->skills;

	do {
		m_nowskill = skills[rand() % skills.size()];	// srand(time(NULL))은 Monster_Manager::Init에서 호출된다.
	} while (!m_phase2_on && m_nowskill->type == ST_PHASE2);

	m_animation->Set_Nowclip(m_nowskill->skill_early_clipkey);
}

void Monster::Attack(const int _damage) {
	if (m_target->Get_Strtag() == BATTLE_PLAYER_CLONE_KEY) {
		if (((Player*)m_target)->Get_Dodging())
			return;

		((Player*)m_target)->Damaged(_damage);
	}
}

void Monster::Damaged(int _damage) {
	if (_damage >= m_HP) {
		_damage = m_HP;
		m_delay_timer = -10000.f; // 죽는 애니메이션 실행 중에 Draw_Skill이 호출되지 않도록 하기 위함이다.
		m_animation->Set_Nowclip(GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second->die_clipkey);
	}

	m_HP -= _damage;
	m_reducing_HP += _damage;
}

void Monster::Set_Skinkey(const string _skinkey) {
	m_skinkey = _skinkey;

	MONSTER_SKIN * skin = GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second;

	m_animation->Set_Nowclip(skin->idle_clipkey);
	m_HP = skin->init_HP;
}

void Monster::Set_Target(Object * const _target) {
	m_target = _target;

	if (m_target && m_target->Get_Strtag() == BATTLE_PLAYER_CLONE_KEY) {
		Player * player = (Player*)m_target;
		m_HP_bar_blendfunc.SourceConstantAlpha = player->Get_HP_Bar_SourceConstantAlpha();
		m_reducing_HP_bar_blendfunc.SourceConstantAlpha = player->Get_Reducing_HP_Bar_SourceConstantAlpha();
		m_reducing_HP_bar_onetime = player->Get_Reducing_HP_Bar_Onetime()
			* ((float)PLAYER_INIT_MAXHP / GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second->init_HP);
	}
}

bool Monster::Init() {
	Create_Animation(MONSTER_ANIMATION_KEY);

	// 클립 등록, 사이즈 등은 전부 Monster_Manager::Init과 Monster::Clone_Init에서 이루어진다.
	m_pivot = MONSTER_PIVOT;

	Texture * back_buffer = GET_SINGLE(Resource_Manager)->Get_BackBuffer();
	HDC back_DC = back_buffer->Get_MemDC();

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

	m_reducing_HP_bar_blendfunc.BlendOp = 0;
	m_reducing_HP_bar_blendfunc.BlendFlags = 0;
	m_reducing_HP_bar_blendfunc.AlphaFormat = AC_SRC_OVER;

	SAFE_RELEASE(back_buffer);

	// m_reducing_HP_bar_FPS와 체력 바 투명도의 설정은 Monster::Set_Target을 통해 플레이어와 같은 속도로 감소하도록 설정된다.

	return TRUE;
}

void Monster::Update(const float _deltatime) {
	Moving::Update(_deltatime);

	if (m_skill_active) {
		if (m_start_state) {
			m_delay_timer += _deltatime;

			if (m_delay_timer >= BATTLE_START_DELAY) {
				Draw_Skill();
				m_start_state = FALSE;
				m_delay_timer = 0.f;
			}
		}
		else if (m_skilldelay_on) {
			m_delay_timer += _deltatime;

			if (m_delay_timer >= m_nowskill->delay) {
				Draw_Skill();
				m_skilldelay_on = FALSE;
				m_delay_timer = 0.f;
			}
		}
	}

	if (!m_phase2_on && m_HP <= GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second->phase2_startHP)
		m_phase2_on = TRUE;

	if (m_reducing_HP != 0) {
		m_reducing_HP_bar_timer += _deltatime;

		if (m_reducing_HP_bar_timer >= m_reducing_HP_bar_onetime) {
			m_reducing_HP -= 1;
			m_reducing_HP_bar_timer -= m_reducing_HP_bar_onetime;
		}
	}
}

void Monster::Render(const HDC & _hDC) {
	Moving::Render(_hDC);

	const int init_HP = GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second->init_HP;
	const float HP_rate = (float)m_HP / init_HP;
	const float reducing_HP_rate = (float)m_reducing_HP / init_HP;

	if (m_reducing_HP != 0) {
		GdiAlphaBlend(_hDC, MONSTER_HP_BAR_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate - HP_rate), MONSTER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * reducing_HP_rate, PLAYER_HP_BAR_SIZE.y,
			m_HP_bar_alphaDC, PLAYER_HP_BAR_RED_LEFTTOP.x + 3, PLAYER_HP_BAR_RED_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * reducing_HP_rate, PLAYER_HP_BAR_SIZE.y, m_reducing_HP_bar_blendfunc);
	}

	if (m_HP == GET_SINGLE(Monster_Manager)->Get_Skinmap().find(m_skinkey)->second->init_HP) {
		TransparentBlt(_hDC, MONSTER_HP_BAR_LEFTTOP.x, MONSTER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y,
			m_HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_GREEN_LEFTTOP.x, PLAYER_HP_BAR_GREEN_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y, m_HP_bar_texture->Get_Colorkey());
	}
	else {
		GdiAlphaBlend(_hDC, MONSTER_HP_BAR_LEFTTOP.x, MONSTER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate - HP_rate), PLAYER_HP_BAR_SIZE.y,
			m_HP_bar_alphaDC, PLAYER_HP_BAR_GRAY_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * HP_rate + 3, PLAYER_HP_BAR_GRAY_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * (1 - reducing_HP_rate - HP_rate) - 3, PLAYER_HP_BAR_SIZE.y, m_HP_bar_blendfunc);

		TransparentBlt(_hDC, MONSTER_HP_BAR_LEFTTOP.x + PLAYER_HP_BAR_SIZE.x * (1 - HP_rate) - 3, MONSTER_HP_BAR_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * HP_rate + 3, PLAYER_HP_BAR_SIZE.y,
			m_HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_YELLOW_LEFTTOP.x, PLAYER_HP_BAR_YELLOW_LEFTTOP.y,
			PLAYER_HP_BAR_SIZE.x * HP_rate, PLAYER_HP_BAR_SIZE.y, m_HP_bar_texture->Get_Colorkey());
	}

	TransparentBlt(_hDC, MONSTER_HP_BAR_LEFTTOP.x, MONSTER_HP_BAR_LEFTTOP.y,
		PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y,
		m_HP_bar_texture->Get_MemDC(), PLAYER_HP_BAR_SIDE_LEFTTOP.x, PLAYER_HP_BAR_SIDE_LEFTTOP.y,
		PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_SIZE.y, m_HP_bar_texture->Get_Colorkey());
}
