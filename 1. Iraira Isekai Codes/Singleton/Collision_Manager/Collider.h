#pragma once

#include "../../Pieces/Bases/Reference.h"

enum COLLIDER_TYPE {
	CT_NONE,
	CT_RECT,
	CT_CIRCLE,
	CT_LINE,
	CT_POINT,
	CT_PIXEL,
	CT_END
};

enum COLLISION_STATE {
	CS_ENTER,
	CS_STAY,
	CS_LEAVE,
	CS_END
};

class Object;

class Collider : public Reference {
protected:
	// Varibles
	list<Collider*>															m_colliding_list;
	list<function<void(Collider * const, Collider * const, const float)>>	m_funclist[CS_END];

	COLLIDER_TYPE														    m_type;
	Object*																	m_object;
	string																    m_strtag;

	// Functions
	Collider();
	Collider(const Collider &_collider);
	virtual ~Collider() = 0;
public:
	// Functions
	virtual Collider*	Clone() = 0;

	int					Release();

	void				Add_Collider(Collider * const _collider);
	void				Erase_Collider(Collider * const _collider);
	bool				Check_Colliding_List(Collider * const _collider);

	void				Add_Function(const COLLISION_STATE _state,
							void(*const _func)(Collider * const, Collider * const, const float));
	template <typename T>
	void				Add_Function(const COLLISION_STATE _state, T * const _object,
							void(T::* const _func)(Collider * const, Collider * const, const float));
	void				Call_function(const COLLISION_STATE _state, Collider * const _collider, const float _deltatime);

	void				Set_Type(const COLLIDER_TYPE &_type)											{ m_type = _type; }
	void				Set_Object(Object* const _object)												{ m_object = _object; }
	void				Set_Strtag(const string &_coll_key)												{ m_strtag = _coll_key; }
	list<Collider*>		Get_Colliding_List() const														{ return m_colliding_list; } // 변수에 절대 집어넣지 말 것
	list<function<void(Collider * const, Collider * const, const float)>> const * Get_Funclist() const	{ return m_funclist; }
	COLLIDER_TYPE		Get_Type() const																{ return m_type; }
	Object*				Get_Object() const																{ return m_object; } // 변수에 절대 집어넣지 말 것
	string				Get_Strtag() const																{ return m_strtag; }

	virtual void		Input() = 0;
	virtual void		Update() = 0;
	virtual bool		Collision(Collider * const _collider) = 0;
	virtual void		Late_Update() = 0;
	virtual void		Render(const HDC &_hDC) = 0;
};

template<typename T>
inline void Collider::Add_Function(const COLLISION_STATE _state, T * const _object,
	void(T::* const _func)(Collider * const, Collider * const, const float)) {

	function<void(Collider * const, Collider * const, const float)> func_tmp;
	func_tmp = bind(_func, _object, placeholders::_1, placeholders::_2, placeholders::_3);

	m_funclist[_state].push_back(func_tmp);
}