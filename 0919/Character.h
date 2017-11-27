#pragma once

#include <Windows.h>
#include <D3DX9.h>
#include <vector>
#include <list>
#include <map>

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
	ET_ATTACK,
	EF_DEFENSE,
	ET_DEAD,
	ET_NONE,
};

class Sprite;
class State;
class AttackState;
class Font;

class Character : public Component
{
public:
	Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	virtual ~Character();

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

	std::wstring _pngName;
	std::wstring _scriptFileName;

	// state
private:
	std::map<eStateType, State*> _stateMap;

public:
	void ChangeState(eStateType stateType);

public:
	void InitMove();

	void MoveStart(int newTileX, int newTileY);

	void SetPosition(float PosX, float PosY);

	void RaceiveMessage(const sComponentMsgParam msgParam);

	virtual Component *Collision(std::list<Component*> &collisonList);

	eDirection GetDirection() { return _currentDirection; }

	float GetMoveTime() { return _moveTime; }

	void MoveStop();

	void Moving(float deltaTime);

	bool IsMoving() { return _isMoving; }

	std::wstring GetTextureFileName() { return _pngName; }
	std::wstring GetScriptFileName() { return _scriptFileName; }

	float GetX() { return _x; }
	float GetY() { return _y; }

	// attack
protected:
	int _hp;
	Component *_target;
	int _attackPoint;
	int _attackedPoint;
	float _attackCoolTime;
	float _attackCoolTimeDuration;
	int _criticalAttackPoint;
public:
	int GetAttackPoint() {	return _attackPoint;  }
	Component *GetTarget() { return _target; }
	void ResetTarget() {_target = NULL; }
	void SetTarget(Component *target) { _target = target;}
	int GetAttackedPoint() { return _attackedPoint; }
	void UpdateAttackCoolTime(float deltaTime);
	bool IsAttackCoolTime();
	void ResetAttackCoolTime();
	int GetCriticalAttack();
	int ReSetAttackPoint();

	void DecreaseHP(int decreaseHP);
	void IncreaseHP(int increaseHP);

	void SetAttackPoint(int FinalAttackPoint) { _attackPoint = FinalAttackPoint; };

	// Font
protected:
	Font *_font;
public:
	virtual void UpdateText();
};