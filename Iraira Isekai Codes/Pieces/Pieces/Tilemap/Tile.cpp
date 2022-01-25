#pragma once

#include "Tile.h"
//#include "../../../Singleton/Camera.h"
#include "../../../Singleton/Resource_Manager/Texture.h"
#include "../../Pieces/Tilemap/Tilemap.h"

Tile::Tile() : m_option_lefttop(OPTION_NORMAL_UNUSING_LEFTTOP), m_object_lefttop(OBJECT_NONE_UNUSING_LEFTTOP), m_event_trigger(FALSE) { }
Tile::Tile(const Tile & _tile) : Unmoving(_tile), m_option_lefttop(_tile.m_option_lefttop),
	m_object_lefttop(_tile.m_object_lefttop), m_event_trigger(_tile.m_event_trigger) { }
Tile::~Tile() { }

bool Tile::Save(FILE * const _file) {
	if (!_file)
		return FALSE;

	if (fwrite(&m_image_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;
	if (fwrite(&m_object_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;
	if (fwrite(&m_option_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;

	return TRUE;
}

bool Tile::Clone_Init(const POSITION &_default_lefttop, Texture * const _texture, const FLOATSIZE & _size, const POSITION & _pos) {
	m_image_lefttop = _default_lefttop;
	Set_Texture(_texture);
	m_size = _size;
	m_pos = _pos;

	return TRUE;
}

bool Tile::Clone_Load_Init(const POSITION &_default_lefttop, FILE * const _file, Texture * const _texture,
							const FLOATSIZE &_size, const POSITION &_pos) {

	if (!_file)
		return FALSE;

	if (!Clone_Init(_default_lefttop, _texture, _size, _pos))
		return FALSE;

	if (fread(&m_image_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;
	if (fread(&m_object_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;
	if (fread(&m_option_lefttop, sizeof(POSITION), 1, _file) != 1)
		return FALSE;

	return TRUE;
}
