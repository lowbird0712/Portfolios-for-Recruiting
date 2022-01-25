#include "Game.h"

bool Is_Integer(const float _float) {
	float num = _float;

	while (num >= 1)
		num -= 1;

	if (num == 0)
		return TRUE;
	else
		return FALSE;
}

float Calc_Distance(const FLOATPOINT & _pos1, const FLOATPOINT & _pos2) {
	const float x = _pos2.x - _pos1.x;
	const float y = _pos2.y - _pos1.y;

	return sqrt(x * x + y * y);
}

float Calc_Angle(const FLOATPOINT & _center, const FLOATPOINT & _pos) {
	const float distance = Calc_Distance(_center, _pos);
	const float cosine = (_pos.x - _center.x) / distance;

	float angle = acosf(cosine);
	if (_pos.y - _center.y > 0)
		angle = -angle;

	return angle;
}

POSITION Get_Numbered_Point(const FLOATRECT & _rect, const int _num) {
	switch (_num) {
	case 1:
		return POSITION(_rect.left, _rect.top);
	case 2:
		return POSITION((_rect.left + _rect.right) / 2, _rect.top);
	case 3:
		return POSITION(_rect.right, _rect.top);
	case 4:
		return POSITION(_rect.left, (_rect.top + _rect.bottom) / 2);
	case 5:
		return POSITION((_rect.left + _rect.right) / 2, (_rect.top + _rect.bottom) / 2);
	case 6:
		return POSITION(_rect.right, (_rect.top + _rect.bottom) / 2);
	case 7:
		return POSITION(_rect.left, _rect.bottom);
	case 8:
		return POSITION((_rect.left + _rect.right) / 2, _rect.bottom);
	case 9:
		return POSITION(_rect.right, _rect.bottom);
	default:
		exit(0);
	}
}

bool RectRect_Collision(const FLOATRECT & _rect1, const FLOATRECT & _rect2) {
	if (_rect1.left >= _rect2.right)
		return FALSE;
	else if (_rect1.right <= _rect2.left)
		return FALSE;
	else if (_rect1.top >= _rect2.bottom)
		return FALSE;
	else if (_rect1.bottom <= _rect2.top)
		return FALSE;
	else
		return TRUE;
}

bool RectCircle_Collision(const FLOATRECT & _rect, const FLOATCIRCLE & _circle) {
	float distance;

	distance = Calc_Distance(FLOATPOINT(_circle.x, _circle.y), POSITION(_rect.left, _rect.top));
	if (distance < _circle.radius)
		return TRUE;
	distance = Calc_Distance(FLOATPOINT(_circle.x, _circle.y), POSITION(_rect.right, _rect.top));
	if (distance < _circle.radius)
		return TRUE;
	distance = Calc_Distance(FLOATPOINT(_circle.x, _circle.y), POSITION(_rect.left, _rect.bottom));
	if (distance < _circle.radius)
		return TRUE;
	distance = Calc_Distance(FLOATPOINT(_circle.x, _circle.y), POSITION(_rect.right, _rect.bottom));
	if (distance < _circle.radius)
		return TRUE;

	if (_circle.x <= _rect.left - _circle.radius ||
		_rect.right + _circle.radius <= _circle.x)
		return FALSE;
	else
		return TRUE;
}

bool CircleCircle_Collision(const FLOATCIRCLE & _circle1, const FLOATCIRCLE & _circle2) {
	const float distance = Calc_Distance(FLOATPOINT(_circle1.x, _circle1.y), FLOATPOINT(_circle2.x, _circle2.y));

	if (distance < _circle1.radius + _circle2.radius)
		return TRUE;
	else
		return FALSE;
}

bool PointRect_Collision(const POSITION & _pos, const FLOATRECT & _rect) {
	if (_pos.x <= _rect.left || _rect.right <= _pos.x)
		return FALSE;
	else if (_pos.y <= _rect.top || _rect.bottom <= _pos.y)
		return FALSE;
	else
		return TRUE;
}

bool PointCircle_Collision(const POSITION & _pos, const FLOATCIRCLE & _circle) {
	const float distance = Calc_Distance(_pos, POSITION(_circle.x, _circle.y));

	if (distance < _circle.radius)
		return TRUE;
	else
		return FALSE;
}