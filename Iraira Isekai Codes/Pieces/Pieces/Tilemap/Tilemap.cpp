#pragma once

#include "Tilemap.h"
#include "../../../Singleton/Path_Manager.h"
#include "../../../Singleton/Resource_Manager/Resource_Manager.h"
#include "../../../Singleton/Resource_Manager/Texture.h"
#include "../../../Singleton/Camera.h"
#include "Tile.h"

OBJECT_INFO::OBJECT_INFO() : image_lefttop(0, 0), size(0, 0) { }
OBJECT_INFO::OBJECT_INFO(const POSITION & _image_lefttop, const FLOATSIZE & _size) : image_lefttop(_image_lefttop), size(_size) { }

bool OBJECT_INFO::Included_To_Tilemap(const POSITION &_pos, const FLOATRECT & _tilemap_rect) const {
	const FLOATPOINT tilemap_2D = Get_Tilenum_2D();
	FLOATRECT tile_rect;

	for (size_t j = 0; j < tilemap_2D.y; j++) {
		for (size_t i = 0; i < tilemap_2D.x; i++) {
			tile_rect.left = _pos.x + TILEMAP_TILESIZE.x * i;
			tile_rect.top = _pos.y + TILEMAP_TILESIZE.y * j;
			tile_rect.right = _pos.x + TILEMAP_TILESIZE.x * (i + 1);
			tile_rect.bottom = _pos.y + TILEMAP_TILESIZE.y * (j + 1);

			if (!RectRect_Collision(tile_rect, _tilemap_rect))
				return FALSE;
		}
	}

	return TRUE;
}

Tilemap::Tilemap() : m_tilemap_lefttop(0, 0), m_numwidth(0), m_numheight(0), m_tilewidth(0), m_tileheight(0) { }
Tilemap::Tilemap(const Tilemap & _tilemap) : Unmoving(_tilemap), m_tilemap_lefttop(0, 0), m_numwidth(_tilemap.m_numwidth), m_numheight(_tilemap.m_numheight),
	m_tilewidth(_tilemap.m_tilewidth), m_tileheight(_tilemap.m_tileheight) {

	SAFE_VECLIST_RELEASE(m_tile_vector);

	for (size_t i = 0; i < _tilemap.m_tile_vector.size(); i++)
		m_tile_vector.push_back(_tilemap.m_tile_vector[i]->Clone());
}
Tilemap::~Tilemap() {
	SAFE_VECLIST_RELEASE(m_tile_vector);
}

bool Tilemap::Save(const TCHAR * const _tiledata_filename, const string &_path_key) {
	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_path_key);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _tiledata_filename;

	FILE *file = NULL;
	_tfopen_s(&file, strpath.c_str(), "wb");
	if (!file)
		return FALSE;

	FLOATPOINT tilenum_2D(m_numwidth, m_numheight);
	FLOATSIZE tilesize(m_tilewidth, m_tileheight);
	if (fwrite(&m_tilemap_lefttop, sizeof(POSITION), 1, file) != 1)
		return FALSE;
	if (fwrite(&tilenum_2D, sizeof(FLOATPOINT), 1, file) != 1)
		return FALSE;
	if (fwrite(&tilesize, sizeof(FLOATSIZE), 1, file) != 1)
		return FALSE;

	for (size_t j = 0; j < m_numheight; j++) {
		for (size_t i = 0; i < m_numwidth; i++) {
			Tile *tile = m_tile_vector[j * m_numwidth + i];
			if (!tile->Save(file))
				return FALSE;
		}
	}

	fclose(file);

	return TRUE;
}

bool Tilemap::Clone_Init(const POSITION & _lefttop, const FLOATPOINT & _tilenum_2D, const FLOATSIZE & _tilesize,
	const POSITION & _default_lefttop, const string & _texture_key, const TCHAR * const _filename, const COLORREF & _color,
	const string & _base_key, FILE * const _file) {

	m_tilemap_lefttop = _lefttop;

	if (!Create_Tilemap(_tilenum_2D, _tilesize, _default_lefttop, _texture_key, _filename, _color, _base_key, _file))
		return FALSE;

	return TRUE;
}

bool Tilemap::Clone_Load_Init(const TCHAR * const _tiledata_filename, const string &_tileset_key,
	const TCHAR * const _tileset_filename, const COLORREF &_color, const string &_tiledata_pathkey, const string &_tileset_pathkey) {

	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_tiledata_pathkey);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _tiledata_filename;

	FILE *file = NULL;
	_tfopen_s(&file, strpath.c_str(), "rb");
	if (!file)
		return FALSE;

	POSITION lefttop;
	FLOATPOINT tilenum_2D;
	FLOATSIZE tilesize;
	if (fread(&lefttop, sizeof(POSITION), 1, file) != 1)
		return FALSE;
	if (fread(&tilenum_2D, sizeof(FLOATPOINT), 1, file) != 1)
		return FALSE;
	if (fread(&tilesize, sizeof(FLOATSIZE), 1, file) != 1)
		return FALSE;

	if (!Clone_Init(lefttop, tilenum_2D, tilesize, POSITION(-1, -1), _tileset_key, _tileset_filename, _color, _tileset_pathkey, file))
		return FALSE;

	fclose(file);

	return TRUE;
}

bool Tilemap::Create_Tilemap(const FLOATPOINT &_tilenum_2D, const FLOATSIZE &_tilesize, const POSITION &_default_lefttop,
	const string & _texture_key, const TCHAR * const tileset_filename, const COLORREF & _color, const string & _base_key,
	FILE * const _file) {

	SAFE_VECLIST_RELEASE(m_tile_vector);

	Texture *tileset = GET_SINGLE(Resource_Manager)->Load_Texture(_texture_key, tileset_filename, _color, _base_key);
	if (!tileset)
		return FALSE;

	m_numwidth = _tilenum_2D.x;
	m_numheight = _tilenum_2D.y;
	m_tilewidth = _tilesize.x;
	m_tileheight = _tilesize.y;

	for (size_t j = 0; j < m_numheight; j++) {
		for (size_t i = 0; i < m_numwidth; i++) {
			Tile *tile = new Tile;

			const FLOATSIZE tilesize(m_tilewidth, m_tileheight);
			const POSITION pos(m_tilemap_lefttop.x + i * m_tilewidth, m_tilemap_lefttop.y + j * m_tileheight);

			if (_file) {
				if (!tile->Clone_Load_Init(_default_lefttop, _file, tileset, tilesize, pos))
					return FALSE;
			}
			else {
				if (!tile->Clone_Init(_default_lefttop, tileset, tilesize, pos))
					return FALSE;
			}

			m_tile_vector.push_back(tile);
		}
	}

	SAFE_RELEASE(tileset);

	return TRUE;
}

void Tilemap::Object_Render(const HDC & _hDC, OBJECT_POSITION _object_position) {
	vector<Tile*>::const_iterator it;
	vector<Tile*>::const_iterator it_end = m_tile_vector.end();

	bool front_object = (_object_position == OP_BACK) ? FALSE : TRUE;

	for (it = m_tile_vector.begin(); it != it_end; it++) {
		Tile *tile = *it;
		
		if ((tile->Get_Option_Lefttop() == OPTION_FRONT_OBJECT_IMAGE_LEFTTOP) == front_object) {
			const POSITION object_lefttop = tile->Get_Object_Lefttop();
			const FLOATSIZE size = tile->Get_Size();
			POSITION pos = tile->Get_Pos() - tile->Get_Pivotsize();

			if (SINGLE_DECLARED(Camera))
				pos -= GET_SINGLE(Camera)->Get_Pos();

			const Texture *texture = tile->Get_Texture();
			TransparentBlt(_hDC, pos.x, pos.y, size.x, size.y,
				texture->Get_MemDC(), object_lefttop.x, object_lefttop.y, size.x, size.y, texture->Get_Colorkey());
		}
	}
}

void Tilemap::Option_Render(const HDC & _hDC) {
	vector<Tile*>::const_iterator it;
	vector<Tile*>::const_iterator it_end = m_tile_vector.end();

	for (it = m_tile_vector.begin(); it != it_end; it++) {
		Tile *tile = *it;

		const POSITION option_lefttop = tile->Get_Option_Lefttop();

		if (option_lefttop != OPTION_NORMAL_UNUSING_LEFTTOP) {
			const FLOATSIZE size = tile->Get_Size();
			POSITION pos = tile->Get_Pos() - tile->Get_Pivotsize();

			if (SINGLE_DECLARED(Camera))
				pos -= GET_SINGLE(Camera)->Get_Pos();

			const Texture *texture = tile->Get_Texture();
			TransparentBlt(_hDC, pos.x, pos.y, size.x, size.y,
				texture->Get_MemDC(), option_lefttop.x, option_lefttop.y, size.x, size.y, texture->Get_Colorkey());
		}
	}
}

void Tilemap::Event_Trigger_Render(const HDC & _hDC) {
	vector<Tile*>::const_iterator it;
	vector<Tile*>::const_iterator it_end = m_tile_vector.end();

	for (it = m_tile_vector.begin(); it != it_end; it++) {
		Tile *tile = *it;

		if (tile->Get_Event_Trigger()) {
			const FLOATSIZE size = tile->Get_Size();
			POSITION pos = tile->Get_Pos() - tile->Get_Pivotsize();

			if (SINGLE_DECLARED(Camera))
				pos -= GET_SINGLE(Camera)->Get_Pos();

			const Texture *texture = tile->Get_Texture();
			TransparentBlt(_hDC, pos.x, pos.y, size.x, size.y,
				texture->Get_MemDC(), EVENT_TRIGGER_IMAGE_LEFTTOP.x, EVENT_TRIGGER_IMAGE_LEFTTOP.y, size.x, size.y, texture->Get_Colorkey());
		}
	}
}

void Tilemap::Set_Tilemap_Lefttop(const float _x, const float _y) {
	m_tilemap_lefttop = POSITION(_x, _y);

	for (size_t j = 0; j < m_numheight; j++) {
		for (size_t i = 0; i < m_numwidth; i++)
			m_tile_vector[m_numwidth * j + i]->Set_Pos(m_tilemap_lefttop.x + i * m_tilewidth, m_tilemap_lefttop.y + j * m_tileheight);
	}
}

void Tilemap::Set_Tile_Lefttop(const size_t _x, const size_t _y, const POSITION & _lefttop) {
	m_tile_vector[m_numwidth * _y + _x]->Set_Image_Lefttop(_lefttop);
}

void Tilemap::Set_Tile_Object_Lefttop(const size_t _x, const size_t _y, const OBJECT_INFO & _info) {
	const FLOATPOINT tilenum_2D = _info.Get_Tilenum_2D();

	POSITION object_lefttop;
	for (size_t j = 0; j < tilenum_2D.y; j++) {
		for (size_t i = 0; i < tilenum_2D.x; i++) {
			object_lefttop = POSITION(_info.image_lefttop.x + m_tilewidth * i, _info.image_lefttop.y + m_tileheight * j);
			m_tile_vector[m_numwidth * (_y + j) + (_x + i)]->Set_Object_Lefttop(object_lefttop);
		}
	}
}

void Tilemap::Set_Tile_Object_Lefttop(const size_t _x, const size_t _y, const POSITION & _lefttop) {
	m_tile_vector[m_numwidth * _y + _x]->Set_Object_Lefttop(_lefttop);
}

void Tilemap::Set_Tile_Option_Lefttop(const size_t _x, const size_t _y, const POSITION &_lefttop) {
	m_tile_vector[m_numwidth * _y + _x]->Set_Option_Lefttop(_lefttop);
}

void Tilemap::Set_Tile_Event_Trigger(const size_t _x, const size_t _y) {
	Tile* tile = m_tile_vector[m_numwidth * _y + _x];

	if (tile->Get_Event_Trigger())
		tile->Set_Event_Trigger(FALSE);
	else
		tile->Set_Event_Trigger(TRUE);
}

FLOATRECT Tilemap::Get_Tilemap_Rect() const {
	FLOATRECT tilemap_rect;

	tilemap_rect.left = m_tilemap_lefttop.x;
	tilemap_rect.top = m_tilemap_lefttop.y;
	tilemap_rect.right = m_tilemap_lefttop.x + m_numwidth * m_tilewidth;
	tilemap_rect.bottom = m_tilemap_lefttop.y + m_numheight * m_tileheight;

	return tilemap_rect;
}

POSITION Tilemap::Get_Tile_Lefttop(const POSITION & _tileindex) const {
	POSITION tile_lefttop = m_tilemap_lefttop;
	tile_lefttop.x += m_tilewidth * _tileindex.x;
	tile_lefttop.y += m_tileheight * _tileindex.y;

	return tile_lefttop;
}

POSITION Tilemap::Get_Tile_Image_Lefttop(const size_t _x, const size_t _y) const {
	Tile* tile = m_tile_vector[m_numwidth * _y + _x];
	return tile->Get_Image_Lefttop();
}

POSITION Tilemap::Get_Tile_Object_Lefttop(const size_t _x, const size_t _y) const {
	Tile* tile = m_tile_vector[m_numwidth * _y + _x];
	return tile->Get_Object_Lefttop();
}

POSITION Tilemap::Get_Tile_Option_Lefttop(const size_t _x, const size_t _y) const {
	Tile* tile = m_tile_vector[m_numwidth * _y + _x];
	return tile->Get_Option_Lefttop();
}

bool Tilemap::Get_Tile_Event_Trigger(const size_t _x, const size_t _y) const {
	Tile* tile = m_tile_vector[m_numwidth * _y + _x];
	return tile->Get_Event_Trigger();
}

POSITION Tilemap::Get_Tile_Index(const POSITION & _pos) const {
	const POSITION tile_pos = _pos - m_tilemap_lefttop;
	const POSITION tile_float_index = POSITION(tile_pos.x / m_tilewidth, tile_pos.y / m_tileheight);
	const POSITION tile_int_index = POSITION((int)tile_float_index.x, (int)tile_float_index.y);

	if (Is_Integer(tile_float_index.x) || Is_Integer(tile_float_index.y))
		return TILEMAP_INDEX_ON_LINE;
	else if ((tile_float_index.x < 0 || tile_float_index.x >= m_numwidth) || (tile_float_index.y < 0 || tile_float_index.y >= m_numheight))
		return TILEMAP_INDEX_OUTOF_TILEMAP;
	else
		return tile_int_index;
}

FLOATRECT Tilemap::Get_Tile_Rect(const size_t _x, const size_t _y) const {
	const POSITION lefttop = Get_Tile_Lefttop(_x, _y);
	const FLOATRECT rect = {
		lefttop.x,
		lefttop.y,
		lefttop.x + m_tilewidth,
		lefttop.y + m_tileheight
	};

	return rect;
}

void Tilemap::Render(const HDC & _hDC) {
	for (size_t i = 0; i < m_tile_vector.size(); i++)
		m_tile_vector[i]->Render(_hDC);

	Object_Render(_hDC, OP_BACK);
}