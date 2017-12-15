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

	struct compare
	{
		bool operator()(TileCell* a, TileCell* b)
		{
			// return a->GetDistanceFromStart() > b->GetDistanceFromStart();
			return a->GetHeuristic() > b->GetHeuristic();
		}
	};

private:
	// std::queue<TileCell*> _pathFindingTileQueue;
	std::priority_queue<TileCell*, std::vector<TileCell*>, compare> _pathFindingTileQueue;		// 우선순위 큐
	TileCell *_targetTileCell;
	TileCell *_reverseTileCell;
	eUpdateState _updateState;

	void UpdatePathFinding();
	void UpdateBuildPath();

	float CalcSimpleHeuristic(TileCell *tileCell, TileCell *nextTileCell, TileCell *targetTileCell);
};