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
public:
	enum eUpdateState
	{
		PATHFINDING,
		BUILD_PATH,
	};

private:
	std::queue<TileCell*> _pathFindingTileQueue;
	TileCell *_targetTileCell;
	TileCell *_reverseTileCell;
	eUpdateState _updateState;

	void UpdatePathFinding();
	void UpdateBuildPath();
};