#pragma once

#include "Equipment.h"

// ����

class Shield : public Equipment {
protected:
	// ����
	const int	m_barrier;
public:
	// �Լ�
	Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const int _barrier, const POSITION & _icon_lefttop);
	virtual ~Shield();

	// Ŭ�� �߰� �� �߰��� �Լ����̴�.
	// ����ϴ� ������� ������Ű�� ������ �ݰ��� ȿ���� �Ҵ�.
	virtual void	Use();
	virtual void	Unuse();

	int				Get_Barrier() const		{ return m_barrier; }
};