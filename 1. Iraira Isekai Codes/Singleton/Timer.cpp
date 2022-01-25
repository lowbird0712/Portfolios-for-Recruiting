#include "Timer.h"

DEFINITION_SINGLE(Timer);

Timer::Timer() : m_hWnd(NULL), m_frequency(LARGE_INTEGER()), m_oldtimer(LARGE_INTEGER()), m_deltatime(0.f), m_timescale(1.f),
m_FPS(0.f), m_passed_FPStime(0.f), m_passed_frame(0) { }
Timer::~Timer() { }

bool Timer::Init(HWND _hWnd) {
	m_hWnd = _hWnd;

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_oldtimer);

	return TRUE;
}

void Timer::Update() {
	LARGE_INTEGER timer;
	QueryPerformanceCounter(&timer);

	m_deltatime = (timer.QuadPart - m_oldtimer.QuadPart) / (float)m_frequency.QuadPart;
	m_oldtimer = timer;

	m_passed_FPStime += m_deltatime;
	m_passed_frame++;
	if (m_passed_FPStime >= 1.f) {
		m_FPS = m_passed_frame / m_passed_FPStime;
		m_passed_FPStime = 0.f;
		m_passed_frame = 0;

#ifdef _DEBUG
		char FPS_string[64] = {};
		sprintf_s(FPS_string, "FPS : %.f\n", m_FPS);
		SetWindowTextA(m_hWnd, FPS_string); // 윈도우 캡션의 문자열을 설정해주는 함수이다.
		//OutputDebugStringA(FPS_string); // 디버깅 창에 문자열을 출력해주는 함수이다.
		//_cprintf(FPS_string); // 콘솔 창에 출력해주는 함수이다.
#endif // _DEBUG
	}
}