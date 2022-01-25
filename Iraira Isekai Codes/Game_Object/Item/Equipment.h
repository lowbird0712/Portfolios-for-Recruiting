#pragma once

#include "../../Game.h"

#define FINITE_LATER_CLIP_KEY			"Doesn't have later animation!"
#define EQUIPMENT_ICON_IMAGE_KEY		"Equipment_Icon_Image_Key"
#define EQUIPMENT_ICON_IMAGE_FILENAME	TEXT("Equipment_Icon.bmp")

#define EQUIPMENT_ICON_SIZE				FLOATSIZE(60.f, 60.f)

enum EQUIPMENT_TYPE {
	ET_NONE,
	ET_WEAPON,
	ET_SHIELD,
	ET_POTION,
};

class Object;
class Player;

class Equipment {
protected:
	// 변수
	static const string		early_clip_key_part;
	static const string		later_clip_key_part;

	const string			m_name;
	const string			m_early_clip_key;
	const string			m_later_clip_key;
	const EQUIPMENT_TYPE	m_type;
	const POSITION			m_icon_image_lefttop;

	Player *				m_player = NULL;

	bool					m_command_pressed = FALSE;
public:
	// 함수
	static string			Get_Early_Clip_Key_Part()					{ return early_clip_key_part; }
	static string			Get_Later_Clip_Key_Part()					{ return later_clip_key_part; }

	// _icon_lefttop은 아이콘이 출력되는 위치가 아닌 텍스쳐 내에서 아이콘이 자리하는 위치를 의미한다. 사용 시 주의할 것.
	Equipment(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const EQUIPMENT_TYPE _type, const POSITION & _icon_lefttop);
	Equipment(Equipment & _equipment);
	virtual ~Equipment();

	// 클립 추가 시 추가할 함수들이다.
	// 무기의 효과를 발동시키고 후딜레이 애니메이션을 재생시킨다.
	virtual void			Use();

	void					Set_Player(Player * const _player)			{ m_player = _player; }
	void					Set_Command_Pressed(const bool _pressed)	{ m_command_pressed = _pressed; }
	string					Get_Name() const							{ return m_name; }
	string					Get_Early_Clip_Key() const					{ return m_early_clip_key; }
	string					Get_Later_Clip_Key() const					{ return m_later_clip_key; }
	EQUIPMENT_TYPE			Get_Type() const							{ return m_type; }
	POSITION				Get_Icon_Image_Lefttop() const				{ return m_icon_image_lefttop; }
};