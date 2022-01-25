#pragma once

#include "Equipment.h"

class Weapon : public Equipment {
protected:
	// 변수
	const float		m_cooltime;
	const int		m_minatk;
	const int		m_maxatk;

	bool			m_cooltime_state = FALSE;
	float			m_cooltime_timer = 0.f;
public:
	// 함수
	Weapon(const string & _name, const string & _early_clip_key, const string & _later_clip_key, const float _cooltime,
		const int _minatk, const int _maxatk, const POSITION & _icon_lefttop);
	virtual ~Weapon();

	virtual void	Use();

	int				Get_Random_Damage();

	void			Set_Cooltime_State(const int _state);
	float			Get_Cooltime() const					{ return m_cooltime; }
	bool			Get_Cooltime_State() const				{ return m_cooltime_state; }
	float			Get_Cooltime_Timer() const				{ return m_cooltime_timer; }

	void			Update(const float _deltatime);
};

