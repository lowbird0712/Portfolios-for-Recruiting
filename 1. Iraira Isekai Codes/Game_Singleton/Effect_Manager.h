#pragma once

#include "../Game.h"

#define EFFECT_MANAGER_ANIMATION_KEY	"Effect_Manager_Animation_Key"
#define EFFECT_RASER_CLIP_KEY			"Effect_Raser_Clip_Key"
#define EFFECT_RASER_SPRITE_KEY			"Effect_Raser_Sprite_Key"
#define EFFECT_RASER_SPRITE_FILENAME	TEXT("Effect_Raser.bmp")

class Animation;

class Effect_Manager {
	DECLARE_SINGLE(Effect_Manager)
private:
	// ����
	Animation * m_animation = NULL;	// �ִϸ��̼� ��ü�� �ҼӵǾ� �ִ� Object ��ü�� ���ٴ� �Ϳ� ��������.
	POSITION	m_pos = POSITION();
	bool		m_visible = FALSE;
public:
	// �Լ�
	void		Set_Position(const POSITION & _pos)				{ m_pos = _pos; }
	void		Set_Position(const float _x, const float _y)	{ Set_Position(POSITION(_x, _y)); }
	void		Set_Visible(const bool _visible)				{ m_visible = _visible; }
	Animation*	Get_Animation() const							{ return m_animation; }

	bool		Init();
	void		Update(const float _deltatime);
	void		Render(const HDC & _hDC);
};

