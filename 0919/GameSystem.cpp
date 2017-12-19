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
#include "RecoveryItem.h"
#include "Stage.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int mouseX;
	int mouseY;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		GameSystem::GetInstance()->MouseDown(mouseX, mouseY);
		return 0;

	case WM_LBUTTONUP:
		GameSystem::GetInstance()->MouseUp();
		return 0;

	case WM_KEYDOWN:
		GameSystem::GetInstance()->KeyDown(wParam);

		if (VK_ESCAPE == wParam)
		{
			ComponentSystem::GetInstance()->RemoveAllComponents();
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_KEYUP:
		GameSystem::GetInstance()->KeyUp(wParam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		/*
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World!", L"Hello", MB_OK);
		return 0;
		*/
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameSystem *GameSystem::_instance = NULL;

GameSystem *GameSystem::GetInstance()
{
	if (_instance == NULL)
		_instance = new GameSystem();
	return _instance;
}

GameSystem::GameSystem()
{
	_isFULLScreen = false;

	_isMouseDown = false;
}

GameSystem::~GameSystem()
{
	delete _stage;

	RELEASE_COM(_sprite);
	RELEASE_COM(_device3d);

	delete _testFont;
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
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Base";

	if (!RegisterClass(&wc))
	{
		return 0;
	}

	DWORD style;

	if (_isFULLScreen)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	_hWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 800, 0, 0, hInstance, 0);
	// 1280 : 출력 화면의 가로
	// 700 ~ 800 : 출력 화면의 세로

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

	ShowWindow(_hWnd, nCmdShow);

	UpdateWindow(_hWnd);

	if (false == InitDirect3D())
	{
		return false;
	}

	// Stage Map1 생성
	_stage = new Stage();
	_stage->Init(L"Map1");

	return true;
}

int GameSystem::Update()
{
	int ret = 1;
	MSG msg = { 0 };

	GameTimer _gameTime;
	_gameTime.Reset();

	float SecondPerFrame = 1.0f / 60;		// 1초당 60프레임
	float _frameDuration = 0.0f;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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

			_stage->Update(_deltaTime);

			if (SecondPerFrame <= _frameDuration)
			{
				_frameDuration = 0.0f;
			}

			_device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(20, 40, 70), 0.0f, 0);		// 화면 색상 변경

			_device3d->BeginScene();

			// Sprite를 화면에 그린다.
			_sprite->Begin(D3DXSPRITE_ALPHABLEND);

			_stage->Render();

			_sprite->End();

			_device3d->EndScene();

			CheckDeviceLost();

			_device3d->Present(NULL, NULL, NULL, NULL);
		}

		// Stage 교체 테스트
		{
			// F1키를 누르면 일반 몬스터 죽이기 RPG 게임이 실행된다.
			if (IsKeyDown(VK_F1))
			{
				ComponentSystem::GetInstance()->ClearMessageQueue();
				delete _stage;
				_stage = new Stage();
				_stage->Init(L"Map2");
			}

			// F2키를 누르면 인생 게임이 실행된다.
			if (IsKeyDown(VK_F2))
			{
				ComponentSystem::GetInstance()->ClearMessageQueue();
				delete _stage;
				_stage = new Stage();
				_stage->Init(L"Map3");
			}

			// F3키를 누르면 길찾기 게임이 실행된다.
			if (IsKeyDown(VK_F3))
			{
				ComponentSystem::GetInstance()->ClearMessageQueue();
				delete _stage;
				_stage = new Stage();
				_stage->Init(L"Map4");
			}
		}
	}
	return (int)msg.wParam;
}

void GameSystem::CheckDeviceLost()
{
	HRESULT hr = _device3d->TestCooperativeLevel();

	if (FAILED(hr))
	{
		if (D3DERR_DEVICELOST == hr)
		{
			Sleep(100);
			return;
		}

		else if (D3DERR_DEVICENOTRESET == hr)
		{
			_stage->Relese();

			hr = _device3d->Reset(&_d3dpp);
			InitDirect3D();

			_stage->Reset();
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

void GameSystem::MouseDown(int mouseX, int mouseY)
{
	_isMouseDown = true;

	_mouseX = mouseX;
	_mouseY = mouseY;
}