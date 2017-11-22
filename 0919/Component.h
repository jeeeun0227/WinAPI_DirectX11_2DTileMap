#pragma once

#include <windows.h>
#include <string>

class ComponentSystem;
struct sComponentMsgParam;

enum eComponentType
{
	CT_PLAYER,
	CT_MONSTER,
	CT_NPC,
	CT_TILE_OBJECT,
	CT_ITEM,
	CT_NONE,
};

class Component
{
public:
	Component()
	{
		_canMove = false;
	}

	Component(LPCWSTR name);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Deinit() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Relese() = 0;
	virtual void Reset() = 0;

	virtual void MoveDeltaPosition(float _deltaX, float _deltaY) {}
	virtual void SetPosition(float PosX, float PosY) {}

	void SetCanMove(bool canMove) { _canMove = canMove; }
	bool CanMove() { return _canMove; }

	int GetTileX() { return _tileX; }
	int GetTileY() { return _tileY; }

public:
	float GetMoveDeltaX() { return _moveDistancePerTimeX; }
	float GetMoveDeltaY() { return _moveDistancePerTimeY; }

	bool isLive() { return _isLive; }

protected:
	LPCWSTR _name;
	bool _canMove;

	int _tileX;
	int _tileY;

	float _moveDistancePerTimeX;
	float _moveDistancePerTimeY;

	bool _isLive;

	// message
public:
	virtual void RaceiveMessage(const sComponentMsgParam msgParam);

	// GetType
public:
	eComponentType GetType() { return _type; }

protected:
	eComponentType _type;
};