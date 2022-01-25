#pragma once

#include "../Game.h"

class Turn_Checker {
	DECLARE_SINGLE(Turn_Checker)
private:
	// Variables
	float	m_turn_timelength;
	float	m_passed_time;
	bool	m_working;
	bool	m_ringing;
public:
	// Functions
	void	Reset();

	void	Set_Working(const bool _working)	{ m_working = _working; }
	float	Get_Turn_Timelength() const			{ return m_turn_timelength; }
	float	Get_Passed_Time() const				{ return m_passed_time; }
	float	Get_Passed_Ratio_Time() const		{ return m_passed_time / m_turn_timelength; }
	bool	Get_Ringing() const					{ return m_ringing; }

	bool	Init(const float _turn_timelength);
	void	Update(const float _deltatime);
};

