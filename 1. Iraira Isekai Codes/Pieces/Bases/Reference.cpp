#include "Reference.h"

Reference::Reference() : m_ref(1), m_freeze(FALSE), m_visible(TRUE), m_enable(TRUE), m_live(TRUE) { }
Reference::~Reference() { }

int Reference::Release() {
	m_ref--;
	if (m_ref == 0) {
		delete this;
		return 0;
	}

	return m_ref;
}