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

	if (false == _character->IsMoving())
		return;

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.0f;

		_character->MoveStop();
		_nextState = eStateType::ET_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
		_character->Moving(deltaTime);
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

	// Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");
	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

	int newTileX = _character->GetTileX();
	int newTileY = _character->GetTileY();

	switch (_character->GetDirection())
	{
	case eDirection::LEFT:
		newTileX--;
		break;

	case eDirection::RIGHT:
		newTileX++;
		break;

	case eDirection::UP:
		newTileY--;
		break;

	case eDirection::DOWN:
		newTileY++;
		break;
	}

	std::list<Component*> collisonList;
	bool canMove = map->GetTileCollisonList(newTileX, newTileY, collisonList);
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
		_character->MoveStart(newTileX, newTileY);
	}
}