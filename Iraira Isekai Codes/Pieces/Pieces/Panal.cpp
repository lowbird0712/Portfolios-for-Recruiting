#pragma once

#include "Panal.h"
#include "../../Singleton/Resource_Manager/Texture.h"

Panal::Panal() { }
Panal::Panal(const Panal & _panal) : Unmoving(_panal) { }
Panal::~Panal() { }

bool Panal::Clone_Init(const POSITION &_pos, const string & _texture_key, const TCHAR * _filename, const COLORREF & _color, const string & _base_key) {
	if (!Set_Texture(_texture_key, _filename, _color, _base_key)) {
		printf("Panal::Clone_Init -> Set_Texture 에러\n");
		return FALSE;
	}

	m_size = m_texture->Get_Frame_Size();
	m_pos = _pos;

	return TRUE;
}

bool Panal::Clone_Init(const POSITION &_pos, const string & _texture_key) {
	Texture *texture = GET_SINGLE(Resource_Manager)->Find_Texture(_texture_key);
	if (!Set_Texture(texture)) {
		printf("Panal::Clone_Init -> Set_Texture 에러\n");
		return FALSE;
	}

	m_size = m_texture->Get_Frame_Size();
	m_pos = _pos;

	SAFE_RELEASE(texture);

	return TRUE;
}

bool Panal::Clone_Init(const POSITION & _pos, const POSITION & _image_lefttop, const FLOATSIZE & _size,
	const string & _texture_key, const TCHAR * _filename, const COLORREF & _color, const string & _base_key) {

	if (!Clone_Init(_pos, _texture_key, _filename, _color, _base_key))
		return FALSE;

	m_image_lefttop = _image_lefttop;
	m_size = _size;

	return TRUE;
}

bool Panal::Clone_Init(const POSITION & _pos, const POSITION & _image_lefttop, const FLOATSIZE & _size, const string & _texture_key) {
	if (!Clone_Init(_pos, _texture_key))
		return FALSE;

	m_image_lefttop = _image_lefttop;
	m_size = _size;

	return TRUE;
}
