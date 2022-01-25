#pragma once

#include "Object.h"
#include "../../Singleton/Camera.h"
#include "../../Singleton/Resource_Manager/Texture.h"

Object::Object() : m_texture(NULL), m_animation(NULL), m_image_lefttop(0.f, 0.f), m_strtag(""), m_scene(NULL), m_layer(NULL),
m_pos(0.f, 0.f), m_size(0.f, 0.f), m_pivot(0.f, 0.f) { }
Object::Object(const Object & _object)
	: m_texture(NULL), m_animation(NULL), m_image_lefttop(_object.m_image_lefttop), m_strtag(_object.m_strtag), m_scene(NULL), m_layer(NULL),
	m_pos(_object.m_pos), m_size(_object.m_size), m_pivot(_object.m_pivot) {

	list<Collider*>::const_iterator it;
	list<Collider*>::const_iterator it_end = _object.m_collist.end();
	for (it = _object.m_collist.begin(); it != it_end; it++) {
		Collider *collider = (*it)->Clone();
		collider->Set_Object(this);
		m_collist.push_back(collider);
	}

	if (_object.m_texture)
		Set_Texture(_object.m_texture);

	if (_object.m_animation) {
		m_animation = _object.m_animation->Clone();
		m_animation->Set_Object(this);
	}
}
Object::~Object() {
	SAFE_VECLIST_RELEASE(m_collist);
	SAFE_RELEASE(m_animation);
	SAFE_RELEASE(m_texture);
}

bool Object::Set_Texture(Texture *_texture) {
	SAFE_RELEASE(m_texture);

	m_texture = _texture;
	if (!m_texture)
		return FALSE;

	m_texture->Add_Ref();
	return TRUE;
}

bool Object::Set_Texture(const string &_texture_key, const TCHAR * const _filename,
	const COLORREF &_color, const string &_path_key) {

	SAFE_RELEASE(m_texture);

	m_texture = GET_SINGLE(Resource_Manager)->Load_Texture(_texture_key, _filename, _color, _path_key);
	if (!m_texture)
		return FALSE;

	return TRUE;
}

void Object::Release_Texture() {
	SAFE_RELEASE(m_texture);
}

void Object::Create_Animation(const string & _ani_key) {
	SAFE_RELEASE(m_animation);

	m_animation = new Animation;

	m_animation->Set_Strtag(_ani_key);
	m_animation->Set_Object(this);
}

bool Object::Add_Atlas_Clip(const string &_clip_key, const CLIP_INPUT &_clip,
	const string &_texture_key, const TCHAR *_filename, const COLORREF &_color, const string &_path_key) {

	if (!m_animation)
		return FALSE;
	if (!m_animation->Add_Atlas_Clip(_clip_key, _clip, _texture_key, _filename, _color, _path_key))
		return FALSE;

	return TRUE;
}

FLOATRECT Object::Get_Rect() const {
	FLOATRECT rect;
	POSITION lefttop = m_pos - Get_Pivotsize();

	rect.left = lefttop.x;
	rect.top = lefttop.y;
	rect.right = lefttop.x + m_size.x;
	rect.bottom = lefttop.y + m_size.y;

	return rect;
}

POSITION Object::Get_Center() const {
	POSITION center;
	FLOATRECT rect = Get_Rect();
	center.x = (rect.left + rect.right) / 2;
	center.y = (rect.top + rect.bottom) / 2;

	return center;
}

void Object::Input(const float _deltatime) {
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_collist.end();

	for (it = m_collist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Input();

		if (!(*it)->Get_Live()) {
			SAFE_RELEASE((*it));
			it = m_collist.erase(it);
			it_end = m_collist.end();
		}
		else
			it++;
	}
}

void Object::Update(const float _deltatime) {
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_collist.end();

	for (it = m_collist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Update();

		if (!(*it)->Get_Live()) {
			SAFE_RELEASE((*it));
			it = m_collist.erase(it);
			it_end = m_collist.end();
		}
		else
			it++;
	}

	if (m_animation)
		m_animation->Update(_deltatime);
}

void Object::Late_Update() {
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_collist.end();

	for (it = m_collist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable())
			it++;
		else if (!(*it)->Get_Live()) {
			SAFE_RELEASE((*it));
			it = m_collist.erase(it);
			it_end = m_collist.end();
		}
		else {
			(*it)->Late_Update();
			it++;
		}
	}
}

void Object::Render(const HDC & _hDC) {
	POSITION pos = m_pos - Get_Pivotsize();
	RESOLUTION client_resolution = GET_RESOLUTION();
	POSITION camera_pos;

	if (SINGLE_DECLARED(Camera))
		camera_pos = GET_SINGLE(Camera)->Get_Pos();

	RECT rect_tmp;
	RECT render_rect = { pos.x, pos.y, pos.x + m_size.x, pos.y + m_size.y };
	RECT client_rect = { camera_pos.x, camera_pos.y, camera_pos.x + client_resolution.width, camera_pos.y + client_resolution.height };

	if (!IntersectRect(&rect_tmp, &render_rect, &client_rect))
		return;

#ifdef _DEBUG
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_collist.end();

	for (it = m_collist.begin(); it != it_end; it++) {
		if (!(*it)->Get_Enable())
			continue;
		(*it)->Render(_hDC);
	}
#endif //_DEBUG

	if (SINGLE_DECLARED(Camera))
		pos -= GET_SINGLE(Camera)->Get_Pos();

	if (m_animation && m_animation->Get_Nowclip()->type == AT_ATLAS) { // 아틀라스 애니메이션 출력
		ANIMATIONCLIP* clip = m_animation->Get_Nowclip();
		POSITION render_lefttop = {
			clip->nowframe_x * m_size.x,
			clip->nowframe_y * m_size.y
		};

		if (m_texture->Get_Colorkey() != TEXTURE_COLORKEY_UNUSE) {
			TransparentBlt(_hDC, pos.x, pos.y, m_size.x, m_size.y,
				m_texture->Get_MemDC(), render_lefttop.x, render_lefttop.y, m_size.x, m_size.y, m_texture->Get_Colorkey());
		}
		else
			BitBlt(_hDC, pos.x, pos.y, m_size.x, m_size.y, m_texture->Get_MemDC(), render_lefttop.x, render_lefttop.y, SRCCOPY);
	}
	else if (m_texture && m_image_lefttop != TEXTURE_UNUSE_IMAGE_LEFTTOP) { // 일반 이미지 출력
		if (m_texture->Get_Colorkey() != TEXTURE_COLORKEY_UNUSE) {
			TransparentBlt(_hDC, pos.x, pos.y, m_size.x, m_size.y,
				m_texture->Get_MemDC(), m_image_lefttop.x, m_image_lefttop.y, m_size.x, m_size.y, m_texture->Get_Colorkey());
		}
		else
			BitBlt(_hDC, pos.x, pos.y, m_size.x, m_size.y, m_texture->Get_MemDC(), m_image_lefttop.x, m_image_lefttop.y, SRCCOPY);
	}
#ifdef _DEBUG
	else
		Rectangle(_hDC, pos.x, pos.y, pos.x + m_size.x, pos.y + m_size.y);
#endif //_DEBUG
}