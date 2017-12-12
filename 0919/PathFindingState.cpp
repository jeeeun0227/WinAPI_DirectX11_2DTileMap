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

	// ��ã�� �˰��� ����
	if (0 != _pathFindingTileQueue.size())
	{
		// ù��° ��带 ������ �˻�
		TileCell * tileCell = _pathFindingTileQueue.front();
		_pathFindingTileQueue.pop();
		if (false == tileCell->IsPathFindingMark())
		{
			tileCell->PathFinded();

			{
				wchar_t log[256];
				wsprintf(log, L"pos : %d %d to %d %d\n", tileCell->GetTileX(), tileCell->GetTileY(),
					_targetTileCell->GetTileX(), _targetTileCell->GetTileY());
				OutputDebugString(log);
			}			

			// ��ǥ Ÿ���̸� ����
			if (tileCell->GetTileX() == _targetTileCell->GetTileX() && 
				tileCell->GetTileY() == _targetTileCell->GetTileY())
			{
				OutputDebugString(L"-- Find Goal!! --\n");

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

				_nextState = eStateType::ET_IDLE;
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
					if (NULL == nextTileCell->GetPrevPathFindingCell())
					{
						nextTileCell->SetPrevPathFindingCell(tileCell);
						_pathFindingTileQueue.push(nextTileCell);

						if (
							// (nextTileCell->GetTileX() != _targetTileCell->GetTileX() || nextTileCell->GetTileY() != _targetTileCell->GetTileY())
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
							&&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							GameSystem::GetInstance()->GetStage()->CreatePathFindingNPC(nextTileCell);
						}
					}
				}
			}
		}
	}
}

void PathFindingState::Start()
{
	State::Start();

	_targetTileCell = _character->GetTargetCell();

	// 1.  ��� TileCell�� ��ã�� �Ӽ� �ʱ�ȭ
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

	while (0 != _pathFindingTileQueue.size())
	{
		_pathFindingTileQueue.pop();
	}
}
