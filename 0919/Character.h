#pragma once

#include <Windows.h>
#include <D3DX9.h>
#include <vector>
#include <list>

#include "Component.h"

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class MoveState;
class Sprite;

class Character : public Component
{
public:
	Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~Character();

	void Init();
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Relese();
	void Reset();

	// transform
public:
	void MoveDeltaPosition(float deltaX, float deltaY);

	// AI
public:
	virtual void UpdateAI();

protected:

	float _moveTime;

	MoveState *_state;

	eDirection _currentDirection;

	float _targetX;
	float _targetY;

	float _x;
	float _y;

	std::vector<Sprite*> _spriteList;

	std::wstring _pngName;
	std::wstring _scriptFileName;

	// attack
	int _attackPoint;
	int _hp;

public:
	void InitMove();
	void UpdateMove(float deltaTime);
	void MoveStart(int newTileX, int newTileY);

	void SetPosition(float PosX, float PosY);

	void RaceiveMessage(const sComponentMsgParam msgParam);

	virtual void Collision(std::list<Component*> &collisonList);

	eDirection GetDirection() { return _currentDirection; }
};