#pragma once

#include "../../Game.h"

class Object;
class Collider;

class Collision_Manager {
	DECLARE_SINGLE(Collision_Manager)
private:
	// Variables
	list<Object*>	m_objlist;
public:
	// Functions
	void			Add_Object(Object * const _object);

	bool			Object_Constraint(Object * const _object1, Object * const _object2);
	bool			Collider_Constraint(Collider * const _collider1, Collider * const _collider2);

	bool			Init()									{ return TRUE; }
	void			Collision(const float _deltatime);
	void			Collision(Object * const _object1, Object * const _object2, const float _deltatime);
};