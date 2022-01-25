#pragma once

#include "Collision_Manager.h"
#include "../../Pieces/Bases/Object.h"
#include "Collider.h"

DEFINITION_SINGLE(Collision_Manager);

Collision_Manager::Collision_Manager() { }
Collision_Manager::~Collision_Manager() { }

void Collision_Manager::Add_Object(Object * const _object) {
	if (_object->Using_Collider()) {
		_object->Add_Ref();
		m_objlist.push_back(_object);
	}
}

bool Collision_Manager::Object_Constraint(Object * const _object1, Object * const _object2) {
	// 충돌 판정을 하지 않는 특정 오브젝트인지를 체크

	return FALSE;
}

bool Collision_Manager::Collider_Constraint(Collider * const _collider1, Collider * const _collider2) {
	// 충돌 판정을 하지 않는 특정 충돌체인지를 체크

	return FALSE;
}

void Collision_Manager::Collision(const float _deltatime) {
	if (m_objlist.size() >= 2) {
		list<Object*>::iterator it;
		list<Object*>::iterator it_end = m_objlist.end();
		it_end--;

		for (it = m_objlist.begin(); it != it_end; it++) {
			list<Object*>::iterator inner_it = it;
			list<Object*>::iterator inner_it_end = m_objlist.end();
			inner_it++;

			for (; inner_it != inner_it_end; inner_it++) {
				if (Object_Constraint(*it, *inner_it))
					continue;

				Collision(*it, *inner_it, _deltatime);
			}
		}
	}

	SAFE_VECLIST_RELEASE(m_objlist);
	m_objlist.clear();
}

void Collision_Manager::Collision(Object * const _object1, Object * const _object2, const float _deltatime) {
	list<Collider*> source_list = _object1->Get_Collist();
	list<Collider*> dest_list = _object2->Get_Collist();

	list<Collider*>::const_iterator it1;
	list<Collider*>::const_iterator it1_end = source_list.end();
	list<Collider*>::const_iterator it2;
	list<Collider*>::const_iterator it2_end = dest_list.end();

	for (it1 = source_list.begin(); it1 != it1_end; it1++) {
		for (it2 = dest_list.begin(); it2 != it2_end; it2++) {
			if (Collider_Constraint(*it1, *it2))
				continue;

			if ((*it1)->Collision(*it2)) {
				if (!(*it1)->Check_Colliding_List(*it2)) {
					(*it1)->Add_Collider(*it2);
					(*it2)->Add_Collider(*it1);

					(*it1)->Call_function(CS_ENTER, *it2, _deltatime);
					(*it2)->Call_function(CS_ENTER, *it1, _deltatime);
				}
				else {
					(*it1)->Call_function(CS_STAY, *it2, _deltatime);
					(*it2)->Call_function(CS_STAY, *it1, _deltatime);
				}
			}
			else {
				if ((*it1)->Check_Colliding_List(*it2)) {
					(*it1)->Erase_Collider(*it2);
					(*it2)->Erase_Collider(*it1);

					(*it1)->Call_function(CS_LEAVE, *it2, _deltatime);
					(*it2)->Call_function(CS_LEAVE, *it1, _deltatime);
				}
			}
		}
	}
}
