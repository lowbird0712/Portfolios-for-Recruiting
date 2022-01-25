#pragma once

#include "Pixel_Collider.h"
#include "../Resource_Manager/Resource_Manager.h"
#include "Rect_Collider.h"
#include "Circle_Collider.h"
#include "Point_Collider.h"

Pixel_Collider::Pixel_Collider()
	: m_file_header(BITMAPFILEHEADER()), m_info_header(BITMAPINFOHEADER()),
	m_width(0.f), m_height(0.f), m_color(COLOR_MAGENTA), m_topline(0) {
	m_type = CT_PIXEL;
}
Pixel_Collider::Pixel_Collider(const Pixel_Collider & _pixel)
	: Collider(_pixel), m_file_header(_pixel.m_file_header), m_info_header(_pixel.m_info_header),
	m_pixel_vector(_pixel.m_pixel_vector), m_width(_pixel.m_width), m_height(_pixel.m_height), m_color(_pixel.m_color), m_topline(0) { }
Pixel_Collider::~Pixel_Collider() { }

bool Pixel_Collider::Rect_Collision(const FLOATRECT & _rect) {
	size_t start_x, end_x;
	size_t start_y, end_y;

	start_x = _rect.left < 0 ? 0 : _rect.left;
	end_x = _rect.right >= m_width ? m_width - 1 : _rect.right;
	start_y = _rect.top < 0 ? 0 : _rect.top;
	end_y = _rect.bottom >= m_height ? m_height - 1 : _rect.bottom;

	for (size_t i = start_y; i <= end_y; i++) {
		for (size_t j = start_x; j <= end_x; j++) {
			size_t index = i * m_width + j;
			COLOR pixel = m_pixel_vector[index];
			if (pixel == m_color) {
				m_topline = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}

bool Pixel_Collider::Set_Pixel_Info(const TCHAR * const _filename, const string & _path_key) {
	const TCHAR *path = GET_SINGLE(Path_Manager)->Find_Path(_path_key);
	tstring strpath;
	if (path)
		strpath = path;
	strpath += _filename;

	FILE *file = NULL;
	fopen_s(&file, strpath.c_str(), "rb");
	if (!file) {
		fclose(file);
		return FALSE;
	}

	fread(&m_file_header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&m_info_header, sizeof(BITMAPINFOHEADER), 1, file);
	m_width = m_info_header.biWidth;
	m_height = m_info_header.biHeight;
	m_pixel_vector.clear();
	m_pixel_vector.resize(m_width * m_height);
	fread(&m_pixel_vector[0], sizeof(COLOR), m_pixel_vector.size(), file);

	COLOR *pixel_array = new COLOR[m_width];
	for (size_t i = 0; i < m_height / 2; i++) {
		memcpy(pixel_array, &m_pixel_vector[i * m_width], sizeof(COLOR) * m_width);
		memcpy(&m_pixel_vector[i * m_width], &m_pixel_vector[(m_height - 1 - i) * m_width], sizeof(COLOR) * m_width);
		memcpy(&m_pixel_vector[(m_height - 1 - i) * m_width], pixel_array, sizeof(COLOR) * m_width);
	}
	SAFE_ARRAY_DELETE(pixel_array);
	// PIXEL의 사이즈가 4바이트일 경우에는 설정->C++->구조체->코드 생성->구조체 멤버 맞춤 의 설정을 조정해야 한다.
	// 비트맵은 상하가 반전되어 저장되기 때문에 출력 시 반드시 상하 반전을 다시 한 번 해 주어야 한다.

	fclose(file);

	return TRUE;
}

bool Pixel_Collider::Collision(Collider * const _collider) {
	switch (_collider->Get_Type()) {
	case CT_NONE:
		return FALSE;
	case CT_RECT:
		return Rect_Collision(((Rect_Collider*)_collider)->Get_Rect());
	case CT_CIRCLE:
		return Circle_Collision(((Circle_Collider*)_collider)->Get_Circle());
	case CT_LINE:
		return FALSE;
	case CT_POINT:
		return Point_Collision(((Point_Collider*)_collider)->Get_Pos());
	case CT_PIXEL:
		return FALSE;
	case CT_END:
		return FALSE;
	}
}