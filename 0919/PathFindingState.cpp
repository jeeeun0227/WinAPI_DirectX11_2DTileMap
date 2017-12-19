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
	// _pathFindingTileQueue.push(startTileCell);
	
	sPathCommand newCommand;
	newCommand.heuristic = 0.0f;
	newCommand.tileCell = startTileCell;
	_pathFindingTileQueue.push(newCommand);

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
		// TileCell * tileCell = _pathFindingTileQueue.top();
		sPathCommand command = _pathFindingTileQueue.top();
		_pathFindingTileQueue.pop();
		if (false == command.tileCell->IsPathFindingMark())
		{
			command.tileCell->PathFinded();

			{
				wchar_t log[256];
				wsprintf(log, L"position : %d %d / destination : %d %d\n", command.tileCell->GetTileX(), command.tileCell->GetTileY(),
					_targetTileCell->GetTileX(), _targetTileCell->GetTileY());
				OutputDebugString(log);
			}

			// 목표 타일이면 종료
			if (command.tileCell->GetTileX() == _targetTileCell->GetTileX() &&
				command.tileCell->GetTileY() == _targetTileCell->GetTileY())
			{

				std::list<Component*> componentList = command.tileCell->GetComponentList();
				for (std::list<Component*>::iterator it = componentList.begin(); it != componentList.end(); it++)
				{
					if (eComponentType::CT_MONSTER == (*it)->GetType())
					{
						Character *monster = (Character*)(*it);

						if (command.tileCell->GetTileX() < command.tileCell->GetPrevPathFindingCell()->GetTileX())
						{
							monster->SetDirection(eDirection::RIGHT);
						}
						else if (command.tileCell->GetTileX() > command.tileCell->GetPrevPathFindingCell()->GetTileX())
						{
							monster->SetDirection(eDirection::LEFT);
						}
						if (command.tileCell->GetTileY() < command.tileCell->GetPrevPathFindingCell()->GetTileY())
						{
							monster->SetDirection(eDirection::DOWN);
						}
						else if (command.tileCell->GetTileY() > command.tileCell->GetPrevPathFindingCell()->GetTileY())
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
				currentTilePos.x = command.tileCell->GetTileX();
				currentTilePos.y = command.tileCell->GetTileY();
				TilePosition nextTilePos = GetNextTilePosition(currentTilePos, (eDirection)direction);

				Map *map = GameSystem::GetInstance()->GetStage()->GetMap();
				TileCell *nextTileCell = map->GetTileCell(nextTilePos);

				if (
					(true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathFindingMark())
					||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
					)
				{
					float distanceFromStart = command.tileCell->GetDistanceFromStart() + command.tileCell->GetDistanceWeight();

					// float heuristic = distanceFromStart;
					// float heuristic = CalcSimpleHeuristic(tileCell, nextTileCell, _targetTileCell);		// 만약 몬스터가 계속 움직이는 상황이라면 CalcSimpleHeuristic을 사용하는 것이 좋다.
					// float heuristic = CalcComplexHeuristic(nextTileCell, _targetTileCell);
					float heuristic = CalcAStarHeuristic(distanceFromStart, nextTileCell, _targetTileCell);

					if (NULL == nextTileCell->GetPrevPathFindingCell())
					{
						nextTileCell->SetDistanceFromStart(distanceFromStart);
						nextTileCell->SetHeuristic(heuristic);
						nextTileCell->SetPrevPathFindingCell(command.tileCell);
						// _pathFindingTileQueue.push(nextTileCell);

						sPathCommand newCommand;
						newCommand.heuristic = heuristic;
						newCommand.tileCell = nextTileCell;
						_pathFindingTileQueue.push(newCommand);

						if (
							// (nextTileCell->GetTileX() != _targetTileCell->GetTileX() || nextTileCell->GetTileY() != _targetTileCell->GetTileY())
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
							&&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							GameSystem::GetInstance()->GetStage()->CreatePathFindingNPC(nextTileCell);
							// ↑ 주석을 풀어주면 타일 검사하는 것을 시각적으로 볼 수 있다.
						}
					}
					else
					{
						if (distanceFromStart < nextTileCell->GetDistanceFromStart())
						{
							// 다시 검사
							nextTileCell->SetDistanceFromStart(distanceFromStart);
							nextTileCell->SetPrevPathFindingCell(command.tileCell);
							// _pathFindingTileQueue.push(nextTileCell);
							// nextTileCell->SetHeuristic(new hhh);		포인터가 들어가고, 포인터에서 비교하는 값을 조작하면 우선순위 큐가 망가진다.

							sPathCommand newCommand;
							newCommand.heuristic = CalcAStarHeuristic(distanceFromStart, nextTileCell, _targetTileCell);
							newCommand.tileCell = nextTileCell;
							_pathFindingTileQueue.push(newCommand);
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
		/*
		if (_reverseTileCell->GetTileX() != _targetTileCell->GetTileX() ||
			_reverseTileCell->GetTileY() != _targetTileCell->GetTileY())
		{
		
			GameSystem::GetInstance()->GetStage()->CreatePathFindingMark(_reverseTileCell);
			// ↑ 주석을 풀어주면 타일 검사하는 것을 시각적으로 볼 수 있다.
			_character->PushPathTileCell(_reverseTileCell);
		}
		*/
		GameSystem::GetInstance()->GetStage()->CreatePathFindingMark(_reverseTileCell);
		_character->PushPathTileCell(_reverseTileCell);
		_reverseTileCell = _reverseTileCell->GetPrevPathFindingCell();
	}
	else
	{
		_nextState = eStateType::ET_MOVE;
	}
}

float PathFindingState::CalcSimpleHeuristic(TileCell *tileCell, TileCell *nextTileCell, TileCell *targetTileCell)
{
	float heuristic = 0.0f;

	int diffFromCurrent = 0;
	int diffFromNext = 0;

	// x : 발견적 값을 갱신
	{
		diffFromCurrent = tileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromCurrent < 0)
			diffFromCurrent = -diffFromCurrent;

		diffFromNext = nextTileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromNext < 0)
			diffFromNext = -diffFromNext;

		if (diffFromNext < diffFromCurrent)
		{
			heuristic -= 1.0f;
		}
		else if (diffFromNext > diffFromCurrent)
		{
			heuristic += 1.0f;
		}
	}

	// y : 발견적 값을 누적 갱신
	{
		diffFromCurrent = tileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromCurrent < 0)
			diffFromCurrent = -diffFromCurrent;

		diffFromNext = nextTileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromNext < 0)
			diffFromNext = -diffFromNext;

		if (diffFromNext < diffFromCurrent)
		{
			heuristic -= 1.0f;
		}
		else if (diffFromNext > diffFromCurrent)
		{
			heuristic += 1.0f;
		}
	}
	return heuristic;
}

float PathFindingState::CalcComplexHeuristic(TileCell *nextTileCell, TileCell *targetTileCell)
{
	int distanceW = nextTileCell->GetTileX() - targetTileCell->GetTileX();
	int distanceH = nextTileCell->GetTileY() - targetTileCell->GetTileY();

	distanceW = distanceW * distanceW;
	distanceH = distanceH * distanceH;
	
	return (float)((double)distanceW + (double)distanceH);
}

float PathFindingState::CalcAStarHeuristic(float distanceFromStart, TileCell *nextTileCell, TileCell *targetTileCell)
{
	return distanceFromStart + CalcComplexHeuristic(nextTileCell, targetTileCell);
}