#pragma once

#include "Equipment.h"

// 방패

class Shield : public Equipment {
protected:
	// 변수
	const int	m_barrier;
public:
	// 함수
	Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const int _barrier, const POSITION & _icon_lefttop);
	virtual ~Shield();

	// 클립 추가 시 추가할 함수들이다.
	// 방어하는 모습으로 고정시키고 데미지 반감의 효과를 켠다.
	virtual void	Use();
	virtual void	Unuse();

	int				Get_Barrier() const		{ return m_barrier; }
};