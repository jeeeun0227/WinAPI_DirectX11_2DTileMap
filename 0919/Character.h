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

enum eStateType
{
	ET_MOVE,
	ET_IDLE,
};

class Sprite;
class State;

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
	bool _isMoving;

	State *_state;
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
	void ChangeState(eStateType stateType);

	void InitMove();

	void MoveStart(int newTileX, int newTileY);

	void SetPosition(float PosX, float PosY);

	void RaceiveMessage(const sComponentMsgParam msgParam);

	virtual void Collision(std::list<Component*> &collisonList);

	eDirection GetDirection() { return _currentDirection; }

	float GetMoveTime() { return _moveTime; }

	void MoveStop();

	void Moving(float deltaTime);

	bool IsMoving() { return _isMoving; }
};