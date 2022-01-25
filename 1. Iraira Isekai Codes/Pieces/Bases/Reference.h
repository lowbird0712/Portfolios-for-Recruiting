#pragma once

#include "../../Game.h"

class Reference {
protected:
	// Variables
	int		m_ref;
	bool	m_freeze;	// ���̱⸸ �Ѵ�
	bool	m_visible;	// Input, Update���� �Լ��� �۵��ϰ� �������� �ʴ´�.
	bool	m_enable;	// ���̰� Input, Update���� �Լ� ���� �۵��ȴ�
	bool	m_live;		// FALSE�� ��� �������� �ʰ� Input, Update ���� �Լ� ���� �۵����� �ʴ´�
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