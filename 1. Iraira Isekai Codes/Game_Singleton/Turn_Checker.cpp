#include "Turn_Checker.h"

DEFINITION_SINGLE(Turn_Checker);

Turn_Checker::Turn_Checker() : m_turn_timelength(0.f), m_passed_time(0.f), m_working(FALSE), m_ringing(FALSE) { }
Turn_Checker::~Turn_Checker() { }

void Turn_Checker::Reset() {
	m_passed_time = 0.f;
	m_working = FALSE;
	m_ringing = FALSE;
}

bool Turn_Checker::Init(const float _turn_timelength) {
	m_turn_timelength = _turn_timelength;

	return TRUE;
}

void Turn_Checker::Update(const float _deltatime) {
	if (m_working) {
		m_passed_time += _deltatime;

		if (m_passed_time >= m_turn_timelength) {
			m_passed_time = 0.f;
			m_working = FALSE;
			m_ringing = TRUE;
		}
	}
}
