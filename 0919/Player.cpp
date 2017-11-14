#include "Player.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Component.h"
#include "MoveState.h"

Player::Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename)
	: Character(name, scriptName, textureFilename)
{
	// player 속도 조절
	_moveTime = 0.1f;

	_type = eComponentType::CT_PLAYER;
}

Player::~Player()
{

}

void Player::UpdateAI()
{
	if (false == _state->IsMoving())
	{
		eDirection direction = eDirection::NONE;

		if (GameSystem::GetInstance()->IsKeyDown(VK_UP))
		{
			direction = eDirection::UP;
		}

		if (GameSystem::GetInstance()->IsKeyDown(VK_DOWN))
		{
			direction = eDirection::DOWN;
		}

		if (GameSystem::GetInstance()->IsKeyDown(VK_LEFT))
		{
			direction = eDirection::LEFT;
		}

		if (GameSystem::GetInstance()->IsKeyDown(VK_RIGHT))
		{
			direction = eDirection::RIGHT;
		}

		if (eDirection::NONE != direction)
		{
			_currentDirection = direction;
			_state->Start();
		}
	}
}