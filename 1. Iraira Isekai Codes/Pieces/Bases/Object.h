#pragma once

#include "Reference.h"
#include "../../Singleton/Collision_Manager/Collider.h"
#include "../Pieces/Animation.h"

#define TEXTURE_UNUSE_IMAGE_LEFTTOP	POSITION(-1, -1)
#define FILELOAD_UNUSE				TEXT("")

class Scene;
class Layer;
class Texture;

class Object : public Reference {
protected:
	// Variables
	list<Collider*>			m_collist;
	Texture*				m_texture;
	Animation*				m_animation;
	POSITION				m_image_lefttop;

	string					m_strtag;
	Scene*					m_scene;
	Layer*					m_layer;

	POSITION				m_pos;
	FLOATSIZE				m_size;
	PIVOT					m_pivot;
public:
	// Functions
	Object();
	Object(const Object &_object);
	virtual ~Object();

	virtual Object*			Clone() = 0;

	// 이동 관련
	void					Move(const float _x, const float _y)					{ m_pos.x += _x; m_pos.y += _y; }
	void					Move(const FLOATSIZE &_size)							{ m_pos += _size; }

	// Collider 관련
	template <typename T>
	T*						Add_Collider(const string &_coll_key);
	template <typename T>
	void					Add_Function(const string &_coll_key, const COLLISION_STATE _state, T * const _object,
								void(T::* const _func)(Collider * const, Collider * const, const float));
	bool					Using_Collider()										{ return !m_collist.empty(); }

	// Texture 관련
	bool					Set_Texture(Texture *_texture);
	bool					Set_Texture(const string &_texture_key, const TCHAR * const _filename,
								const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_path_key = IMAGE_PATH_KEY);
	void					Release_Texture();

	// Animation 관련
	void					Create_Animation(const string &_ani_key);
	bool					Add_Atlas_Clip(const string &_clip_key, const CLIP_INPUT &_clip,
								const string &_texture_key, const TCHAR *_filename, const COLORREF &_color = TEXTURE_COLORKEY_UNUSE,
								const string &_path_key = SPRITE_PATH_KEY);

	void					Set_Image_Lefttop(const float _left, const float _top)	{ m_image_lefttop.x = _left; m_image_lefttop.y = _top; }
	void					Set_Image_Lefttop(const POSITION _pos)					{ m_image_lefttop = _pos; }
	void					Set_Strtag(const string &_obj_key)						{ m_strtag = _obj_key; }
	void					Set_Scene(Scene* const _scene)							{ m_scene = _scene; }
	void					Set_Layer(Layer* const _layer)							{ m_layer = _layer; }
	void					Set_Pos(const float _x, const float _y)					{ m_pos = POSITION(_x, _y); }
	void					Set_Pos(const POSITION &_pos)							{ m_pos = _pos; }
	void					Set_Pos(const POINT &_pos)								{ m_pos = _pos; }
	void					Set_Size(const float _x, const float _y)				{ m_size = FLOATSIZE(_x, _y); }
	void					Set_Size(const FLOATSIZE &_size)						{ m_size = _size; }
	void					Set_Pivot(const float _x, const float _y)				{ m_pivot.x = _x; m_pivot.y = _y; }
	void					Set_Pivot(const PIVOT &_pivot)							{ m_pivot = _pivot; }
	list<Collider*>			Get_Collist() const										{ return m_collist; }   // 변수에 절대 집어넣지 말 것
	Animation*				Get_Animation() const									{ return m_animation; } // 변수에 절대 집어넣지 말 것
	POSITION				Get_Image_Lefttop() const								{ return m_image_lefttop; }
	string					Get_Strtag() const										{ return m_strtag; }
	Scene*					Get_Scene() const										{ return m_scene; }
	Layer*					Get_Layer() const										{ return m_layer; }
	Texture*				Get_Texture() const										{ return m_texture; }   // 변수에 절대 집어넣지 말 것
	POSITION				Get_Pos() const											{ return m_pos; }
	FLOATSIZE				Get_Size() const										{ return m_size; }
	FLOATSIZE   			Get_Pivot() const										{ return m_pivot; }
	FLOATSIZE   			Get_Pivotsize() const									{ return m_size * m_pivot; }
	FLOATSIZE				Get_Lefttop_Pos() const									{ return m_pos - Get_Pivotsize(); }
	FLOATRECT				Get_Rect() const;
	POSITION				Get_Center() const;

	virtual bool			Init() = 0;
	virtual void			Input(const float _deltatime);
	virtual void			Update(const float _deltatime);
	virtual void			Late_Update();
	virtual void			Render(const HDC &_hDC);
};

template<typename T>
inline T * Object::Add_Collider(const string & _coll_key) {
	T *collider = new T;

	collider->Set_Object(this);
	collider->Set_Strtag(_coll_key);

	collider->Add_Ref();
	m_collist.push_back(collider);

	return collider;
}

template<typename T>
inline void Object::Add_Function(const string & _coll_key, const COLLISION_STATE _state, T * const _object,
	void(T::* const _func)(Collider * const, Collider * const, const float)) {

	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_collist.end();

	for (it = m_collist.begin(); it != it_end; it++) {
		if (_coll_key == (*it)->Get_Strtag()) {
			(*it)->Add_Function(_state, _object, _func);
			break;
		}
	}
}