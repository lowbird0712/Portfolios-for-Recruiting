#pragma once

#include "Scene_Manager.h"
//#include "../../Game_Object/Game_Scene/Gamenovel_Start_Scene.h" ///////////////////
#include "../../Game_Object/Game_Scene/Gamenovel_Ingame_Scene/Before_Battle_Scene.h"
#include "../../Game_Object/Game_Scene/Gamenovel_Ingame_Scene/Battle_Scene.h"
#include "../../Pieces/Bases/Object.h"

DEFINITION_SINGLE(Scene_Manager);

Scene_Manager::Scene_Manager() : m_scene(NULL), m_next_scene(NULL) { }
Scene_Manager::~Scene_Manager() { SAFE_DELETE(m_scene); SAFE_DELETE(m_next_scene); }

SCENE_CHANGING Scene_Manager::Change_Scene() {
	if (m_next_scene) {
		SAFE_DELETE(m_scene);
		m_scene = m_next_scene;
		m_next_scene = NULL;

		return SC_CHANGING;
	}
	else
		return SC_STABLE;
}

bool Scene_Manager::Send_Prototype(const string & _proto_key) {
	unordered_map<string, Object*> protomap = m_scene->Get_Protomap();

	unordered_map<string, Object*>::iterator it = protomap.find(_proto_key);
	if (it == protomap.end())
		return FALSE;

	m_next_scene->Get_Protomap().insert(make_pair(it->first, it->second));
	protomap.erase(it->first);

	return TRUE;
}

bool Scene_Manager::Send_Clone(const string & _clone_key, const string & _layer_key) {
	string layer_key;
	Object *object;

	if (_layer_key != "") {
		if (!(object = Find_Object(_clone_key, _layer_key)))
			return FALSE;

		layer_key = _layer_key;
	}
	else {
		if (!(object = Find_Object(_clone_key)))
			return FALSE;

		layer_key = object->Get_Layer()->Get_Strtag();
	}

	Layer * const layer = m_next_scene->Find_Layer(layer_key);
	if (!layer)
		return FALSE;

	layer->Add_Object(object);
	SAFE_RELEASE(object);
	m_scene->Find_Layer(layer_key)->Get_Objlist().remove(object);

	return TRUE;
}

Object * Scene_Manager::Find_Object(const string & _object_key, const string & _layer_key) {
	Scene *scenes[2] = { m_scene, m_next_scene };

	for (size_t i = 0; i < 2; i++) {
		if (_layer_key != "") {
			Layer* layer = scenes[i]->Find_Layer(_layer_key);
			if (!layer)
				continue;

			list<Object*>::const_iterator it;
			list<Object*>::const_iterator it_end = layer->Get_Objlist().end();

			for (it = layer->Get_Objlist().begin(); it != it_end; it++) {
				if (_object_key == (*it)->Get_Strtag())
					return *it;
			}
		}
		else {
			list<Layer*>::iterator layer_it;
			list<Layer*>::iterator layer_it_end = scenes[i]->Get_Layerlist().end();

			for (layer_it = scenes[i]->Get_Layerlist().begin(); layer_it != layer_it_end; layer_it++) {
				list<Object*>::iterator object_it;
				list<Object*>::iterator object_it_end = (*layer_it)->Get_Objlist().end();

				for (object_it = (*layer_it)->Get_Objlist().begin(); object_it != object_it_end; object_it++) {
					if (_object_key == (*object_it)->Get_Strtag())
						return *object_it;
				}
			}
		}
	}

	return NULL;
}

bool Scene_Manager::Init() {
	if (!Create_Scene<Before_Battle_Scene>(ST_CURRENT)) ///// 테스트용
		return FALSE;

	return TRUE;
}

SCENE_CHANGING Scene_Manager::Input(const float _deltatime) {
	m_scene->Input(_deltatime);

	return Change_Scene();
}

SCENE_CHANGING Scene_Manager::Update(const float _deltatime) {
	m_scene->Update(_deltatime);

	return Change_Scene();
}

void Scene_Manager::Add() {
	m_scene->Add();
}

SCENE_CHANGING Scene_Manager::Late_Update() {
	m_scene->Late_Update();

	return Change_Scene();
}

void Scene_Manager::Render(const HDC & _hDC) {
	m_scene->Render(_hDC);
}