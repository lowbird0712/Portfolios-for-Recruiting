#pragma once

#include "../../Game.h"

// °ñ·½
#define GOLEM_KEY								"Golem_Key"
#define GOLEM_PORTRAIT_IMAGE_LEFTTOP			POSITION(0, 934)
#define GOLEM_INIT_HP							200
#define GOLEM_PHASE2_START_HP					100
#define GOLEM_SPRITE_KEY						"Golem_Sprite_Key"
#define GOLEM_SPRITE_FILENAME					TEXT("Monster_Golem_Sprite.bmp")
#define GOLEM_IDLE_CLIP_KEY						"Golem_Idle_Clip_Key"
#define GOLEM_DIE_CLIP_KEY						"Golem_Die_Clip_Key"
#define GOLEM_SMASH_EARLY_CLIP_KEY				"Golem_Smash_Early_Clip_Key"
#define GOLEM_SMASH_LATER_CLIP_KEY				"Golem_Smash_Later_Clip_Key"
#define GOLEM_SMASH_DAMAGE						10
#define GOLEM_SMASH_DELAY						1.f
#define GOLEM_HAMMER_EARLY_CLIP_KEY				"Golem_Hammer_Early_Clip_Key"
#define GOLEM_HAMMER_LATER_CLIP_KEY				"Golem_Hammer_Later_Clip_Key"
#define GOLEM_HAMMER_DAMAGE						20
#define GOLEM_HAMMER_DELAY						1.f
#define GOLEM_IMPACT_EARLY_CLIP_KEY				"Golem_Impact_Early_Clip_Key"
#define GOLEM_IMPACT_LATER_CLIP_KEY				"Golem_Impact_Later_Clip_Key"
#define GOLEM_IMPACT_DAMAGE						30
#define GOLEM_IMPACT_DELAY						1.f
#define GOLEM_FAINTCOMBO_EARLY_CLIP_KEY			"Golem_Faintcombo_Early_Clip_Key"
#define GOLEM_FAINTCOMBO1_DAMAGE				10
#define GOLEM_FAINTCOMBO_MIDDLE1_CLIP_KEY		"Golem_Faintcombo_Middle1_Clip_Key"
#define GOLEM_FAINTCOMBO2_DAMAGE				20
#define GOLEM_FAINTCOMBO_MIDDLE2_CLIP_KEY		"Golem_Faintcombo_Middle2_Clip_Key"
#define GOLEM_FAINTCOMBO3_DAMAGE				30
#define GOLEM_FAINTCOMBO_LATER_CLIP_KEY			"Golem_Faintcombo_Later_Clip_Key"
#define GOLEM_FAINTCOMBO_DELAY					2.f
#define GOLEM_FAKE_DIE_IDLE_CLIP_KEY			"Golem_Fake_Die_Idle_Clip_Key"
#define GOLEM_BEFORE_RASER_TIME					5.f
#define GOLEM_RASER_EARLY_CLIP_KEY				"Golem_Raser_Early_Clip_Key"
#define GOLEM_RASER_LATER_CLIP_KEY				"Golem_Raser_Later_Clip_Key"
#define GOLEM_RASER_DAMAGE						50
#define GOLEM_RASER_X_SHIFT						-400
#define GOLEM_RASER_Y_SHIFT						300

class Monster;

enum SKILL_TYPE {
	ST_NORMAL,
	ST_PHASE2
};

struct MONSTER_SKILL {
	SKILL_TYPE	type;
	string		skill_early_clipkey;
	float		delay;
};

struct MONSTER_SKIN {
	vector<MONSTER_SKILL*>	skills;
	string					idle_clipkey;
	string					die_clipkey;
	int						init_HP;
	int						phase2_startHP;

	~MONSTER_SKIN();
};

class Monster_Manager {
	DECLARE_SINGLE(Monster_Manager)
private:
	// º¯¼ö
	Monster*								m_monster = NULL;
	unordered_map<string, MONSTER_SKIN*>	m_skinmap = unordered_map<string, MONSTER_SKIN*>(0);

	float									m_timer = 0.f;			// »ç¿ë Àå¼Ò : °ñ·½
	bool									m_condition = FALSE;	// »ç¿ë Àå¼Ò : °ñ·½
public:
	// ÇÔ¼ö
	bool									Add_Skin(const string & _skinkey, const string & _idle_clipkey, const string & _die_clipkey,
												const int _init_HP, const int _phase2_startHP);
	bool									Add_Skill(const string & _skinkey, const SKILL_TYPE _type,
												const string & _early_clipkey, const float _delay);
	void									Finish_Skill();
	void									Die();

	// °ñ·½ ½ºÅ³
	void									Golem_Smash();
	void									Golem_Hammer();
	void									Golem_Impact();
	void									Golem_Faintcombo1();
	void									Golem_Faintcombo2();
	void									Golem_Faintcombo3();
	void									Golem_Fake_Die();
	void									Golem_Raser();

	unordered_map<string, MONSTER_SKIN*>	Get_Skinmap() const	{ return m_skinmap; }

	bool									Init(Monster * const _monster);
	void									Update(const float _deltatime);
};

