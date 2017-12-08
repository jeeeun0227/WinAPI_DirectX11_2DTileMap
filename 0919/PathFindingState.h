#pragma once

#include "State.h"
#include "TileCell.h"

#include <queue>

class TileCell;

class PathFindingState : public State
{
public:
	PathFindingState();
	~PathFindingState();

public:
	void Init(Character *character);
	void Update(float deltaTime);

	void Start();
	void Stop();

	// PathFinding
private:
	std::queue<TileCell*> _pathFindingTileQueue;
	TileCell *_targetTileCell;
};