#include "ComponentSystem.h"
#include "Map.h"
#include "PathFindingMoveState.h"
#include "Character.h"
#include "GameSystem.h"
#include "Stage.h"
#include "GlobalType.h"

PathFindingMoveState::PathFindingMoveState()
{
	_nowState = eStateType::ET_MOVE;
}

PathFindingMoveState::~PathFindingMoveState()
{

}

void PathFindingMoveState::Init(Character *character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void PathFindingMoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->isLive())
		return;

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.001f;

		if (0 != _pathTileCellStack.size())
		{
			TileCell *tileCell = _pathTileCellStack.top();
			_pathTileCellStack.pop();

			/*
			// player가 이동에 맞게 방향을 설정하도록 구현
			TilePosition to:
			TilePosition from;
			eDirection direction = GetDirection(to, from);
			_character->SetDirection(direction);
			*/

			TilePosition to;
			to.x = tileCell->GetTileX();
			to.y = tileCell->GetTileY();

			TilePosition from;
			from.x = _character->GetTileX();
			from.y = _character->GetTileY();

			eDirection direction = GetDirection(to, from);
			if(eDirection::NONE != direction)
			_character->SetDirection(direction);

			_character->MoveStart(tileCell->GetTileX(), tileCell->GetTileY());
			_character->MoveStop();
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void PathFindingMoveState::Stop()
{
	State::Stop();
}

void PathFindingMoveState::Start()
{
	State::Start();

	_nextState = eStateType::ET_NONE;
	_movingDuration = 0.0f;

	_pathTileCellStack = _character->GetPathTileCellStack();
	_pathTileCellStack.pop();
}