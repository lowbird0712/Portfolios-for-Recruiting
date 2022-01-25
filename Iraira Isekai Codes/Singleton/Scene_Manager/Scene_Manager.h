#pragma once

#include "../../Game.h"

enum SCENE_TYPE {
	ST_CURRENT,
	ST_NEXT
};

enum SCENE_CHANGING {
	SC_STABLE,
	SC_CHANGING
};

class Scene;
class Object;

class Scene_Manager {
	DECLARE_SINGLE(Scene_Manager)
private:
	// Variables
	Scene*	m_scene = NULL;
	Scene*	m_next_scene = NULL;

	// Functions
	SCENE_CHANGING	Change_Scene();
public:
	// Functions
	template <typename T>
	T*				Create_Scene(const SCENE_TYPE _type);

	bool			Send_Prototype(const string &_proto_key);
	bool			Send_Clone(const string &_clone_key, const string &_layer_key = "");

	Object*			Find_Object(const string &_object_key, const string &_layer_key = "");

	Scene*			Get_Scene() { return m_scene; }

	bool			Init();
	SCENE_CHANGING	Input(const float _deltatime);
	SCENE_CHANGING	Update(const float _deltatime);
	void			Add();
	SCENE_CHANGING	Late_Update();
	void			Render(const HDC &_hDC);
};

template<typename T>
inline T * Scene_Manager::Create_Scene(const SCENE_TYPE _type) {
	T* scene = new T;
	Scene *scene_tmp;

	switch (_type) {
	case ST_CURRENT:
		scene_tmp = m_scene;
		m_scene = scene;
		break;
	case ST_NEXT:
		scene_tmp = m_next_scene;
		m_next_scene = scene;
		break;
	default:
		SAFE_DELETE(scene);
		return NULL;
	}

	if (!scene->Init()) {
		SAFE_DELETE(scene);

		switch (_type) {
		case ST_CURRENT:
			m_scene = scene_tmp;
			break;
		case ST_NEXT:
			m_next_scene = scene_tmp;
			break;
		}

		return NULL;
	}
	else
		SAFE_DELETE(scene_tmp);

	return scene;
}