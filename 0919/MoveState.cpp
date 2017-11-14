#include "ComponentSystem.h"
#include "Map.h"
#include "MoveState.h"
#include "Character.h"

MoveState::MoveState()
{

}

MoveState::~MoveState()
{

}

bool MoveState::IsMoving()
{
	return _isMoving;
}

float MoveState::GetMovingDuration()
{
	return _movimgDuration;
}

void MoveState::Start()
{
	if (true == IsMoving())
		return;

	Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");

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
		_character->Collision(collisonList);
	}
	else
	{
		_character->MoveStart(newTileX, newTileY);
	}
	_isMoving = true;
}

void MoveState::Stop()
{
	_movimgDuration = 0.0f;
	_isMoving = false;
}

void MoveState::UpdateMove(float deltaTime)
{
	_movimgDuration += deltaTime;
}

void MoveState::Update(float deltaTime)
{
	if (false == _character->isLive())
		return;

	if (false == IsMoving())
		return;

	_character->UpdateMove(deltaTime);
}

void MoveState::Init(Character *character)
{
	_character = character;
	_isMoving = false;
	_movimgDuration = 0.0f;
}

void MoveState::SetMoving(bool isMoving)
{
	_isMoving = isMoving;
}