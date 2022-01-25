#pragma once

#include "Scene.h"
#include "../Core.h"
#include "../../Pieces/Bases/Object.h"

Scene::Scene() { }
Scene::~Scene() {
	Erase_Prototype();
	SAFE_VECLIST_DELETE(m_layerlist);
}

Layer * Scene::Create_Layer(const string & _layer_key, const int _zorder) {
	Layer *layer = new Layer;
	layer->Set_Scene(this);
	layer->Set_Strtag(_layer_key);
	layer->Set_Zorder(_zorder);

	m_layerlist.push_back(layer);

	if (m_layerlist.size() >= 2)
		m_layerlist.sort(Scene::Compare_Layer);

	return layer;
}

void Scene::Erase_layer(const string & _layer_key) {
	Layer * const layer = Find_Layer(_layer_key);
	if (!layer)
		DestroyWindow(GET_HWND());

	layer->Die();
}

void Scene::Erase_Prototype(const string & _proto_key) {
	unordered_map<string, Object*>::iterator it = m_protomap.find(_proto_key);
	if (!it->second)
		return;

	SAFE_RELEASE(it->second);
	m_protomap.erase(it);
}

void Scene::Erase_Prototype() {
	SAFE_MAP_RELEASE(m_protomap);
}

Object * Scene::Create_Clone(const string & _proto_key, const string & _clone_key, Layer * const _layer) {
	Object *prototype = _layer->Get_Scene()->Find_Prototype(_proto_key);
	if (!prototype)
		return NULL;

	Object* clone = prototype->Clone();
	clone->Set_Strtag(_clone_key);
	_layer->Add_Object(clone);

	SAFE_RELEASE(prototype);

	return clone;
}

Layer * Scene::Find_Layer(const string & _layer_key) const {
	list<Layer*>::const_iterator it;
	list<Layer*>::const_iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end; it++) {
		if ((*it)->Get_Strtag() == _layer_key)
			return *it;
	}

	return NULL;
}

Object * Scene::Find_Prototype(const string & _proto_key) const {
	unordered_map<string, Object*>::const_iterator it = m_protomap.find(_proto_key);
	if (it == m_protomap.end())
		return NULL;

	it->second->Add_Ref();
	return it->second;
}

Object * Scene::Find_Object(const string & _object_key, const string & _layer_key) const {
	const Layer *layer;

	if (_layer_key != "") {
		if (!(layer = Find_Layer(_layer_key)))
			return NULL;

		return layer->Find_Object(_object_key);
	}
	else {
		list<Layer*>::const_iterator it;
		list<Layer*>::const_iterator it_end = m_layerlist.end();

		for (it = m_layerlist.begin(); it != it_end; it++) {
			layer = *it;

			if (layer->Get_Strtag() == _layer_key)
				return layer->Find_Object(_object_key);
		}

		return NULL;
	}
}

void Scene::Erase_Object(const string & _object_key, const string & _layer_key) {
	Layer *layer;

	if (_layer_key != "") {
		if (!(layer = Find_Layer(_layer_key)))
			DestroyWindow(GET_HWND());

		layer->Erase_Object(_object_key);
	}
	else {
		list<Layer*>::const_iterator it;
		list<Layer*>::const_iterator it_end = m_layerlist.end();

		for (it = m_layerlist.begin(); it != it_end; it++) {
			layer = *it;

			if (layer->Get_Strtag() == _layer_key)
				layer->Erase_Object(_object_key);
		}

		DestroyWindow(GET_HWND());
	}
}

bool Scene::Init() {
	if (!Create_Layer(BACKGROUND_LAYER_KEY))
		return FALSE;
	if (!Create_Layer(DEFAULT_LAYER_KEY, 1))
		return FALSE;
	if (!Create_Layer(UI_LAYER_KEY, INT_MAX))
		return FALSE;

	return TRUE;
}

void Scene::Input(const float _deltatime) {
	list<Layer*>::iterator it;
	list<Layer*>::iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Input(_deltatime);

		if (!(*it)->Get_Live()) {
			SAFE_DELETE((*it));
			it = m_layerlist.erase(it);
			it_end = m_layerlist.end();
		}
		else
			it++;
	}
}

void Scene::Update(const float _deltatime) {
	list<Layer*>::iterator it;
	list<Layer*>::iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable()) {
			it++;
			continue;
		}

		(*it)->Update(_deltatime);

		if (!(*it)->Get_Live()) {
			SAFE_DELETE((*it));
			it = m_layerlist.erase(it);
			it_end = m_layerlist.end();
		}
		else
			it++;
	}
}

void Scene::Add() {
	list<Layer*>::const_iterator it;
	list<Layer*>::const_iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end; it++) {
		if ((*it)->Get_Strtag() != DEFAULT_LAYER_KEY)
			continue;

		if (!(*it)->Get_Enable())
			continue;
		(*it)->Add();
	}
}

void Scene::Late_Update() {
	list<Layer*>::iterator it;
	list<Layer*>::iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end;) {
		if (!(*it)->Get_Enable())
			it++;
		else if (!(*it)->Get_Live()) {
			SAFE_DELETE((*it));
			it = m_layerlist.erase(it);
			it_end = m_layerlist.end();
		}
		else {
			(*it)->Late_Update();
			it++;
		}
	}
}

void Scene::Render(const HDC & hDC) {
	list<Layer*>::const_iterator it;
	list<Layer*>::const_iterator it_end = m_layerlist.end();

	for (it = m_layerlist.begin(); it != it_end; it++) {
		if (!(*it)->Get_Enable())
			continue;
		(*it)->Render(hDC);
	}
}