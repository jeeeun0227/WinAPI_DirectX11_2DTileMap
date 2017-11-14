#include "Monster.h"
#include "ComponentMessage.h"
#include "MoveState.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename)
	: Character(name, scriptName, textureFilename)
{
	_type = eComponentType::CT_MONSTER;

	int speed = (rand() % 1500) + 200;		// 기호에 따라 수정
	_moveTime = (float)speed / 1000.0f;		// 고정

	_attackPoint = 10;		// 재정의
}

Monster::~Monster()
{

}

void Monster::UpdateAI()
{
	if (false == _state->IsMoving())
	{
		std::vector<eComponentType> compareTypeList;
		compareTypeList.push_back(eComponentType::CT_NPC);
		compareTypeList.push_back(eComponentType::CT_PLAYER);
		Component *FindEnemy = ComponentSystem::GetInstance()->FindComponentInRange(this, 4, compareTypeList);

		if (NULL != FindEnemy)
		{
			// 추격 방향 설정
			eDirection direction = eDirection::NONE;

			if (FindEnemy->GetTileX() < _tileX)
			{
				direction = eDirection::LEFT;
			}

			else if (_tileX < FindEnemy->GetTileX())
			{
				direction = eDirection::RIGHT;
			}

			else if (FindEnemy->GetTileY() < _tileY)
			{
				direction = eDirection::UP;
			}

			else if (_tileY < FindEnemy->GetTileY())
			{
				direction = eDirection::DOWN;
			}

			if (eDirection::NONE != direction)
			{
				_currentDirection = direction;
				_state->Start();
			}
		}

		else
		{
			Character::UpdateAI();
		}
	}
}

void Monster::Collision(std::list<Component*> &collisonList)
{
	// collisonList 순환
	for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
	{
		Component *com = (*it);

		if (com->GetType() == eComponentType::CT_NPC ||
			com->GetType() == eComponentType::CT_PLAYER)
		{
			// Attack Effect 구현
			sComponentMsgParam msgParam;
			msgParam.sender = this;
			msgParam.attackPoint = _attackPoint;
			msgParam.recevier = (*it);
			msgParam.message = L"Attack";
			ComponentSystem::GetInstance()->SendMsg(msgParam);
		}
	}
}