#pragma once

#include "Select_Panal.h"
#include "Button/Button.h"

Select_Panal::Select_Panal() { }
Select_Panal::Select_Panal(const Select_Panal & _panal) : Panal(_panal) { }
Select_Panal::~Select_Panal() {
	for (size_t i = 0; i < m_button_vector.size(); i++)
		SAFE_RELEASE(m_button_vector[i]);
}