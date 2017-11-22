#include "IdleState.h"
#include "Character.h"

IdleState::IdleState()
{
	_nowState = eStateType::ET_IDLE;
}

IdleState::~IdleState()
{

}

void IdleState::Init(Character *character)
{
	State::Init(character);
}

void IdleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (false == _character->isLive())
		return;

	if (false == _character->IsMoving())
		_character->UpdateAI();

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void IdleState::Stop()
{
	State::Stop();
}

void IdleState::Start()
{
	State::Start();
}