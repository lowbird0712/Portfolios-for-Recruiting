#pragma once

#include "../../Game.h"
#include "Layer.h"

#define BACKGROUND_LAYER_KEY			"Background_Layer"
#define DEFAULT_LAYER_KEY				"Default_Layer"
#define UI_LAYER_KEY					"UI_Layer"

class Object;

class Scene {
protected:
	// Variables
	unordered_map<string, Object*>	m_protomap;
	list<Layer*>					m_layerlist;

	// Functions
	// sort 함수는 전역 함수 포인터만을 인자로 받기 때문에 static 함수로 선언해준다.
	static bool							Compare_Layer(const Layer* const _layer1, const Layer* const _layer2)
											{ return _layer1->Get_Zorder() < _layer2->Get_Zorder(); }
	Layer*								Create_Layer(const string &_layer_key, const int _zorder = 0);
	void								Erase_layer(const string &_layer_key);

	void								Erase_Prototype(const string &_proto_key);
	void								Erase_Prototype();

	template <typename T> // 함수 내에서 초기화가 일어난다.
	bool								Create_Object(const string &_object_key, Layer* const _layer);
	void								Erase_Object(const string &_object_key, const string &_layer_key = "");
	Object*								Create_Clone(const string &_proto_key, const string &_clone_key, Layer* const _layer);
public:
	// Functions
	Scene();
	virtual ~Scene() = 0;

	Layer*								Find_Layer(const string &_layer_key) const;

	template <typename T> // 함수 내에서 초기화가 일어난다.
	bool								Create_Prototype(const string &_proto_key);
		// 게임 오브젝트를 초기화할 때 하위 게임 오브젝트의 프로토타입을 만들어야 하는 경우가 있기 때문에 public으로 한다.
	Object*								Find_Prototype(const string &_proto_key) const;

	Object*								Find_Object(const string &_object_key, const string &_layer_key = "") const;

	unordered_map<string, Object*>&		Get_Protomap()		{ return m_protomap; } // 지속적으로 사용할 변수에 대입하지 말 것.
	list<Layer*>&						Get_Layerlist()		{ return m_layerlist; }

	virtual bool						Init();
	virtual void						Input(const float _deltatime);
	virtual void						Update(const float _deltatime);
	virtual void						Add();
	virtual void						Late_Update();
	virtual void						Render(const HDC &hDC);
};

template<typename T>
inline bool Scene::Create_Prototype(const string & _proto_key) {
	T *prototype = new T;
	prototype->Set_Strtag(_proto_key);
	prototype->Set_Scene(this);

	if (!prototype->Init()) {
		SAFE_RELEASE(prototype);
		return FALSE;
	}

	m_protomap.insert(make_pair(_proto_key, prototype));

	return TRUE;
}

template<typename T>
inline bool Scene::Create_Object(const string & _object_key, Layer * const _layer) {
	T *object = new T;

	object->Set_Strtag(_object_key);
	_layer->Add_Object(object);

	if (!object->Init()) {
		SAFE_RELEASE(object);
		return FALSE;
	}

	SAFE_RELEASE(object);
	return TRUE;
}