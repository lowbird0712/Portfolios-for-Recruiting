#pragma once

#include "Collider.h"
#include "../Path_Manager.h"

class Pixel_Collider : public Collider {
	// Variables
	BITMAPFILEHEADER		m_file_header;
	BITMAPINFOHEADER		m_info_header;
	vector<COLOR>			m_pixel_vector;
	size_t					m_width;
	size_t					m_height;

	// 필요 시 추가적인 충돌 위치 표시용 변수를 선언할 것
	COLOR					m_color;
	size_t					m_topline;

	// Functions
	bool					Rect_Collision(const FLOATRECT &_rect);
	bool					Circle_Collision(const FLOATCIRCLE &_circle)	{ return FALSE; }
	bool					Point_Collision(const POSITION &_pos)			{ return FALSE; }
public:
	// Functions
	Pixel_Collider();
	Pixel_Collider(const Pixel_Collider &_pixel);
	virtual ~Pixel_Collider();

	virtual Collider*		Clone()											{ return new Pixel_Collider(*this); }

	bool					Set_Pixel_Info(const TCHAR * const _filename,
								const string &_path_key = IMAGE_PATH_KEY);

	void					Set_Color(const COLOR &_color)					{ m_color = _color; }
	void					Set_Topline(const size_t _topline)				{ m_topline = _topline; }
	const vector<COLOR>		Get_Pixel_Vector() const						{ return m_pixel_vector; }
	size_t					Get_Width() const								{ return m_width; }
	size_t					Get_Height() const								{ return m_height; }
	COLOR					Get_Color() const								{ return m_color; }
	size_t					Get_Topline() const								{ return m_topline; }

	virtual void			Input()											{ }
	virtual void			Update()										{ }
	virtual bool			Collision(Collider * const _collider);
	virtual void			Late_Update()									{ }
	virtual void			Render(const HDC &_hDC)							{ }
};