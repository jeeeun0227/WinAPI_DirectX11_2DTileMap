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
class Map;

class Character : public Component
{
private:
	std::wstring  _pngName;
	std::wstring  _scriptFileName;

	float _x;
	float _y;

protected:
	int _hp;

public:
	Character(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	virtual ~Character();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Relese();
	void Reset();

	void Init(int tileX, int tileY);

	std::wstring  GetTextureFileName() { return _pngName; }
	std::wstring  GetScriptFileName() { return _scriptFileName; }

	// Message
public:
	void RaceiveMessage(const sComponentMsgParam msgParam);

	// Common
public:
	void DecreaseHP(int decreaseHP);
	void IncreaseHP(int increaseHP);

	// transform
public:
	void SetPosition(float PosX, float PosY);
	void MoveDeltaPosition(float deltaX, float deltaY);

	// AI
public:
	virtual void UpdateAI();

	// state
private:
	std::map<eStateType, State*> _stateMap;

public:
	void InitState();
	void ReplaceState(eStateType changeType, State* replaceState);
	void ChangeState(eStateType stateType);

	// Move
protected:
	bool _isMoving;
	float _moveTime;

	State *_state;
	eDirection _currentDirection;

	float _targetX;
	float _targetY;

	// Moving
public:
	void InitMove();

	void MoveStart(int newTileX, int newTileY);
	void MoveStop();
	void Moving(float deltaTime);

	bool IsMoving() { return _isMoving; }
	float GetMoveTime() { return _moveTime; }

	float GetX() { return _x; }
	float GetY() { return _y; }

	eDirection GetDirection() { return _currentDirection; }
	virtual Component *Collision(std::list<Component*> &collisonList);

	void InitTilePosition(int tileX, int tileY);

	// Attack
protected:
	Component *_target;
	int _attackPoint;

	float _attackCoolTime;
	float _attackCoolTimeDuration;

	int _criticalAttackPoint;

public:
	int GetAttackPoint() { return _attackPoint; }
	Component *GetTarget() { return _target; }
	void SetTarget(Component *target) { _target = target; }
	void ResetTarget() { _target = NULL; }

	void UpdateAttackCoolTime(float deltaTime);
	bool IsAttackCoolTime();
	void ResetAttackCoolTime();

	int GetCriticalAttack();
	int ReSetAttackPoint();
	void SetAttackPoint(int FinalAttackPoint) { _attackPoint = FinalAttackPoint; };

	// Defense
protected:
	int _attackedPoint;

public:
	int GetAttackedPoint() { return _attackedPoint; }

	// Font
protected:
	Font *_font;
public:
	virtual void UpdateText();
};