#pragma once

#include "../Game.h"
#include "Core.h"

#define CAMERA_TARGET_UNUSE_PIVOT		PIVOT(-1.f, -1.f)
#define CAMERA_SCROLL_SPEED				500.f

#define CAMERA_LEFTMOVE_KEY				"Editing_Camera_Leftmove"
#define CAMERA_RIGHTMOVE_KEY			"Editing_Camera_Rightmove"
#define CAMERA_UPMOVE_KEY				"Editing_Camera_Upmove"
#define CAMERA_DOWNMOVE_KEY				"Editing_Camera_Downmove"

#define GET_WORLDMOUSEPOS()				(GET_SINGLE(Camera)->Get_Pos() + GET_MOUSEPOS())

class Object;

class Camera {
	DECLARE_SINGLE(Camera)
private:
	// Variables
	POSITION		m_pos;
	RESOLUTION		m_client_resolution;
	RESOLUTION		m_world_resolution;
	FLOATSIZE		m_pivot;
	Object*			m_target;
	float			m_scroll_speed;
public:
	// Functions
	void		Set_Target(Object * const _target);
	void		Release_Target();

	void		Set_Pos(const float _x, const float _y)									{ m_pos = POSITION(_x, _y); }
	void		Set_Pos(const POSITION &_pos)											{ m_pos = _pos; }
	void		Set_Client_Resolution(const unsigned int _x, const unsigned int _y)		{ m_client_resolution = RESOLUTION(_x, _y); }
	void		Set_Client_Resolution(const RESOLUTION &_resolution)					{ m_client_resolution = _resolution; }
	void		Set_World_Resolution(const unsigned int _x, const unsigned int _y)		{ m_world_resolution = RESOLUTION(_x, _y); }
	void		Set_World_Resolution(const RESOLUTION &_resolution)						{ m_world_resolution = _resolution; }
	void		Set_Pivot(const float _x, const float _y)								{ m_pivot = POSITION(_x, _y); }
	void		Set_Pivot(const PIVOT &_pivot)											{ m_pivot = _pivot; }
	POSITION	Get_Pos() const															{ return m_pos; }
	RESOLUTION	Get_Client_Resolution() const											{ return m_client_resolution; }
	RESOLUTION	Get_World_Resolution() const											{ return m_world_resolution; }
	string		Get_Target_Strtag() const;

	bool		Init(const POSITION &_pos, const RESOLUTION &_client_resolution, const PIVOT &_pivot = CAMERA_TARGET_UNUSE_PIVOT);
	void		Input(const float _deltatime);
	void		Update();
};