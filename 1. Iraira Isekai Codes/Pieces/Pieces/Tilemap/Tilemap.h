#pragma once

#include "../../Bases/Unmoving.h"

#define TILEMAP_PROTOTYPE_KEY					"Tilemap_Prototype"
#define TILEMAP_TILESIZE						FLOATSIZE(48, 48) /////////////////////////////////////
#define TILEMAP_INDEX_ON_LINE					POSITION(-1.f, -1.f)
#define TILEMAP_INDEX_OUTOF_TILEMAP				POSITION(-1.1f, -1.1f)

#define TILESET_IMAGE_KEY						"Tileset_Image"
#define TILESET_IMAGE_FILENAME					TEXT("NOTYET") ///////////////////////////////////////

#define TILE_NONE_KEY							"Tile_None"
#define TILE_NONE_IMAGE_LEFTTOP					POSITION(336, 144) ///////////////////////////////////////
//#define TILE_HALL_KEY							"Tile_Hall"
//#define TILE_HALL_LEFTTOP						POSITION(192, 96)
//#define TILE_RED_SHEET_KEY						"Tile_Red_Sheet"
//#define TILE_RED_SHEET_LEFTTOP					POSITION(144, 96)
//#define TILE_WOOD_STAGE1_KEY					"Tile_Wood_Stage1"
//#define TILE_WOOD_STAGE1_LEFTTOP				POSITION(144, 144)
//#define TILE_WOOD_STAGE2_KEY					"Tile_Wood_Stage2"
//#define TILE_WOOD_STAGE2_LEFTTOP				POSITION(384, 192)
//#define TILE_STAGE_EDGE_KEY						"Tile_Stage_Edge"
//#define TILE_STAGE_EDGE_LEFTTOP					POSITION(192, 144)

#define OBJECT_NONE_KEY							"Object_None"
#define OBJECT_NONE_UNUSING_LEFTTOP				POSITION(-2.f, -2.f) /////////////////////////////////
//#define OBJECT_NONE_UNUSING_INFO				OBJECT_INFO(OBJECT_NONE_UNUSING_LEFTTOP, TILEMAP_TILESIZE)
//#define OBJECT_STONE_COLUMN_KEY					"Object_Stone_Column"
//#define OBJECT_STONE_COLUMN_INFO				OBJECT_INFO(POSITION(0, 0), FLOATSIZE(48, 96))
//#define OBJECT_STAGE_STAIR_KEY					"Object_Stage_Stair"
//#define OBJECT_STAGE_STAIR_INFO					OBJECT_INFO(POSITION(240, 0), FLOATSIZE(144, 96))
//#define OBJECT_LEFT_STAINED_GLASS_KEY			"Object_Left_Stained_Glass"
//#define OBJECT_LEFT_STAINED_GLASS_INFO			OBJECT_INFO(POSITION(384, 0), FLOATSIZE(48, 96))
//#define OBJECT_RIGHT_STAINED_GLASS_KEY			"Object_Right_Stained_Glass"
//#define OBJECT_RIGHT_STAINED_GLASS_INFO			OBJECT_INFO(POSITION(432, 0), FLOATSIZE(48, 96))
//#define OBJECT_NORMAL_CURTAIN_KEY				"Object_Normal_Curtain"
//#define OBJECT_NORMAL_CURTAIN_INFO				OBJECT_INFO(POSITION(48, 0), FLOATSIZE(144, 96))
//#define OBJECT_SHAPED_CURTAIN_KEY				"Object_Shaped_Curtain"
//#define OBJECT_SHAPED_CURTAIN_INFO				OBJECT_INFO(POSITION(0, 96), FLOATSIZE(144, 96))
//#define OBJECT_SIDE_CURTAIN_KEY					"Object_Side_Curtain"
//#define OBJECT_SIDE_CURTAIN_INFO				OBJECT_INFO(POSITION(192, 0), FLOATSIZE(48, 96))
//#define OBJECT_LEFTTOP_CURTAIN_KEY				"Object_Lefttop_Curtain"
//#define OBJECT_LEFTTOP_CURTAIN_INFO				OBJECT_INFO(POSITION(240, 96), FLOATSIZE(48, 96))
//#define OBJECT_RIGHTTOP_CURTAIN_KEY				"Object_Righttop_Curtain"
//#define OBJECT_RIGHTTOP_CURTAIN_INFO			OBJECT_INFO(POSITION(288, 96), FLOATSIZE(48, 96))
//#define OBJECT_PRINCIPAL_KEY					"Object_Principal"
//#define OBJECT_PRINCIPAL_INFO					OBJECT_INFO(POSITION(384, 96), FLOATSIZE(48, 96))
//#define OBJECT_SITTING_STUDENT_KEY				"Object_Sitting_Student"
//#define OBJECT_SITTING_STUDENT_LEFTTOP			OBJECT_INFO(POSITION(336, 96), FLOATSIZE(48, 48))

#define OPTION_NORMAL_KEY						"Option_Normal"
#define OPTION_NORMAL_UNUSING_LEFTTOP			POSITION(-3.f, -3.f) /////////////////////////////////////
#define OPTION_NOMOVE_KEY						"Option_Nomove"
#define OPTION_NOMOVE_IMAGE_LEFTTOP				POSITION(432, 96) ///////////////////////////////////////////
#define OPTION_FRONT_OBJECT_KEY					"Front_Behind_Object"
#define OPTION_FRONT_OBJECT_IMAGE_LEFTTOP		POSITION(432, 144) ////////////////////////////////////////
#define EVENT_TRIGGER_KEY						"Event_Trigger"
#define EVENT_TRIGGER_IMAGE_LEFTTOP				POSITION(432, 192) ///////////////////////////////////////

enum OBJECT_POSITION {
	OP_FRONT,
	OP_BACK
};

struct OBJECT_INFO {
	POSITION image_lefttop;
	FLOATSIZE size;

	OBJECT_INFO();
	OBJECT_INFO(const POSITION &_image_lefttop, const FLOATSIZE &_size);

	FLOATPOINT	Get_Tilenum_2D() const	{ return FLOATPOINT(size.x / TILEMAP_TILESIZE.x, size.y / TILEMAP_TILESIZE.y); }
	bool		Included_To_Tilemap(const POSITION& _pos, const FLOATRECT& _tilemap_rect) const;
};

class Tile;

class Tilemap : public Unmoving {
	// Variables
	vector<Tile*>		m_tile_vector;
	POSITION			m_tilemap_lefttop;
	size_t				m_numwidth;
	size_t				m_numheight;
	int					m_tilewidth;
	int					m_tileheight;
public:
	// Functions
	Tilemap();
	Tilemap(const Tilemap &_tilemap);
	virtual ~Tilemap();

	bool				Save(const TCHAR * const _tiledata_filename, const string &_path_key = DATA_PATH_KEY);

	Tilemap*			Clone()																					{ return new Tilemap(*this); }
	bool				Clone_Init(const POSITION &_lefttop, const FLOATPOINT &_tilenum_2D, const FLOATSIZE &_tilesize,
							const POSITION &_default_lefttop, const string &_texture_key, const TCHAR * const _filename,
							const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_base_key = IMAGE_PATH_KEY, FILE * const _file = NULL);
	bool				Clone_Load_Init(const TCHAR * const _tiledata_filename,
							const string &_tileset_key, const TCHAR * const _tileset_filename, const COLORREF &_color = TEXTURE_COLORKEY_UNUSE,
							const string &_tiledata_pathkey = DATA_PATH_KEY, const string &_tileset_pathkey = IMAGE_PATH_KEY);

	bool				Create_Tilemap(const FLOATPOINT &_tilenum_2D, const FLOATSIZE &_tilesize,
							const POSITION &_default_lefttop, const string &_texture_key, const TCHAR * const tileset_filename,
							const COLORREF &_color, const string &_base_key, FILE * const _file);

	void				Object_Render(const HDC &_hDC, OBJECT_POSITION _object_position);
	void				Option_Render(const HDC &_hDC);
	void				Event_Trigger_Render(const HDC &_hDC);

	void				Set_Tilemap_Lefttop(const float _x, const float _y);
	void				Set_Tilemap_Lefttop(const POSITION _pos)												{ Set_Tilemap_Lefttop(_pos.x, _pos.y); }
	void				Set_Tile_Lefttop(const size_t _x, const size_t _y, const POSITION &_lefttop);
	void				Set_Tile_Lefttop(const POSITION _tileindex, const POSITION &_lefttop)					{ Set_Tile_Lefttop(_tileindex.x, _tileindex.y, _lefttop); }
	void				Set_Tile_Object_Lefttop(const size_t _x, const size_t _y, const OBJECT_INFO &_info);
	void				Set_Tile_Object_Lefttop(const POSITION &_tileindex, const OBJECT_INFO &_info)			{ Set_Tile_Object_Lefttop(_tileindex.x, _tileindex.y, _info); }
	void				Set_Tile_Object_Lefttop(const size_t _x, const size_t _y, const POSITION &_lefttop);
	void				Set_Tile_Object_Lefttop(const POSITION &_tileindex, const POSITION &_lefttop)			{ Set_Tile_Object_Lefttop(_tileindex.x, _tileindex.y, _lefttop); }
	void				Set_Tile_Option_Lefttop(const size_t _x, const size_t _y, const POSITION &_lefttop);
	void				Set_Tile_Option_Lefttop(const POSITION &_tileindex, const POSITION &_lefttop)			{ Set_Tile_Option_Lefttop(_tileindex.x, _tileindex.y, _lefttop); }
	void				Set_Tile_Event_Trigger(const size_t _x, const size_t _y);
	void				Set_Tile_Event_Trigger(const POSITION &_tileindex)										{ Set_Tile_Event_Trigger(_tileindex.x, _tileindex.y); }
	POSITION			Get_Tilemap_Lefttop() const																{ return m_tilemap_lefttop; }
	size_t				Get_Tilemap_Numwidth() const															{ return m_numwidth; }
	size_t				Get_Tilemap_Numheight() const															{ return m_numheight; }
	FLOATRECT			Get_Tilemap_Rect() const;
	POSITION			Get_Tile_Lefttop(const POSITION &_tileindex) const;
	POSITION			Get_Tile_Lefttop(const size_t _x, const size_t _y) const								{ return Get_Tile_Lefttop(POSITION(_x, _y)); }
	POSITION			Get_Tile_Image_Lefttop(const POSITION &_tileindex) const								{ return Get_Tile_Image_Lefttop(_tileindex.x, _tileindex.y); }
	POSITION			Get_Tile_Image_Lefttop(const size_t _x, const size_t _y) const;
	POSITION			Get_Tile_Object_Lefttop(const POSITION &_tileindex) const								{ return Get_Tile_Object_Lefttop(_tileindex.x, _tileindex.y); }
	POSITION			Get_Tile_Object_Lefttop(const size_t _x, const size_t _y) const;
	POSITION			Get_Tile_Option_Lefttop(const POSITION &_tileindex) const								{ return Get_Tile_Option_Lefttop(_tileindex.x, _tileindex.y); }
	POSITION			Get_Tile_Option_Lefttop(const size_t _x, const size_t _y) const;
	bool				Get_Tile_Event_Trigger(const POSITION &_tileindex) const								{ return Get_Tile_Event_Trigger(_tileindex.x, _tileindex.y); }
	bool				Get_Tile_Event_Trigger(const size_t _x, const size_t _y) const;
	POSITION			Get_Tile_Index(const POSITION &_pos) const;
	POSITION			Get_Tile_Index(const float _x, const float _y) const									{ return Get_Tile_Index(POSITION(_x, _y)); }
	FLOATSIZE			Get_Tile_Size() const																	{ return FLOATSIZE(m_tilewidth, m_tileheight); }
	FLOATRECT			Get_Tile_Rect(const POSITION &_tileindex) const											{ return Get_Tile_Rect(_tileindex.x, _tileindex.y); }
	FLOATRECT			Get_Tile_Rect(const size_t _x, const size_t _y) const;

	virtual bool		Init()																					{ return TRUE; }
	virtual void		Input(const float _deltatime)															{ Unmoving::Input(_deltatime); }
	virtual void		Update(const float _deltatime)															{ Unmoving::Update(_deltatime); }
	virtual void		Late_Update()																			{ Unmoving::Late_Update(); }
	virtual void		Render(const HDC &_hDC);
};