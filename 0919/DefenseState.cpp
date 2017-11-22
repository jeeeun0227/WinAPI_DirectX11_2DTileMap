#include "DefenseState.h"
#include "Character.h"

DefenseState::DefenseState()
{
	_nowState = eStateType::EF_DEFENSE;
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
		_nextState = eStateType::ET_DEAD;
	}
	else
	{
		_nextState = eStateType::ET_MOVE;
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