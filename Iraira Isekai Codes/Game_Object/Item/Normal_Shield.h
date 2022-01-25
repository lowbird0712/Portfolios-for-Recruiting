#pragma once

#include "Shield.h"

class Normal_Shield : public Shield {

public:
	// 함수
	Normal_Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const int _barrier, const POSITION & _icon_lefttop);
	virtual ~Normal_Shield();

	// 클립 추가 시 추가할 함수들이다.
	// 방어하는 모습으로 고정시키고 데미지 반감의 효과를 켠다.
	virtual void	Use()									{ Shield::Use(); }
	virtual void	Unuse()									{ Shield::Unuse(); }
};

