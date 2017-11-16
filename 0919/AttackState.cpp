#include "AttackState.h"
#include "ComponentSystem.h"
#include "Character.h"

AttackState::AttackState()
{

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
}

void AttackState::Stop()
{
	State::Stop();
}

void AttackState::Start()
{
	State::Start();

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*) _character;
	msgParam.attackPoint = _character->GetAttackPoint();
	msgParam.recevier = _character->GetTarget();
	msgParam.message = L"Attack";
	ComponentSystem::GetInstance()->SendMsg(msgParam);
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