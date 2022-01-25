#pragma once

#include "Equipment.h"

class Finite : public Equipment {
protected:
	// ����
	int	m_usable_number = 0;
public:
	// �Լ�
	Finite(const string & _name, const string & _early_clip_key, const EQUIPMENT_TYPE _type,
		const int _usable_number, const POSITION & _icon_lefttop);
	virtual ~Finite();

	int						Get_Usable_Number()						{ return m_usable_number; }

	// �ĵ����� ����
	virtual void			Use();
};

