#pragma once

#include "Layer.h"
#include "../Core.h"
#include "../Collision_Manager/Collision_Manager.h"
#include "../../Pieces/Bases/Object.h"

Layer::Layer() : m_strtag(""), m_scene(NULL), m_zorder(0), m_enable(TRUE), m_live(TRUE) { }
Layer::~Layer() {
	SAFE_VECLIST_RELEASE(m_objlist);
}

void Layer::Add_Object(Object * const _object) {
	_object->Set_Scene(m_scene);
	_object->Set_Layer(this);

	_object->Add_Ref();
	m_objlist.push_back(_object);
}

void Layer::Erase_Object(const string & _object_key) {
	Object *object = Find_Object(_object_key);
	if (!object)
		DestroyWindow(GET_HWND());

	object->Die();
}

Object * Layer::Find_Object(const string & _object_key) const {
	list<Object*>::const_iterator it;
	list<Object*>::const_iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end; it++) {
		if ((*it)->Get_Strtag() == _object_key)
			return *it;
	}

	return NULL;
}

void Layer::Input(const float _deltatime) {
	list<Object*>::iterator it;
	list<Object*>::iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end;) {
		if ((*it)->Get_Freeze()) {
			it++;
			continue;
		}
		else if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Input(_deltatime);

		if (!(*it)->Get_Live()) {
			SAFE_RELEASE((*it));
			it = m_objlist.erase(it);
			it_end = m_objlist.end();
		}
		else
			it++;
	}
}

void Layer::Update(const float _deltatime) {
	list<Object*>::iterator it;
	list<Object*>::iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end;) {
		if ((*it)->Get_Freeze()) {
			it++;
			continue;
		}
		else if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Update(_deltatime);

		if (!(*it)->Get_Live()) {
			SAFE_RELEASE((*it));
			it = m_objlist.erase(it);
			it_end = m_objlist.end();
		}
		else
			it++;
	}
}

void Layer::Add() {
	list<Object*>::iterator it;
	list<Object*>::iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end; it++) {
		if ((*it)->Get_Freeze())
			continue;
		else if (!(*it)->Get_Enable())
			continue;

		GET_SINGLE(Collision_Manager)->Add_Object(*it);
	}
}

void Layer::Late_Update() {
	list<Object*>::iterator it;
	list<Object*>::iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end;) {
		if ((*it)->Get_Freeze())
			it++;
		else if (!(*it)->Get_Enable())
			it++;
		else if (!(*it)->Get_Live()) {
			SAFE_DELETE((*it));
			it = m_objlist.erase(it);
			it_end = m_objlist.end();
		}
		else {
			(*it)->Late_Update();
			it++;
		}
	}
}

void Layer::Render(const HDC & _hDC) {
	list<Object*>::iterator it;
	list<Object*>::iterator it_end = m_objlist.end();

	for (it = m_objlist.begin(); it != it_end; it++) {
		if (!(*it)->Get_Enable())
			continue;
		else if (!(*it)->Get_Visible())
			continue;
		(*it)->Render(_hDC);
	}
}
