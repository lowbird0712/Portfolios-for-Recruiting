#pragma once

#include "../Game.h"
#include "Scene_Manager/Scene_Manager.h"

#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT		563
#define FRAME_PER_SEC		120

#define EXIT_PROGRAM_KEY	"Exit_Program"
#define MY_SCREENSHOT_KEY	"My_Screenshot_Key"

#define GET_HINST()			(GET_SINGLE(Core)->Get_hInst())
#define GET_HWND()			(GET_SINGLE(Core)->Get_HWND())
#define GET_HDC()			(GET_SINGLE(Core)->Get_HDC())
#define GET_RESOLUTION()	(GET_SINGLE(Core)->Get_Resolution())

typedef struct RESOLUTION {
	LONG width;
	LONG height;

	RESOLUTION() : width(0), height(0) { }
	RESOLUTION(const LONG _width, const LONG _height) : width(_width), height(_height) { }

	void operator=(const FLOATSIZE &_size);
} RESOLUTION;

class Core {
	DECLARE_SINGLE(Core)
private:
	// Variables
	HINSTANCE				m_hInst;
	HWND					m_hWnd;
	HDC						m_hDC;

	static bool				m_loop; // WndProc이 전역 함수여야 하기 때문에 이 함수가 다루는 m_loop는 멤버 변수일 수 없다.
	RESOLUTION				m_resolution;
	float					m_frametimer;

	// Functions
	static LRESULT CALLBACK	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	ATOM					MyRegisterClass();
	BOOL					Create();

	void					Logic();
	SCENE_CHANGING			Input(const float _deltatime);
	SCENE_CHANGING			Update(const float _deltatime);
	void					Collision(const float _deltatime);
	SCENE_CHANGING			Late_Update();
	void					Render();
public:
	// Functions
	int						Run();

	HINSTANCE				Get_hInst() const		{ return m_hInst; }
	HWND					Get_HWND() const		{ return m_hWnd; }
	HDC						Get_HDC() const			{ return m_hDC; }
	RESOLUTION				Get_Resolution() const	{ return m_resolution; }

	bool					Init(HINSTANCE _hInst);
};