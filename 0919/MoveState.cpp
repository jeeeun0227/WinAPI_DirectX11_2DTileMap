#include "ComponentSystem.h"
#include "Map.h"
#include "MoveState.h"
#include "Character.h"
#include "GameSystem.h"
#include "Stage.h"

MoveState::MoveState()
{
	_nowState = eStateType::ET_MOVE;
}

MoveState::~MoveState()
{

}

void MoveState::Init(Character *character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void MoveState::Update(float deltaTime)
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
		_nextState = eStateType::ET_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void MoveState::Stop()
{
	State::Stop();
}

void MoveState::Start()
{
	State::Start();

	if (true == _character->IsMoving())
		return;

	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

	TilePosition currentTilePos;
	currentTilePos.x = _character->GetTileX();
	currentTilePos.y = _character->GetTileY();
	TilePosition nextTilePos = GetNextTilePosition(currentTilePos, _character->GetDirection());

	std::list<Component*> collisonList;
	bool canMove = map->GetTileCollisonList(nextTilePos.x, nextTilePos.y, collisonList);
	if (false == canMove)
	{
		Component *target = _character->Collision(collisonList);

		if (NULL != target && _character->IsAttackCoolTime())
		{
			_character->ResetAttackCoolTime();
			_character->SetTarget(target);

			_nextState = eStateType::ET_ATTACK;
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_character->MoveStart(nextTilePos.x, nextTilePos.y);
		_character->MoveStop();
		_movingDuration = 0.0f;
	}
}