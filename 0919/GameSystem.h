#pragma once

#include <Windows.h>
#include <D3DX9.h>		
#include <d3d9.h>
#include <list>

#define RELEASE_COM(x) {if(x){x->Release(); x=NULL;}}

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

class Sprite;
class Map;
class Character;
class ComponentSystem;
class Component;
class ComponentMessage;
class Font;
class Stage;

class GameSystem
{
	// Singleton -> 한번만 생성되도록 하는 기능을 보장
private:
	static GameSystem *_instance;

public:
	static GameSystem *GetInstance();

private:
	GameSystem();

public:
	~GameSystem();

public:
	bool InitDirect3D();
	bool InitSystem(HINSTANCE hInstance, int nCmdShow);
	int Update();

	void CheckDeviceLost();

	int GetClientWidth();
	int GetClientHeight();

	LPD3DXSPRITE GetSprite();
	LPDIRECT3DDEVICE9 GetDevice();

	// Input
public:
	enum eKeyState
	{
		KEY_DOWN,
		KEY_UP
	};

private:
	eKeyState _keyState[256];

	bool _isMouseDown;
	int _mouseX;
	int _mouseY;

public:
	void InitInput();
	void KeyDown(unsigned int KeyCode);
	void KeyUp(unsigned int KeyCode);
	bool IsKeyDown(unsigned int KeyCode);

	void MouseDown(int mouseX, int mouseY);
	void MouseUp() { _isMouseDown = false; }
	bool IsMouseDown() { return _isMouseDown; }
	int GetMouseX() { return _mouseX; }
	int GetMouseY() { return _mouseY; }

	// Direct3D
private:
	UINT _4xMsaaQuality;

	HWND _hWnd;

	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;

	bool _isFULLScreen;

	D3DPRESENT_PARAMETERS _d3dpp;

	// Font Test
private:
	Font *_testFont;

	// Load Map
private:
	Stage *_stage;
public:
	Stage *GetStage() { return _stage; }
};