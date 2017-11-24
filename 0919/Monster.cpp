#include "Monster.h"
#include "ComponentMessage.h"
#include "MoveState.h"
#include "GameSystem.h"
#include "map.h"
#include "Stage.h"

Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename)
	: Character(name, scriptName, textureFilename)
{
	_type = eComponentType::CT_MONSTER;

	int speed = (rand() % 1500) + 200;		// ��ȣ�� ���� ����
	_moveTime = (float)speed / 1000.0f;		// ����
	
	_attackPoint = 20;		// ���ݷ� ������
	_hp = 150;		// ü�� ������
}

Monster::~Monster()
{

}

void Monster::UpdateAI()
{
	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();
	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_NPC);
	compareTypeList.push_back(eComponentType::CT_PLAYER);
	Component *FindEnemy = ComponentSystem::GetInstance()->FindComponentInRange(map, this, 4, compareTypeList);

	if (NULL != FindEnemy)
	{
		// �߰� ���� ����
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
			ChangeState(ET_MOVE);
		}
	}

	else
	{
		Character::UpdateAI();
	}
}

Component *Monster::Collision(std::list<Component*> &collisonList)
{
	// collisonList ��ȯ
	for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
	{
		Component *com = (*it);

		if (com->GetType() == eComponentType::CT_NPC ||
			com->GetType() == eComponentType::CT_PLAYER)
		{
			return (*it);
		}
	}
	return NULL;
}