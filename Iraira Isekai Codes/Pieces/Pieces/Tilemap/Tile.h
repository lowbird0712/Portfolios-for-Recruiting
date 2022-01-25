#pragma once

#include "../../Bases/Unmoving.h"

class Texture;

class Tile : public Unmoving {
	// Variables
	POSITION		m_option_lefttop;
	POSITION		m_object_lefttop;
	bool			m_event_trigger;
public:
	// Functions
	Tile();
	Tile(const Tile &_tile);
	virtual ~Tile();

	bool			Save(FILE * const _file);

	virtual Tile*	Clone()											{ return new Tile(*this); }
	bool			Clone_Init(const POSITION &_default_lefttop, Texture * const _texture, const FLOATSIZE &_size, const POSITION &_pos);
	bool			Clone_Load_Init(const POSITION &_default_lefttop, FILE * const _file, Texture * const _texture,
						const FLOATSIZE &_size, const POSITION &_pos);

	void			Set_Option_Lefttop(const POSITION &_lefttop)	{ m_option_lefttop = _lefttop; }
	void			Set_Object_Lefttop(const POSITION &_lefttop)	{ m_object_lefttop = _lefttop; }
	void			Set_Event_Trigger(const bool _trigger)			{ m_event_trigger = _trigger; }
	POSITION		Get_Option_Lefttop() const						{ return m_option_lefttop; }
	POSITION		Get_Object_Lefttop() const						{ return m_object_lefttop; }
	bool			Get_Event_Trigger() const						{ return m_event_trigger; }

	virtual bool	Init()											{ return TRUE; }
	virtual void	Render(const HDC &_hDC)							{ Unmoving::Render(_hDC); }
};