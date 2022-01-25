#pragma once

// C++ ���
#include <cmath>
#include <typeinfo>						// typeid �Լ��� ����ϱ� ���� �ʿ��ϴ�.
#include <ctime>
#include <vector>
#include <list>
#include <unordered_map>
#include <functional>					// functional�� ����ϱ� ���� �ʿ��ϴ�.
#include <conio.h>
#include <tchar.h>
#include <string>
// C++ ���̺귯��
#pragma comment(lib, "msimg32")			// LoadImage���� �ܺ����� ������ �߻����� �ʰ� �ϱ� ���� �ʿ��ϴ�

// Fmod ���
#include "Fmod/fmod.hpp"
// Fmod ���̺귯��
#pragma comment(lib, "fmod_vc.lib")

// ����� ���� ���
#include "resource.h"
#include "Macro.h"
#include "Types.h"

// using ����
using namespace std;

// ����� ���� �Լ�
bool		Is_Integer(const float _float);
float		Calc_Distance(const FLOATPOINT &_pos1, const FLOATPOINT &_pos2);
float		Calc_Angle(const FLOATPOINT &_center, const FLOATPOINT &_pos);
POSITION	Get_Numbered_Point(const FLOATRECT &_rect, const int _num);
bool		RectRect_Collision(const FLOATRECT &_rect1, const FLOATRECT &_rect2);
bool		RectCircle_Collision(const FLOATRECT &_rect, const FLOATCIRCLE &_circle);
bool		CircleCircle_Collision(const FLOATCIRCLE &_circle1, const FLOATCIRCLE &_circle2);
bool		PointRect_Collision(const POSITION &_pos, const FLOATRECT &_rect);
bool		PointCircle_Collision(const POSITION &_pos, const FLOATCIRCLE &_circle);