#pragma once

#include "Core.h"
#include "Timer.h"
#include "Path_Manager.h"
#include "Input_Manager/Input_Manager.h"
#include "Resource_Manager/Resource_Manager.h"
#include "FMOD_Manager/FMOD_Manager.h"
#include "Camera.h"
#include "Resource_Manager/Texture.h"

DEFINITION_SINGLE(Core);
bool Core::m_loop = TRUE;

void RESOLUTION::operator=(const FLOATSIZE & _size) {
	width = _size.x;
	height = _size.y;
}

Core::Core() : m_hInst(NULL), m_hWnd(NULL), m_hDC(NULL),
m_resolution(RESOLUTION(WINDOW_WIDTH, WINDOW_HEIGHT)), m_frametimer(0.f) {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1971);
	// 1242 1242 1240 1236 - 1231
	// 두번째 줄 주석 처리한 후 메모리 릭 있는지 확인
	// 있으면 두번째 줄에 해당 블록 번호를 인수로 넣고 주석 해제 후 실행
	// 호출 스택에서 중단점 바로 전 단계로 이동하면 그부분에서 메모리 릭 발생
#endif // _DEBUG

#ifdef _DEBUG
	//AllocConsole();
// 콘솔 창을 생성시켜준다. 디버그 할 때 필요시 활성화 시켜주자
#endif // _DEBUG
}
Core::~Core() {
	ReleaseDC(m_hWnd, m_hDC);

	DESTROY_SINGLE(Scene_Manager);
	DESTROY_SINGLE(FMOD_Manager);
	DESTROY_SINGLE(Resource_Manager);
	DESTROY_SINGLE(Input_Manager);
	DESTROY_SINGLE(Path_Manager);
	DESTROY_SINGLE(Timer);

#ifdef _DEBUG
	FreeConsole();
	// 생성된 콘솔 창의 할당을 해제해준다
#endif // _DEBUG
}

LRESULT Core::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	switch (_message) {
	case WM_DESTROY:
		m_loop = FALSE;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

ATOM Core::MyRegisterClass() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("AR13API");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

BOOL Core::Create() {
	RECT window_rect = { 0, 0, m_resolution.width, m_resolution.height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
	const LONG window_width = window_rect.right - window_rect.left;
	const LONG window_height = window_rect.bottom - window_rect.top;

	m_hWnd = CreateWindow(TEXT("AR13API"), TEXT("Lowbird"), WS_OVERLAPPEDWINDOW,
		0, 0, window_width, window_height, nullptr, nullptr, m_hInst, nullptr);
	if (!m_hWnd)
		return FALSE;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	ShowCursor(FALSE);

	return TRUE;
}

void Core::Logic() {
	GET_SINGLE(Timer)->Update();
	const float deltatime = GET_SINGLE(Timer)->Get_Deltatime();

	if (Input(deltatime) == SC_CHANGING)
		return;
	if (Update(deltatime) == SC_CHANGING)
		return;
	Collision(deltatime);
	if (Late_Update() == SC_CHANGING)
		return;
	Render();

	float passed_time = (float)clock() / CLOCKS_PER_SEC;

	if (m_frametimer == 0.f)
		m_frametimer = passed_time;

	while (TRUE) {
		if (passed_time - m_frametimer >= 1.f / FRAME_PER_SEC) {
			m_frametimer = passed_time;
			break;
		}
		else
			passed_time = (float)clock() / CLOCKS_PER_SEC;
	}
}

SCENE_CHANGING Core::Input(const float _deltatime) {
	if (GET_KEYDOWN(EXIT_PROGRAM_KEY))
		DestroyWindow(m_hWnd);
	if (GET_KEYDOWN(MY_SCREENSHOT_KEY)) {
		system("Pause");
		DestroyWindow(m_hWnd);
	}

	SCENE_CHANGING changing = SC_STABLE;

	changing = GET_SINGLE(Scene_Manager)->Input(_deltatime);

	if (SINGLE_DECLARED(Camera))
		GET_SINGLE(Camera)->Input(_deltatime);

	return changing;
}

SCENE_CHANGING Core::Update(const float _deltatime) {
	SCENE_CHANGING changing = SC_STABLE;

	GET_SINGLE(Input_Manager)->Update();
	GET_SINGLE(FMOD_Manager)->Update();
	changing = GET_SINGLE(Scene_Manager)->Update(_deltatime);

	if (SINGLE_DECLARED(Camera))
		GET_SINGLE(Camera)->Input(_deltatime);

	return changing;
}

void Core::Collision(const float _deltatime) {
	GET_SINGLE(Scene_Manager)->Add();
}

SCENE_CHANGING Core::Late_Update() {
	SCENE_CHANGING changing = SC_STABLE;

	changing = GET_SINGLE(Scene_Manager)->Late_Update();

	return changing;
}

void Core::Render() {
	Texture *backbuffer = GET_SINGLE(Resource_Manager)->Get_BackBuffer();
	HDC memDC = backbuffer->Get_MemDC();

	Rectangle(memDC, 0, 0, m_resolution.width, m_resolution.height);
	GET_SINGLE(Scene_Manager)->Render(memDC);
	GET_SINGLE(Input_Manager)->Render(memDC);
	BitBlt(m_hDC, 0, 0, m_resolution.width, m_resolution.height, memDC, 0, 0, SRCCOPY);

	SAFE_RELEASE(backbuffer);
}

int Core::Run() {
	MSG msg;

	while (m_loop) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Logic();
		}
	}

	return (int)msg.wParam;
}

bool Core::Init(HINSTANCE _hInst) {
	m_hInst = _hInst;

	MyRegisterClass();
	Create();

	m_hDC = GetDC(m_hWnd);

	if (!GET_SINGLE(Timer)->Init(m_hWnd))
		return FALSE;
	if (!GET_SINGLE(Path_Manager)->Init())
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Init(m_hWnd))
		return FALSE;
	if (!GET_SINGLE(Resource_Manager)->Init(m_hInst, m_hDC))
		return FALSE;
	if (!GET_SINGLE(FMOD_Manager)->Init())
		return FALSE;
	if (!GET_SINGLE(Scene_Manager)->Init())
		return FALSE;

	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_ESCAPE, EXIT_PROGRAM_KEY))
		return FALSE;
	if (!GET_SINGLE(Input_Manager)->Add_Key(VK_SPACE, MY_SCREENSHOT_KEY))
		return FALSE;

	return TRUE;
}