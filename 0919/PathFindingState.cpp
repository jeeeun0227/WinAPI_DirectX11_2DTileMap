#include "PathFindingState.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "TileCell.h"
#include "Character.h"

PathFindingState::PathFindingState()
{

}

PathFindingState::~PathFindingState()
{

}


void PathFindingState::Init(Character *character)
{
	State::Init(character);
}

void PathFindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	// 길찾기 알고리즘 시작
	if (0 != _pathFindingTileQueue.size())
	{
		// 첫번째 노드를 꺼내서 검사
		TileCell * tileCell = _pathFindingTileQueue.front();
		_pathFindingTileQueue.pop();
		if (false == tileCell->IsPathFindingMark())
		{
			tileCell->PathFinded();

			// 목표 타일이면 종료
			if (tileCell->GetTileX() == _targetTileCell->GetTileX() && 
				tileCell->GetTileY() == _targetTileCell->GetTileY())
			{
				_nextState = eStateType::ET_IDLE;
			}
		}
	}
}

void PathFindingState::Start()
{
	State::Start();

	_targetTileCell = _character->GetTargetCell();

	// 1.  모든 TileCell의 길찾기 속성 초기화
	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

	int height = map->GetHeight();
	int width = map->GetWidth();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileCell *tileCell = map->GetTileCell(x, y);
			tileCell->InitPathFinding();
		}
	}

	TileCell *startTileCell = map->GetTileCell(_character->GetTileX(), _character->GetTileY());
	_pathFindingTileQueue.push(startTileCell);
}

void PathFindingState::Stop()
{
	State::Stop();
}
