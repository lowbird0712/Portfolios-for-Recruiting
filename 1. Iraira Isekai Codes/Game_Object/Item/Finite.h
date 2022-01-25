#pragma once

#include "Equipment.h"

class Finite : public Equipment {
protected:
	// 변수
	int	m_usable_number = 0;
public:
	// 함수
	Finite(const string & _name, const string & _early_clip_key, const EQUIPMENT_TYPE _type,
		const int _usable_number, const POSITION & _icon_lefttop);
	virtual ~Finite();

	int						Get_Usable_Number()						{ return m_usable_number; }

	// 후딜레이 없음
	virtual void			Use();
};

