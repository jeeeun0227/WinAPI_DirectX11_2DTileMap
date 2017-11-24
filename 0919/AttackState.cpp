#include "AttackState.h"
#include "ComponentSystem.h"
#include "Character.h"
#include "Component.h"

AttackState::AttackState()
{
	_nowState = eStateType::ET_ATTACK;
}

AttackState::~AttackState()
{

}

void AttackState::Init(Character *character)
{
	State::Init(character);
}

void AttackState::Update(float deltaTime)
{
	State::Update(deltaTime);

	_character->ResetTarget();
	_character->ChangeState(eStateType::ET_IDLE);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void AttackState::Stop()
{
	State::Stop();
}

void AttackState::Start()
{
	State::Start();

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*)_character;

	msgParam.attackPoint = _character->GetAttackPoint();

	if (_character->GetAttackPoint() == 50)
	{
		msgParam.recevier = _character->GetTarget();
		msgParam.message = L"Attack";
		ComponentSystem::GetInstance()->SendMsg(msgParam);

		int FinalAttackPoint = _character->GetCriticalAttack();
		_character->SetAttackPoint(FinalAttackPoint);
	}
	else
	{
		msgParam.recevier = _character->GetTarget();
		msgParam.message = L"Attack";
		ComponentSystem::GetInstance()->SendMsg(msgParam);

		if (_character->CanMove() == false)
		{
			_character->ReSetAttackPoint();
		}
	}
}

void AttackState::Render()
{
	State::Render();
}

void AttackState::Relese()
{
	State::Relese();
}

void AttackState::Reset()
{
	State::Reset();
}

void AttackState::Deinit()
{
	State::Deinit();
}