﻿#include <stdio.h>
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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

	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World!", L"Hello", MB_OK);
		return 0;
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

	_componentList.clear();
}

GameSystem::~GameSystem()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}

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

	_hWnd = CreateWindow(L"Base", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

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
	for (int i = 0; i < 1; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
		_componentList.push_back(_npc);
	}

	// monster 생성
	for (int i = 0; i < 1; i++)
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

			// Sprite를 화면에 그린다.
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