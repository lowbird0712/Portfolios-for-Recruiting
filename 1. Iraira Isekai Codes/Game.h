#pragma once

// C++ 헤더
#include <cmath>
#include <typeinfo>						// typeid 함수를 사용하기 위해 필요하다.
#include <ctime>
#include <vector>
#include <list>
#include <unordered_map>
#include <functional>					// functional을 사용하기 위해 필요하다.
#include <conio.h>
#include <tchar.h>
#include <string>
// C++ 라이브러리
#pragma comment(lib, "msimg32")			// LoadImage에서 외부참조 오류가 발생하지 않게 하기 위해 필요하다

// Fmod 헤더
#include "Fmod/fmod.hpp"
// Fmod 라이브러리
#pragma comment(lib, "fmod_vc.lib")

// 사용자 정의 헤더
#include "resource.h"
#include "Macro.h"
#include "Types.h"

// using 구문
using namespace std;

// 사용자 정의 함수
bool		Is_Integer(const float _float);
float		Calc_Distance(const FLOATPOINT &_pos1, const FLOATPOINT &_pos2);
float		Calc_Angle(const FLOATPOINT &_center, const FLOATPOINT &_pos);
POSITION	Get_Numbered_Point(const FLOATRECT &_rect, const int _num);
bool		RectRect_Collision(const FLOATRECT &_rect1, const FLOATRECT &_rect2);
bool		RectCircle_Collision(const FLOATRECT &_rect, const FLOATCIRCLE &_circle);
bool		CircleCircle_Collision(const FLOATCIRCLE &_circle1, const FLOATCIRCLE &_circle2);
bool		PointRect_Collision(const POSITION &_pos, const FLOATRECT &_rect);
bool		PointCircle_Collision(const POSITION &_pos, const FLOATCIRCLE &_circle);