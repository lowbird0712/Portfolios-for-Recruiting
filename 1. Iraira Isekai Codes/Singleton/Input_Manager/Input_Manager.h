#pragma once

#include "../../Game.h"

#define MOUSE_KEY					"Mouse"
#define MOUSE_LEFT_KEY				"Mouse_Left"
#define MOUSE_RIGHT_KEY				"Mouse_Right"

#define GET_KEYDOWN(key)			(GET_SINGLE(Input_Manager)->Get_Down(key))
#define GET_KEYPRESSED(key)			(GET_SINGLE(Input_Manager)->Get_Pressed(key))
#define GET_KEYUP(key)				(GET_SINGLE(Input_Manager)->Get_Up(key))
#define GET_KEYDOWNPRESSED(key)		(GET_KEYDOWN(key) || GET_KEYPRESSED(key))
#define GET_MOUSE_CLICKRANGE()		(GET_SINGLE(Input_Manager)->Get_Mouse_Clickrange())
#define GET_MOUSEPOS()				(GET_SINGLE(Input_Manager)->Get_Mousepos())
#define GET_MOUSEDIR()				(GET_SINGLE(Input_Manager)->Get_Mousedir())

class Mouse;
class Collider;

typedef struct _tagKEYINFO {
	string			strtag;
	vector<DWORD>	button_vector;
	bool			down;
	bool			pressed;
	bool			up;

	_tagKEYINFO() : strtag(""), down(FALSE), pressed(FALSE), up(FALSE) { }
} KEYINFO, *PKEYINFO;

class Input_Manager {
	DECLARE_SINGLE(Input_Manager)
private:
	// Variables
	HWND								m_hWnd;
	unordered_map<string, PKEYINFO>		m_keymap;
	PKEYINFO							m_creating_key;

	Mouse*								m_mouse;

	// Functions
	PKEYINFO							Find_Key(const string &_keyinfo_key) const;
public:
	// Functions
	template<typename T>
	bool								Add_Key(const T &_keyinfo_key);
	template<typename T, typename ... Types>
	bool								Add_Key(const T &_button, const Types& ..._args);

	unordered_map<string, PKEYINFO>		Get_Keymap() const									{ return m_keymap; }
	bool								Get_Down(const string &_keyinfo_key) const;
	bool								Get_Pressed(const string &_keyinfo_key) const;
	bool								Get_Up(const string &_keyinfo_key) const;
	Collider*							Get_Mouse_Clickrange() const;
	POSITION							Get_Mousepos() const;
	FLOATSIZE							Get_Mousedir() const;

	bool								Init(HWND _hWnd);
	void								Update();
	void								Render(HDC _hDC);
};

template<typename T>
inline bool Input_Manager::Add_Key(const T & _keyinfo_key) {
	const char *type_name = typeid(T).name(); // 인자로 들어온 자료형의 이름을 문자열로 반환해준다

	if (strcmp(type_name, "char") == 0 || strcmp(type_name, "int") == 0)
		return FALSE;
	else {
		m_creating_key->strtag = _keyinfo_key;
		m_keymap.insert(make_pair(m_creating_key->strtag, m_creating_key));
	}

	return TRUE;
}

template<typename T, typename ...Types>
inline bool Input_Manager::Add_Key(const T & _button, const Types & ..._args) {
	if (!m_creating_key)
		m_creating_key = new KEYINFO;

	const char *type_name = typeid(T).name();

	if (strcmp(type_name, "char") == 0 || strcmp(type_name, "int") == 0)
		m_creating_key->button_vector.push_back((DWORD)_button);
	else {
		SAFE_DELETE(m_creating_key);
		return FALSE;
	}

	Add_Key(_args...);

	if (m_creating_key)
		m_creating_key = NULL;

	return TRUE;
}