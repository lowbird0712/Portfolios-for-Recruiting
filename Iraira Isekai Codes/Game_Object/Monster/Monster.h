#pragma once

#include "../../Pieces/Bases/Moving.h"
#include "../../Singleton/Core.h"
#include "Monster_Manager.h"
#include "../Player/Player.h"

#define MONSTER_PROTOTYPE_KEY			"Monster_Prototype_Key"
#define MONSTER_ANIMATION_KEY			"Monster_Animation_Key"

#define MONSTER_PIVOT					FLOATPOINT(0.5f, 0.5f)
#define BATTLE_START_DELAY				2.f
#define MONSTER_HP_BAR_LEFTTOP			POSITION(WINDOW_WIDTH - PLAYER_HP_BAR_LEFTTOP.x - PLAYER_HP_BAR_SIZE.x, PLAYER_HP_BAR_LEFTTOP.y)

class Player;
class Object;

class Monster : public Moving {
protected:
	// ����
	string							m_skinkey = "";	// ��Ų Ű�� �̸��� ���ҵ� �����Ѵ�.
	MONSTER_SKILL*					m_nowskill = NULL;
	int								m_HP = 0;
	int								m_reducing_HP = 0;

	HDC								m_HP_bar_alphaDC = NULL;
	HBITMAP							m_HP_bar_alphabitmap_tmp = NULL;
	Texture*						m_HP_bar_texture = NULL;
	BLENDFUNCTION					m_HP_bar_blendfunc = BLENDFUNCTION();
	BLENDFUNCTION					m_reducing_HP_bar_blendfunc = BLENDFUNCTION();
	float							m_reducing_HP_bar_onetime;
	float							m_reducing_HP_bar_timer = 0.f;

	Object*							m_target = NULL;
	bool							m_skill_active = TRUE; // TRUE�� ��쿡�� Monster::Update���� Monster::Draw_Skill�� ȣ��ȴ�.
	bool							m_start_state = TRUE;
	bool							m_skilldelay_on = FALSE;
	bool							m_phase2_on = FALSE;

	float							m_delay_timer = 0.f;
public:
	// �Լ�
	Monster();
	Monster(Monster & _monster);
	~Monster();

	Monster*		Clone()													{ return new Monster(*this); }
	bool			Clone_Init(const string & _skinkey, const POSITION & _init_lefttop);

	void			Draw_Skill();
	void			Attack(const int _damage);
	void			Damaged(int _damage);

	void			Set_Skinkey(const string _skinkey);
	void			Set_Target(Object * const _target);
	void			Set_Skill_Active(const bool _active)					{ m_skill_active = _active; }
	void			Set_Skilldelay(const bool _skilldelay_on)				{ m_skilldelay_on = _skilldelay_on; }
	string			Get_Skinkey() const										{ return m_skinkey; }
	Object*			Get_Target() const										{ return m_target; }
	MONSTER_SKILL*	Get_Nowskill() const									{ return m_nowskill; }
	int				Get_HP() const											{ return m_HP; }

	bool			Init();
	void			Update(const float _deltatime);
	void			Render(const HDC & _hDC);
};