#pragma once

#include "Shield.h"

class Normal_Shield : public Shield {

public:
	// �Լ�
	Normal_Shield(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const int _barrier, const POSITION & _icon_lefttop);
	virtual ~Normal_Shield();

	// Ŭ�� �߰� �� �߰��� �Լ����̴�.
	// ����ϴ� ������� ������Ű�� ������ �ݰ��� ȿ���� �Ҵ�.
	virtual void	Use()									{ Shield::Use(); }
	virtual void	Unuse()									{ Shield::Unuse(); }
};

