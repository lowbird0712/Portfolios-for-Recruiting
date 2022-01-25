#include "Collider.h"

Collider::Collider() : m_type(CT_NONE), m_object(NULL), m_strtag("") { }
Collider::Collider(const Collider & _collider) : m_type(_collider.m_type), m_object(NULL), m_strtag(_collider.m_strtag) {
	list<function<void(Collider * const, Collider * const, const float)>>::const_iterator funcit;
	list<function<void(Collider * const, Collider * const, const float)>>::const_iterator funcit_end;

	for (size_t i = 0; i < CS_END; i++) {
		m_funclist[i].clear();
		funcit_end = _collider.Get_Funclist()[i].end();

		for (funcit = _collider.Get_Funclist()[i].begin(); funcit != funcit_end; funcit++)
			m_funclist[i].push_back(*funcit);
	}
}
Collider::~Collider() { }

int Collider::Release() {
	if (!m_colliding_list.empty()) {
		list<Collider*>::iterator colit;
		list<Collider*>::iterator colit_end = m_colliding_list.end();

		for (colit = m_colliding_list.begin(); colit != colit_end; colit++) {
			(*colit)->Erase_Collider(this);
			(*colit)->Reference::Release();
		}
	}

	Reference::Release();

	return m_ref;
}

void Collider::Add_Collider(Collider * const _collider) {
	_collider->Add_Ref();
	m_colliding_list.push_back(_collider);
}

void Collider::Erase_Collider(Collider * const _collider) {
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_colliding_list.end();

	for (it = m_colliding_list.begin(); it != it_end; it++) {
		if (*it == _collider) {
			(*it)->Reference::Release();
			m_colliding_list.erase(it);
			return;
		}
	}
}

bool Collider::Check_Colliding_List(Collider * const _collider) {
	list<Collider*>::iterator it;
	list<Collider*>::iterator it_end = m_colliding_list.end();

	for (it = m_colliding_list.begin(); it != it_end; it++) {
		if (*it == _collider)
			return TRUE;
	}

	return FALSE;
}

void Collider::Add_Function(const COLLISION_STATE _state, void(*const _func)(Collider * const, Collider * const, const float)) {

	function<void(Collider * const, Collider * const, const float)> func_tmp;
	func_tmp = bind(_func, placeholders::_1, placeholders::_2, placeholders::_3);

	m_funclist[_state].push_back(func_tmp);
}

void Collider::Call_function(const COLLISION_STATE _state, Collider * const _collider, const float _deltatime) {
	list<function<void(Collider * const, Collider * const, const float)>>::iterator it;
	list<function<void(Collider * const, Collider * const, const float)>>::iterator it_end = m_funclist[_state].end();

	for (it = m_funclist[_state].begin(); it != it_end; it++)
		(*it)(this, _collider, _deltatime);
}