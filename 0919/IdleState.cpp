#include "IdleState.h"
#include "Character.h"

IdleState::IdleState()
{

}

IdleState::~IdleState()
{

}

void IdleState::Init(Character *character)
{
	_character = character;
}

void IdleState::Update(float deltaTime)
{
	if (false == _character->isLive())
		return;

	if (false == _character->IsMoving())
		_character->UpdateAI();
}

void IdleState::Stop()
{

}

void IdleState::Start()
{

}