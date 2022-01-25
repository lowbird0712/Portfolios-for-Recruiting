#pragma once

#include "../../Game.h"

class Scene;
class Object;

class Layer {
	// Variables
	list<Object*>	m_objlist;

	string			m_strtag;
	Scene*			m_scene;
	int				m_zorder;
	bool			m_enable;
	bool			m_live;
public:
	// Functions
	Layer();
	~Layer();

	void			Add_Object(Object* const _object);
	void			Erase_Object(const string &_object_key);
	Object*			Find_Object(const string &_object_key) const;

	void			Set_Scene(Scene* const _scene)			{ m_scene = _scene; }
	void			Set_Strtag(const string &_layer_key)	{ m_strtag = _layer_key; }
	void			Set_Zorder(const int _zorder)			{ m_zorder = _zorder; }
	void			Set_Enable(const bool _enable)			{ m_enable = _enable; }
	void			Die()									{ m_live = FALSE; }
	list<Object*>&	Get_Objlist()							{ return m_objlist; }
	Scene*			Get_Scene() const						{ return m_scene; }
	string			Get_Strtag() const						{ return m_strtag; }
	int				Get_Zorder() const						{ return m_zorder; }
	bool			Get_Enable() const						{ return m_enable; }
	bool			Get_Live() const						{ return m_live; }

	void			Input(const float _deltatime);
	void			Update(const float _deltatime);
	void			Add();
	void			Late_Update();
	void			Render(const HDC &_hDC);
};