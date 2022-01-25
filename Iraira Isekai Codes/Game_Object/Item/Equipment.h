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
	// ����
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
	// �Լ�
	static string			Get_Early_Clip_Key_Part()					{ return early_clip_key_part; }
	static string			Get_Later_Clip_Key_Part()					{ return later_clip_key_part; }

	// _icon_lefttop�� �������� ��µǴ� ��ġ�� �ƴ� �ؽ��� ������ �������� �ڸ��ϴ� ��ġ�� �ǹ��Ѵ�. ��� �� ������ ��.
	Equipment(const string & _name, const string & _early_clip_key, const string & _later_clip_key,
		const EQUIPMENT_TYPE _type, const POSITION & _icon_lefttop);
	Equipment(Equipment & _equipment);
	virtual ~Equipment();

	// Ŭ�� �߰� �� �߰��� �Լ����̴�.
	// ������ ȿ���� �ߵ���Ű�� �ĵ����� �ִϸ��̼��� �����Ų��.
	virtual void			Use();

	void					Set_Player(Player * const _player)			{ m_player = _player; }
	void					Set_Command_Pressed(const bool _pressed)	{ m_command_pressed = _pressed; }
	string					Get_Name() const							{ return m_name; }
	string					Get_Early_Clip_Key() const					{ return m_early_clip_key; }
	string					Get_Later_Clip_Key() const					{ return m_later_clip_key; }
	EQUIPMENT_TYPE			Get_Type() const							{ return m_type; }
	POSITION				Get_Icon_Image_Lefttop() const				{ return m_icon_image_lefttop; }
};