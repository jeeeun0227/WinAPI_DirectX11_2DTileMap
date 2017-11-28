#include "Player.h"
#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Component.h"
#include "MoveState.h"
#include "Map.h"
#include "Stage.h"

Player::Player(std::wstring name, LPCWSTR scriptName, LPCWSTR textureFilename)
	: Character(name, scriptName, textureFilename)
{
	// player 속도 조절
	_moveTime = 0.1f;

	_type = eComponentType::CT_PLAYER;

	_hp = 500;
	_attackPoint = 50;
}

Player::~Player()
{

}

void Player::UpdateAI()
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
		ChangeState(ET_MOVE);
	}

	// 스페이스바를 누르면 아이템을 먹자

	/* 
	1. 스페이스바가 눌려졌을 때
	2. 현재 내가 있는 타일에 아이템이 있는지 검사
	3. 아이템이 있으면 체력을 회복하고
	4. 아이템을 맵에서 제거한다.
	*/

	if (GameSystem::GetInstance()->IsKeyDown(VK_SPACE))
	{
		// Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");
		Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

		std::list<Component*> componentlist = map->GetTileComponentList(_tileX, _tileY);

		for (std::list<Component*>::iterator it = componentlist.begin(); it != componentlist.end(); it++)
		{
			Component *component = (*it);
			
			if (eComponentType::CT_ITEM == component->GetType())
			{
				/*
				_hp = 100;
				map->ResetTileComponent(_tileX, _tileY, component);
				component->SetLive(false);
				*/

				sComponentMsgParam msgParam;
				msgParam.sender = (Component*)this;
				msgParam.recevier = component;
				msgParam.message = L"Use";
				ComponentSystem::GetInstance()->SendMsg(msgParam);
			}
		}
	}

	/*
	1. 스페이스바가 눌려졌을 때
	2. 내가 있는 타일에 아이템이 있는지 검사
	3. 아이템 사용 메세지를 아이템으로 보낸다.
	*/
}

Component *Player::Collision(std::list<Component*> &collisonList)
{
	for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
	{
		Component *com = (*it);

		if (com->GetType() == eComponentType::CT_MONSTER ||
			com->GetType() == eComponentType::CT_PLAYER)
		{
			return (*it);
		}
	}
	return NULL;
}
