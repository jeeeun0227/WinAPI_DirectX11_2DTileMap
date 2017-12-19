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

	typedef struct _sPathCommand
	{
		float heuristic;
		TileCell *tileCell;
	} sPathCommand;

	struct compare
	{
		bool operator()(sPathCommand& a, sPathCommand& b)
		{
			// return a->GetDistanceFromStart() > b->GetDistanceFromStart();
			return a.heuristic > b.heuristic;
		}
	};

private:
	// std::queue<TileCell*> _pathFindingTileQueue;
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathFindingTileQueue;		// 우선순위 큐
	TileCell *_targetTileCell;
	TileCell *_reverseTileCell;
	eUpdateState _updateState;

	void UpdatePathFinding();
	void UpdateBuildPath();

	float CalcSimpleHeuristic(TileCell *tileCell, TileCell *nextTileCell, TileCell *targetTileCell);
	float CalcComplexHeuristic(TileCell *nextTileCell, TileCell *targetTileCell);
	float CalcAStarHeuristic(float distanceFromStart, TileCell *nextTileCell, TileCell *targetTileCell);
};