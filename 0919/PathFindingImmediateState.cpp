#include "PathFindingImmediateState.h"

PathFindingImmediateState::PathFindingImmediateState()
{

}

PathFindingImmediateState::~PathFindingImmediateState()
{

}

void PathFindingImmediateState::Start()
{
	PathFindingState::Start();

	while (0 != _pathFindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathFinding();
	}

	while (_reverseTileCell)
	{
		UpdateBuildPath();
	}
}