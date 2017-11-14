#include <stdio.h>
#include <string>
#include <vector>
#include <list>

#include "GameSystem.h"
#include "GameTimer.h"
#include "Sprite.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "ComponentSystem.h"
#include "Component.h"
#include "Monster.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// 윈도우 프로시저를 실행시키기 위해 만든다. 
																				// 인자 4개 -> ( 윈도우 핸들, 메세지타입, 메세지당 부가별 정보, 메세지당 부가별 정보 )
																				// 윈도우 핸들에는 메인 윈도우 핸들이 넘어온다.
{
	switch (msg)		// ESC를 누르면 윈도우 창이 종료된다. + 마우스 클릭 시에 Hello World! 출력
	{
	case WM_KEYDOWN:
		GameSystem::GetInstance()->KeyDown(wParam);

		if (VK_ESCAPE == wParam)	// VK_ESCAPE = ESC
		{
			ComponentSystem::GetInstance()->RemoveAllComponents();
			DestroyWindow(hWnd);		// 현재창을 파괴
		}
		return 0;

	case WM_KEYUP:
		GameSystem::GetInstance()->KeyUp(wParam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:		// 마우스 클릭 시에 Hello World! 출력
		MessageBox(0, L"Hello World!", L"Hello", MB_OK);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 정적변수 vs 전역변수 : 메모리를 하나만 사용하는 것은 같으나, 접근이 가능한 범위가 다르다. 
// 전역변수 -> 어디에서나 접근 가능, 정적변수 & 지역변수 -> 해당 함수에 들어왔을때만 변수를 사용 가능하다.

// 헤더에서 선언한 정적 멤버들을 다시 선언해줘야만, 일반 멤버 변수처럼 사용할 수 있다.
GameSystem *GameSystem::_instance = NULL;		// 정적 변수 -> 메모리를 하나만 잡는다.

GameSystem *GameSystem::GetInstance()		// 정적 함수 -> 메모리를 하나만 잡는다.
{
	if (_instance == NULL)
		_instance = new GameSystem();
	return _instance;
}

GameSystem::GameSystem()		// 생성자
{
	_isFULLScreen = false;

	_componentList.clear();
}

GameSystem::~GameSystem()		// 해지자
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}

	RELEASE_COM(_sprite);
	RELEASE_COM(_device3d);
}

bool GameSystem::InitDirect3D()
{
	LPDIRECT3D9 direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (NULL == direct3d)
	{
		MessageBox(0, L"LPDIRECT3D9 에러입니다.", L"ErrorMessage", 0);
		return false;
	}

	ZeroMemory(&_d3dpp, sizeof(_d3dpp));
	_d3dpp.BackBufferWidth = 1280;
	_d3dpp.BackBufferHeight = 800;

	if (_isFULLScreen)
	{
		_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	}

	_d3dpp.BackBufferCount = 1;
	_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dpp.hDeviceWindow = _hWnd;
	_d3dpp.Windowed = (!_isFULLScreen);
	_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	HRESULT hr = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3DCAPS9 에러입니다.", L"ErrorMessage", 0);
		return false;
	}

	DWORD behavior;
	if (FAILED(hr))
		return false;
	if ((caps.DeviceType & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || (caps.VertexShaderVersion < D3DVS_VERSION(1, 1)))
	{
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, behavior, &_d3dpp, &_device3d);

	hr = D3DXCreateSprite(_device3d, &_sprite);

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateDevice 에러입니다.", L"ErrorMessage", 0);
		return false;
	}

	hr = D3DXCreateSprite(_device3d, &_sprite);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3DXCreateSprite 에러입니다.", L"ErrorMessage", 0);
		return false;
	}
	return true;
}

bool GameSystem::InitSystem(HINSTANCE hInstance, int nCmdShow)
{
	// 1. 윈도우 창의 특징을 기술
	WNDCLASS wc;		// 구조체 처럼 쓰이는 것 ( 구조체는 public )
	wc.style = CS_HREDRAW | CS_VREDRAW;		// window 창의 스타일을 결정한다.
	wc.lpfnWndProc = WndProc;		// 윈도우 프로시저 ( WndProc ) : 윈도우를 통해 메세지를 받아서 각자의 방식대로 메세지를 처리하는 것 / 윈도우 프로시저 세팅
	wc.cbClsExtra = 0;		// 잘 사용되지 않는다.
	wc.cbWndExtra = 0;		// 잘 사용되지 않는다.
	wc.hInstance = hInstance; // 윈도우( wc.hInstance )와 현재 어플리케이션의 핸들( hInstance )을 연결 / 윈도우는 핸들을 통해 관리한다.
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);		// 화살표 세팅
	wc.hCursor = LoadCursor(0, IDC_ARROW);		// 커서 세팅
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 배경을 흰색으로 세팅
	wc.lpszMenuName = 0;		// 메뉴 세팅 하지 않음
	wc.lpszClassName = L"Base";		// 이 윈도우의 이름 / 이 윈도우를 사용하고 싶다면 이 이름을 통해 사용할 수 있도록 한다.

	// 2. 윈도우에다가 등록 -> 나중에 쓸 수 있도록 준비한다.
	if (!RegisterClass(&wc))
	{
		return 0;
	}

	// 3. 윈도우 생성 ( * 윈도우 등록 != 생성 )
	DWORD style;

	if (_isFULLScreen)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	_hWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	// CreateWindow( 윈도우 이름, 윈도우 제목, 윈도우 스타일, x, y, 너비, 높이, 부모 윈도우, 윈도우 메뉴, 윈도우 프로시저, 윈도우 플래그 );

	if (false == _isFULLScreen)
	{
		RECT clientRect;
		GetClientRect(_hWnd, &clientRect);

		int addWidth = 1280 - clientRect.right;
		int addHeight = 800 - clientRect.bottom;
		int finalWidth = 1280 + addWidth;
		int finalHeight = 800 + addHeight;

		MoveWindow(_hWnd, 0, 0, finalWidth, finalHeight, TRUE);
	}

	// 4. 윈도우 출력
	ShowWindow(_hWnd, nCmdShow);

	// 5. 윈도우 업데이트 -> 메세지를 받을 준비가 되었다.
	UpdateWindow(_hWnd);

	if (false == InitDirect3D())
	{
		return false;
	}

	_componentList.clear();

	// map 생성
	Map *_map = new Map(L"tileMap");
	_componentList.push_back(_map);

	// player 생성
	Player *_player = new Player(L"player", L"npc", L"Player_Sprite_00");
	// 생성자 (컴포넌트 이름(고유 이름), json 파일 이름.json, 이미지 이름.png);
	_componentList.push_back(_player);

	// npc 생성
	NPC *_npc;
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
		_componentList.push_back(_npc);
	}

	// monster 생성
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		Monster *_monster = new Monster(name, L"monster", L"monster");
		_componentList.push_back(_monster);
	}

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Init();
	}

	_map->InitViewer(_player);

	return true;
}

int GameSystem::Update()
{
	int ret = 1;
	MSG msg = { 0 };		// 초기화

	GameTimer _gameTime;
	_gameTime.Reset();

	float SecondPerFrame = 1.0f / 60;		// 1초당 60프레임
	float _frameDuration = 0.0f;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))		// 윈도우 처리 / PM_REMOVE -> 메세지에 들어간 이벤트를 사용한 후 지운다.
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// todo : 게임 처리
			_gameTime.Update();

			float _deltaTime = _gameTime.GetDeltaTime();
			_frameDuration += _deltaTime;

			ComponentSystem::GetInstance()->Update(_deltaTime);

			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
			{
				(*it)->Update(_deltaTime);
			}

			if (SecondPerFrame <= _frameDuration)
			{
				wchar_t timeCheck[256];
				swprintf(timeCheck, L"delta time : %f\n", _deltaTime);
				OutputDebugString(timeCheck);

				_frameDuration = 0.0f;
			}

			_device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(20, 40, 70), 0.0f, 0);		// 화면 색상 변경

			_device3d->BeginScene();

			_sprite->Begin(D3DXSPRITE_ALPHABLEND);

			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
			{
				(*it)->Render();
			}

			_sprite->End();

			_device3d->EndScene();

			CheckDeviceLost();

			_device3d->Present(NULL, NULL, NULL, NULL);

		}
	}
	return (int)msg.wParam;		// 윈도우가 정상 종료를 하기 위해서는 WM_QUIT 함수를 불러들어야 한다.
}

void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();
	// D3DERR_DEVICELOST :  디바이스 로스트 상태가 되어서 현재 복구할 수 없다.
	// D3DERR_DEVICENOTRESET : 디바이스를 다시 작동할 수 있다.
	// D3DERR_DRIVERINTERNALERROR : 디바이스 내부 에러가 있다. 할 수 있는 방법은 없다.

	if (FAILED(hr))		// 유효한 상태가 아니라면
	{
		if (D3DERR_DEVICELOST == hr)
		{
			Sleep(100);
			return;
		}

		else if (D3DERR_DEVICENOTRESET == hr)		// 복구
		{
			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
			{
				(*it)->Relese();
			}

			hr = _device3d->Reset(&_d3dpp);
			InitDirect3D();

			for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
			{
				(*it)->Reset();
			}
		}
	}
}

int GameSystem::GetClientWidth()
{
	return 1280;
}

int GameSystem::GetClientHeight()
{
	return 800;
}

LPD3DXSPRITE GameSystem::GetSprite()
{
	return _sprite;
}

LPDIRECT3DDEVICE9 GameSystem::GetDevice()
{
	return _device3d;
}

void GameSystem::InitInput()
{
	for (int i = 0; i < 256; i++)
	{
		_keyState[i] = eKeyState::KEY_UP;
	}
}

void GameSystem::KeyDown(unsigned int KeyCode)
{
	_keyState[KeyCode] = eKeyState::KEY_DOWN;
}

void GameSystem::KeyUp(unsigned int KeyCode)
{
	_keyState[KeyCode] = eKeyState::KEY_UP;
}

bool GameSystem::IsKeyDown(unsigned int KeyCode)
{
	return (eKeyState::KEY_DOWN == _keyState[KeyCode]);
}