#include "PathFindingState.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "TileCell.h"
#include "Character.h"
#include "GlobalType.h"

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
	switch (_updateState)
	{
	case eUpdateState::PATHFINDING:
		UpdatePathFinding();
		break;
	case eUpdateState::BUILD_PATH:
		UpdateBuildPath();
		break;
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

	_updateState = eUpdateState::PATHFINDING;
}

void PathFindingState::Stop()
{
	State::Stop();

	while (0 != _pathFindingTileQueue.size())
	{
		_pathFindingTileQueue.pop();
	}
}

void PathFindingState::UpdatePathFinding()
{
	// 길찾기 알고리즘 시작
	if (0 != _pathFindingTileQueue.size())
	{
		// 첫번째 노드를 꺼내서 검사
		TileCell * tileCell = _pathFindingTileQueue.top();
		_pathFindingTileQueue.pop();
		if (false == tileCell->IsPathFindingMark())
		{
			tileCell->PathFinded();

			{
				wchar_t log[256];
				wsprintf(log, L"position : %d %d / destination : %d %d\n", tileCell->GetTileX(), tileCell->GetTileY(),
					_targetTileCell->GetTileX(), _targetTileCell->GetTileY());
				OutputDebugString(log);
			}

			// 목표 타일이면 종료
			if (tileCell->GetTileX() == _targetTileCell->GetTileX() &&
				tileCell->GetTileY() == _targetTileCell->GetTileY())
			{

				std::list<Component*> componentList = tileCell->GetComponentList();
				for (std::list<Component*>::iterator it = componentList.begin(); it != componentList.end(); it++)
				{
					if (eComponentType::CT_MONSTER == (*it)->GetType())
					{
						Character *monster = (Character*)(*it);

						if (tileCell->GetTileX() < tileCell->GetPrevPathFindingCell()->GetTileX())
						{
							monster->SetDirection(eDirection::RIGHT);
						}
						else if (tileCell->GetTileX() > tileCell->GetPrevPathFindingCell()->GetTileX())
						{
							monster->SetDirection(eDirection::LEFT);
						}
						if (tileCell->GetTileY() < tileCell->GetPrevPathFindingCell()->GetTileY())
						{
							monster->SetDirection(eDirection::DOWN);
						}
						else if (tileCell->GetTileY() > tileCell->GetPrevPathFindingCell()->GetTileY())
						{
							monster->SetDirection(eDirection::UP);
						}
					}
				}

				OutputDebugString(L"-- Find Goal!! --\n");
				_updateState = eUpdateState::BUILD_PATH;
				_reverseTileCell = _targetTileCell;
				return;
			}

			for (int direction = 0; direction < eDirection::NONE; direction++)
			{
				TilePosition currentTilePos;
				currentTilePos.x = tileCell->GetTileX();
				currentTilePos.y = tileCell->GetTileY();
				TilePosition nextTilePos = GetNextTilePosition(currentTilePos, (eDirection)direction);

				Map *map = GameSystem::GetInstance()->GetStage()->GetMap();
				TileCell *nextTileCell = map->GetTileCell(nextTilePos);

				if (
					(true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathFindingMark())
					||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
					)
				{
					float distanceFromStart = tileCell->GetDistanceFromStart() + tileCell->GetDistanceWeight();

					if (NULL == nextTileCell->GetPrevPathFindingCell())
					{
						nextTileCell->SetDistanceFromStart(distanceFromStart);
						nextTileCell->SetPrevPathFindingCell(tileCell);
						_pathFindingTileQueue.push(nextTileCell);

						if (
							// (nextTileCell->GetTileX() != _targetTileCell->GetTileX() || nextTileCell->GetTileY() != _targetTileCell->GetTileY())
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
							&&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							// GameSystem::GetInstance()->GetStage()->CreatePathFindingNPC(nextTileCell);
							// ↑ 주석을 풀어주면 타일 검사하는 것을 시각적으로 볼 수 있다.
						}
					}
					else
					{
						if (distanceFromStart < nextTileCell->GetDistanceFromStart())
						{
							// 다시 검사
							nextTileCell->SetDistanceFromStart(distanceFromStart);
							nextTileCell->SetPrevPathFindingCell(tileCell);
							_pathFindingTileQueue.push(nextTileCell);
						}
					}
				}
			}
		}
	}
}

void PathFindingState::UpdateBuildPath()
{
	// 거꾸로 돌아가면서 길을 도출한다.
	if (NULL != _reverseTileCell)
	{
		if (_reverseTileCell->GetTileX() != _targetTileCell->GetTileX() ||
			_reverseTileCell->GetTileY() != _targetTileCell->GetTileY())
		{
			// GameSystem::GetInstance()->GetStage()->CreatePathFindingMark(_reverseTileCell);
			// ↑ 주석을 풀어주면 타일 검사하는 것을 시각적으로 볼 수 있다.
			_character->PushPathTileCell(_reverseTileCell);
		}
		_reverseTileCell = _reverseTileCell->GetPrevPathFindingCell();
	}
	else
	{
		_nextState = eStateType::ET_MOVE;
	}
}

