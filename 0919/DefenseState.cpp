#include "DefenseState.h"
#include "Character.h"

DefenseState::DefenseState()
{

}

DefenseState::~DefenseState()
{

}

void DefenseState::Init(Character *character)
{
	State::Init(character);
}

void DefenseState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void DefenseState::Stop()
{
	State::Stop();
}

void DefenseState::Start()
{
	State::Start();

	int attackPoint = _character->GetAttackedPoint();
	_character->DecreaseHP(attackPoint);

	if (false == _character->isLive())
	{
		_character->SetCanMove(true);
		_character->MoveStop();
		// _character->ChangeState(eStateType::ET_DEAD);
		_nextState = eStateType::ET_DEAD;
	}
	else
	{
		//_character->ChangeState(eStateType::ET_IDLE);
		_nextState = eStateType::ET_IDLE;
	}
}

void DefenseState::Render()
{
	State::Render();
}

void DefenseState::Relese()
{
	State::Relese();
}

void DefenseState::Reset()
{
	State::Reset();
}

void DefenseState::Deinit()
{
	State::Deinit();
}