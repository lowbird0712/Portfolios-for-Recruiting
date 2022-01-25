#pragma once

#include "Circle_Button.h"
#include "../../../Singleton/Collision_Manager/Circle_Collider.h"

Circle_Button::Circle_Button() { }
Circle_Button::Circle_Button(const Circle_Button & _button) : Button(_button) { }
Circle_Button::~Circle_Button() { }

bool Circle_Button::Init() {
	if (!Button::Init())
		return FALSE;

	Circle_Collider *collider = Add_Collider<Circle_Collider>(CIRCLE_BUTTON_CLICKRANGE_KEY);
	if (!collider) {
		SAFE_RELEASE(collider);
		return FALSE;
	}
	collider->Add_Function(CS_ENTER, (Button*)this, &Button::Mouse_Enter);
	collider->Add_Function(CS_LEAVE, (Button*)this, &Button::Mouse_Leave);

	SAFE_RELEASE(collider);

	return TRUE;
}