#pragma once

#include "../../Game.h"

class Reference {
protected:
	// Variables
	int		m_ref;
	bool	m_freeze;	// 보이기만 한다
	bool	m_visible;	// Input, Update등의 함수만 작동하고 보이지는 않는다.
	bool	m_enable;	// 보이고 Input, Update등의 함수 또한 작동된다
	bool	m_live;		// FALSE일 경우 보이지도 않고 Input, Update 등의 함수 또한 작동되지 않는다
public:
	// Functions
	Reference();
	virtual ~Reference() = 0;

	void	Add_Ref()							{ m_ref++; }
	int		Release();

	void	Set_Freeze(const bool _freeze)		{ m_freeze = _freeze; }
	void	Set_Visible(const bool _visible)	{ m_visible = _visible; }
	void	Set_Enable(const bool _enable)		{ m_enable = _enable; }
	void	Die()								{ m_live = FALSE; }
	int		Get_Ref() const						{ return m_ref; }
	bool	Get_Freeze() const					{ return m_freeze; }
	bool	Get_Visible() const					{ return m_visible; }
	bool	Get_Enable() const					{ return m_enable; }
	bool	Get_Live() const					{ return m_live; }
};