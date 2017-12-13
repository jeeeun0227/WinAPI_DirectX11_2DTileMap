#pragma once

#include <stack>

#include "State.h"
#include "GlobalType.h"

class Character;

class PathFindingMoveState : public State
{
public:
	PathFindingMoveState();
	~PathFindingMoveState();

private:
	float _movingDuration;
	std::stack<TileCell*> _pathTileCellStack;

public:
	void Init(Character *character);
	void Update(float deltaTime);

	void Stop();
	void Start();
};