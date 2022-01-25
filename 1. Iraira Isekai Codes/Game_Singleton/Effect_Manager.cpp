#pragma once

#include "Effect_Manager.h"
#include "../Pieces/Pieces/Animation.h"
#include "../Singleton/Resource_Manager/Texture.h"

DEFINITION_SINGLE(Effect_Manager);

Effect_Manager::Effect_Manager() {}
Effect_Manager::~Effect_Manager() {
	SAFE_RELEASE(m_animation);
}

bool Effect_Manager::Init() {
	m_animation = new Animation;
	m_animation->Set_Strtag(EFFECT_MANAGER_ANIMATION_KEY);

	CLIP_INPUT effect_raser_clip;
	effect_raser_clip.option = AO_ONCE;
	effect_raser_clip.maxframe_x = 1;
	effect_raser_clip.maxframe_y = 5;
	effect_raser_clip.startframe_x = 0;
	effect_raser_clip.startframe_y = 0;
	effect_raser_clip.lastframe_x = 0;
	effect_raser_clip.lastframe_y = 4;
	if (!m_animation->Add_Atlas_Clip(EFFECT_RASER_CLIP_KEY, effect_raser_clip,
		EFFECT_RASER_SPRITE_KEY, EFFECT_RASER_SPRITE_FILENAME, RGB_MAGENTA))
		return FALSE;

	return TRUE;
}

void Effect_Manager::Update(const float _deltatime) {
	m_animation->Update(_deltatime);
}

void Effect_Manager::Render(const HDC & _hDC) {
	if (!m_visible)
		return;

	ANIMATIONCLIP * const clip = m_animation->Get_Nowclip();
	if (!clip)
		return;

	if (clip->type == AT_ATLAS) {
		Texture * const texture = clip->texture;
		const FLOATSIZE framesize = m_animation->Get_Frame_Size();
		const POSITION render_lefttop = {
			clip->nowframe_x * framesize.x,
			clip->nowframe_y * framesize.y
		};

		if (texture->Get_Colorkey() != TEXTURE_COLORKEY_UNUSE) {
			TransparentBlt(_hDC, m_pos.x, m_pos.y, framesize.x, framesize.y,
				texture->Get_MemDC(), render_lefttop.x, render_lefttop.y, framesize.x, framesize.y, texture->Get_Colorkey());
		}
		else
			BitBlt(_hDC, m_pos.x, m_pos.y, framesize.x, framesize.y, texture->Get_MemDC(), render_lefttop.x, render_lefttop.y, SRCCOPY);
	}
}
