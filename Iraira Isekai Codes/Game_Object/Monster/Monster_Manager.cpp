#pragma once

#include "Monster_Manager.h"
#include "../Game_Scene/Gamenovel_Ingame_Scene/Battle_Scene.h"
#include "../../Game_Singleton/Effect_Manager.h"
#include "Monster.h"
#include "../Player/Player.h"

DEFINITION_SINGLE(Monster_Manager);

MONSTER_SKIN::~MONSTER_SKIN() {
	SAFE_VECLIST_DELETE(skills);
}

Monster_Manager::Monster_Manager() {}	
Monster_Manager::~Monster_Manager() {
	SAFE_MAP_DELETE(m_skinmap);
}

bool Monster_Manager::Add_Skin(const string & _skinkey, const string & _idle_clipkey, const string & _die_clipkey,
	const int _init_HP, const int _phase2_startHP) {

	if (!m_monster)
		return FALSE;

	Animation * const animation = m_monster->Get_Animation();
	if (!animation)
		return FALSE;

	const ANIMATIONCLIP * clip = animation->Find_Clip(_idle_clipkey);
	if (!clip)
		return FALSE;

	clip = animation->Find_Clip(_die_clipkey);
	if (!clip)
		return FALSE;

	MONSTER_SKIN * skin = new MONSTER_SKIN;
	skin->idle_clipkey = _idle_clipkey;
	skin->die_clipkey = _die_clipkey;
	skin->init_HP = _init_HP;
	skin->phase2_startHP = _phase2_startHP;
	skin->skills = vector<MONSTER_SKILL*>(0);

	m_skinmap.insert(make_pair(_skinkey, skin));

	return TRUE;
}

bool Monster_Manager::Add_Skill(const string & _skinkey, const SKILL_TYPE _type,
	const string & _early_clipkey, const float _delay) {

	unordered_map<string, MONSTER_SKIN*>::iterator it = m_skinmap.find(_skinkey);
	if (it == m_skinmap.end())
		return FALSE;
	MONSTER_SKIN * skin = it->second;

	if (!m_monster)
		return FALSE;
	
	Animation * animation = m_monster->Get_Animation();
	if (!animation)
		return FALSE;

	if (!animation->Find_Clip(_early_clipkey))
		return FALSE;

	MONSTER_SKILL * skill = new MONSTER_SKILL;
	skill->type = _type;
	skill->skill_early_clipkey = _early_clipkey;
	skill->delay = _delay;

	skin->skills.push_back(skill);

	return TRUE;
}

void Monster_Manager::Finish_Skill() {
	m_monster->Set_Skilldelay(TRUE);

	const string idle_clipkey = m_skinmap.find(m_monster->Get_Skinkey())->second->idle_clipkey;
	m_monster->Get_Animation()->Set_Nowclip(idle_clipkey);
}

void Monster_Manager::Die() {
	// 현재 재생중인 애니메이션은 죽는 애니메이션이다.
	// 그 애니메이션의 마지막 프레임으로 고정시킨다.
	ANIMATIONCLIP * const nowclip = m_monster->Get_Animation()->Get_Nowclip();
	const POSITION & last_frame = POSITION(nowclip->lastframe_x, nowclip->lastframe_y);

	m_monster->Set_Freeze(TRUE);
	m_monster->Get_Animation()->Stop_To(last_frame);

	Object * const target = m_monster->Get_Target();
	if (target->Get_Strtag() == BATTLE_PLAYER_CLONE_KEY) {
		if (((Player*)target)->Get_HP() <= 0)
			return;

		target->Set_Visible(TRUE);
		((Player*)target)->Set_Moving(MT_STOPPED);
		((Player*)target)->Set_Movable(FALSE);
		target->Get_Animation()->Set_Nowclip(PLAYER_WIN_CLIP_KEY);
	}
}

void Monster_Manager::Golem_Smash() {
	m_monster->Attack(GOLEM_SMASH_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_SMASH_LATER_CLIP_KEY);
}

void Monster_Manager::Golem_Hammer() {
	m_monster->Attack(GOLEM_HAMMER_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_HAMMER_LATER_CLIP_KEY);
}

void Monster_Manager::Golem_Impact() {
	m_monster->Attack(GOLEM_IMPACT_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_IMPACT_LATER_CLIP_KEY);
}

void Monster_Manager::Golem_Faintcombo1() {
	m_monster->Attack(GOLEM_FAINTCOMBO1_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_FAINTCOMBO_MIDDLE1_CLIP_KEY);
}

void Monster_Manager::Golem_Faintcombo2() {
	m_monster->Attack(GOLEM_FAINTCOMBO2_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_FAINTCOMBO_MIDDLE2_CLIP_KEY);
}

void Monster_Manager::Golem_Faintcombo3() {
	m_monster->Attack(GOLEM_FAINTCOMBO3_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_FAINTCOMBO_LATER_CLIP_KEY);
}

void Monster_Manager::Golem_Fake_Die() {
	m_condition = TRUE;

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_FAKE_DIE_IDLE_CLIP_KEY);
}

void Monster_Manager::Golem_Raser() {
	m_monster->Attack(GOLEM_RASER_DAMAGE);

	m_monster->Get_Animation()->Set_Nowclip(GOLEM_RASER_LATER_CLIP_KEY);

	const POSITION effect_pos = POSITION(m_monster->Get_Lefttop_Pos().x + GOLEM_RASER_X_SHIFT,
		m_monster->Get_Lefttop_Pos().y + GOLEM_RASER_Y_SHIFT);
	GET_SINGLE(Effect_Manager)->Set_Position(effect_pos);
	GET_SINGLE(Effect_Manager)->Get_Animation()->Set_Nowclip(EFFECT_RASER_CLIP_KEY);
	GET_SINGLE(Effect_Manager)->Set_Visible(TRUE);
}

bool Monster_Manager::Init(Monster * const _monster) {
	srand(time(NULL));

	m_monster = _monster;
	Animation * animation = m_monster->Get_Animation();

	// 골렘
	CLIP_INPUT golem_idle_clip;
	golem_idle_clip.option = AO_LOOP;
	golem_idle_clip.maxframe_x = 15;
	golem_idle_clip.maxframe_y = 13;
	golem_idle_clip.startframe_x = 1;
	golem_idle_clip.startframe_y = 0;
	golem_idle_clip.lastframe_x = 6;
	golem_idle_clip.lastframe_y = 0;
	if (!animation->Add_Atlas_Clip(GOLEM_IDLE_CLIP_KEY, golem_idle_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;

	CLIP_INPUT golem_die_clip;
	golem_die_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_die_clip.maxframe_x = 15;
	golem_die_clip.maxframe_y = 13;
	golem_die_clip.startframe_x = 1;
	golem_die_clip.startframe_y = 2;
	golem_die_clip.lastframe_x = 13;
	golem_die_clip.lastframe_y = 3;
	if (!animation->Add_Atlas_Clip(GOLEM_DIE_CLIP_KEY, golem_die_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_DIE_CLIP_KEY, this, &Monster_Manager::Golem_Fake_Die);

	CLIP_INPUT golem_smash_early_clip;
	golem_smash_early_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_smash_early_clip.maxframe_x = 15;
	golem_smash_early_clip.maxframe_y = 13;
	golem_smash_early_clip.startframe_x = 1;
	golem_smash_early_clip.startframe_y = 4;
	golem_smash_early_clip.lastframe_x = 9;
	golem_smash_early_clip.lastframe_y = 4;
	if (!animation->Add_Atlas_Clip(GOLEM_SMASH_EARLY_CLIP_KEY, golem_smash_early_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_SMASH_EARLY_CLIP_KEY, this, &Monster_Manager::Golem_Smash);

	CLIP_INPUT golem_smash_later_clip;
	golem_smash_later_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_smash_later_clip.maxframe_x = 15;
	golem_smash_later_clip.maxframe_y = 13;
	golem_smash_later_clip.startframe_x = 10;
	golem_smash_later_clip.startframe_y = 4;
	golem_smash_later_clip.lastframe_x = 12;
	golem_smash_later_clip.lastframe_y = 4;
	if (!animation->Add_Atlas_Clip(GOLEM_SMASH_LATER_CLIP_KEY, golem_smash_later_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_SMASH_LATER_CLIP_KEY, this, &Monster_Manager::Finish_Skill);

	CLIP_INPUT golem_hammer_early_clip;
	golem_hammer_early_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_hammer_early_clip.maxframe_x = 15;
	golem_hammer_early_clip.maxframe_y = 13;
	golem_hammer_early_clip.startframe_x = 1;
	golem_hammer_early_clip.startframe_y = 5;
	golem_hammer_early_clip.lastframe_x = 7;
	golem_hammer_early_clip.lastframe_y = 5;
	if (!animation->Add_Atlas_Clip(GOLEM_HAMMER_EARLY_CLIP_KEY, golem_hammer_early_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_HAMMER_EARLY_CLIP_KEY, this, &Monster_Manager::Golem_Hammer);

	CLIP_INPUT golem_hammer_later_clip;
	golem_hammer_later_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_hammer_later_clip.maxframe_x = 15;
	golem_hammer_later_clip.maxframe_y = 13;
	golem_hammer_later_clip.startframe_x = 8;
	golem_hammer_later_clip.startframe_y = 5;
	golem_hammer_later_clip.lastframe_x = 9;
	golem_hammer_later_clip.lastframe_y = 5;
	if (!animation->Add_Atlas_Clip(GOLEM_HAMMER_LATER_CLIP_KEY, golem_hammer_later_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_HAMMER_LATER_CLIP_KEY, this, &Monster_Manager::Finish_Skill);

	CLIP_INPUT golem_impact_early_clip;
	golem_impact_early_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_impact_early_clip.maxframe_x = 15;
	golem_impact_early_clip.maxframe_y = 13;
	golem_impact_early_clip.startframe_x = 1;
	golem_impact_early_clip.startframe_y = 6;
	golem_impact_early_clip.lastframe_x = 9;
	golem_impact_early_clip.lastframe_y = 6;
	if (!animation->Add_Atlas_Clip(GOLEM_IMPACT_EARLY_CLIP_KEY, golem_impact_early_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_IMPACT_EARLY_CLIP_KEY, this, &Monster_Manager::Golem_Impact);

	CLIP_INPUT golem_impact_later_clip;
	golem_impact_later_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_impact_later_clip.maxframe_x = 15;
	golem_impact_later_clip.maxframe_y = 13;
	golem_impact_later_clip.startframe_x = 10;
	golem_impact_later_clip.startframe_y = 6;
	golem_impact_later_clip.lastframe_x = 13;
	golem_impact_later_clip.lastframe_y = 6;
	if (!animation->Add_Atlas_Clip(GOLEM_IMPACT_LATER_CLIP_KEY, golem_impact_later_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_IMPACT_LATER_CLIP_KEY, this, &Monster_Manager::Finish_Skill);

	CLIP_INPUT golem_faintcombo_early_clip;
	golem_faintcombo_early_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_faintcombo_early_clip.maxframe_x = 15;
	golem_faintcombo_early_clip.maxframe_y = 13;
	golem_faintcombo_early_clip.startframe_x = 1;
	golem_faintcombo_early_clip.startframe_y = 7;
	golem_faintcombo_early_clip.lastframe_x = 13;
	golem_faintcombo_early_clip.lastframe_y = 7;
	if (!animation->Add_Atlas_Clip(GOLEM_FAINTCOMBO_EARLY_CLIP_KEY, golem_faintcombo_early_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_FAINTCOMBO_EARLY_CLIP_KEY, this, &Monster_Manager::Golem_Faintcombo1);

	CLIP_INPUT golem_faintcombo_middle1_clip;
	golem_faintcombo_middle1_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_faintcombo_middle1_clip.maxframe_x = 15;
	golem_faintcombo_middle1_clip.maxframe_y = 13;
	golem_faintcombo_middle1_clip.startframe_x = 14;
	golem_faintcombo_middle1_clip.startframe_y = 7;
	golem_faintcombo_middle1_clip.lastframe_x = 3;
	golem_faintcombo_middle1_clip.lastframe_y = 8;
	if (!animation->Add_Atlas_Clip(GOLEM_FAINTCOMBO_MIDDLE1_CLIP_KEY, golem_faintcombo_middle1_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_FAINTCOMBO_MIDDLE1_CLIP_KEY, this, &Monster_Manager::Golem_Faintcombo2);

	CLIP_INPUT golem_faintcombo_middle2_clip;
	golem_faintcombo_middle2_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_faintcombo_middle2_clip.maxframe_x = 15;
	golem_faintcombo_middle2_clip.maxframe_y = 13;
	golem_faintcombo_middle2_clip.startframe_x = 4;
	golem_faintcombo_middle2_clip.startframe_y = 8;
	golem_faintcombo_middle2_clip.lastframe_x = 9;
	golem_faintcombo_middle2_clip.lastframe_y = 8;
	if (!animation->Add_Atlas_Clip(GOLEM_FAINTCOMBO_MIDDLE2_CLIP_KEY, golem_faintcombo_middle2_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_FAINTCOMBO_MIDDLE2_CLIP_KEY, this, &Monster_Manager::Golem_Faintcombo3);

	CLIP_INPUT golem_faintcombo_later_clip;
	golem_faintcombo_later_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_faintcombo_later_clip.maxframe_x = 15;
	golem_faintcombo_later_clip.maxframe_y = 13;
	golem_faintcombo_later_clip.startframe_x = 10;
	golem_faintcombo_later_clip.startframe_y = 8;
	golem_faintcombo_later_clip.lastframe_x = 14;
	golem_faintcombo_later_clip.lastframe_y = 8;
	if (!animation->Add_Atlas_Clip(GOLEM_FAINTCOMBO_LATER_CLIP_KEY, golem_faintcombo_later_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_FAINTCOMBO_LATER_CLIP_KEY, this, &Monster_Manager::Finish_Skill);

	CLIP_INPUT golem_fake_die_idle_clip;
	golem_fake_die_idle_clip.option = AO_LOOP;
	golem_fake_die_idle_clip.maxframe_x = 15;
	golem_fake_die_idle_clip.maxframe_y = 13;
	golem_fake_die_idle_clip.startframe_x = 1;
	golem_fake_die_idle_clip.startframe_y = 9;
	golem_fake_die_idle_clip.lastframe_x = 8;
	golem_fake_die_idle_clip.lastframe_y = 9;
	if (!animation->Add_Atlas_Clip(GOLEM_FAKE_DIE_IDLE_CLIP_KEY, golem_fake_die_idle_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;

	CLIP_INPUT golem_raser_early_clip;
	golem_raser_early_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_raser_early_clip.maxframe_x = 15;
	golem_raser_early_clip.maxframe_y = 13;
	golem_raser_early_clip.startframe_x = 1;
	golem_raser_early_clip.startframe_y = 12;
	golem_raser_early_clip.lastframe_x = 14;
	golem_raser_early_clip.lastframe_y = 12;
	if (!animation->Add_Atlas_Clip(GOLEM_RASER_EARLY_CLIP_KEY, golem_raser_early_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_RASER_EARLY_CLIP_KEY, this, &Monster_Manager::Golem_Raser);

	CLIP_INPUT golem_raser_later_clip;
	golem_raser_later_clip.option = AO_ONCE_CALLBACK_ONLY;
	golem_raser_later_clip.maxframe_x = 15;
	golem_raser_later_clip.maxframe_y = 13;
	golem_raser_later_clip.startframe_x = 1;
	golem_raser_later_clip.startframe_y = 10;
	golem_raser_later_clip.lastframe_x = 3;
	golem_raser_later_clip.lastframe_y = 11;
	if (!animation->Add_Atlas_Clip(GOLEM_RASER_LATER_CLIP_KEY, golem_raser_later_clip,
		GOLEM_SPRITE_KEY, GOLEM_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;
	animation->Set_Callback<Monster_Manager>(GOLEM_RASER_LATER_CLIP_KEY, this, &Monster_Manager::Die);

	if (!Add_Skin(GOLEM_KEY, GOLEM_IDLE_CLIP_KEY, GOLEM_DIE_CLIP_KEY, GOLEM_INIT_HP, GOLEM_PHASE2_START_HP))
		return FALSE;
	if (!Add_Skill(GOLEM_KEY, ST_NORMAL, GOLEM_SMASH_EARLY_CLIP_KEY, GOLEM_SMASH_DELAY))
		return FALSE;
	if (!Add_Skill(GOLEM_KEY, ST_NORMAL, GOLEM_HAMMER_EARLY_CLIP_KEY, GOLEM_HAMMER_DELAY))
		return FALSE;
	if (!Add_Skill(GOLEM_KEY, ST_NORMAL, GOLEM_IMPACT_EARLY_CLIP_KEY, GOLEM_IMPACT_DELAY))
		return FALSE;
	if (!Add_Skill(GOLEM_KEY, ST_PHASE2, GOLEM_FAINTCOMBO_EARLY_CLIP_KEY, GOLEM_FAINTCOMBO_DELAY))
		return FALSE;

	return TRUE;
}

void Monster_Manager::Update(const float _deltatime) {
	if (m_condition) {
		if (m_monster->Get_Skinkey() == GOLEM_KEY) {
			m_timer += _deltatime;
			if (m_timer >= GOLEM_BEFORE_RASER_TIME) {
				m_timer = 0.f;
				m_condition = FALSE;
				m_monster->Get_Animation()->Set_Nowclip(GOLEM_RASER_EARLY_CLIP_KEY);
			}
		}
	}
}
