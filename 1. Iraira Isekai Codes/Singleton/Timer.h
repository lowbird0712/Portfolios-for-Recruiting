#pragma once

#include "../Game.h"

class Timer {
	DECLARE_SINGLE(Timer)
private:
	// Variables
	HWND			m_hWnd;

	LARGE_INTEGER	m_frequency;
	LARGE_INTEGER	m_oldtimer;
	float			m_deltatime;
	float			m_timescale;

	float			m_FPS;
	float			m_passed_FPStime;
	int				m_passed_frame;
public:
	// Functions
	float			Set_Timescale(const float _timescale)	{ m_timescale = _timescale; }
	float			Get_Deltatime() const					{ return m_deltatime * m_timescale; }
	float			Get_Timescale() const					{ return m_timescale; }
	float			Get_FPS() const							{ return m_FPS; }

	bool			Init(HWND _hWnd);
	void			Update();
};