#include <stdio.h>
#include <list>

#include "Character.h"
#include "Sprite.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "ComponentMessage.h"
#include "MoveState.h"
#include "IdleState.h"
#include "AttackState.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Component(name)
{
	_state = NULL;
	_moveTime = 1.0f;
	_pngName = textureFilename;
	_scriptFileName = scriptName;

	// ���ݷ� ��ġ ����
	_attackPoint = 10;

	// HP ��ġ ����
	_hp = 100;
}

Character::~Character()
{
	delete _state;
}

void Character::Init()
{
	{
		Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");		// L"tileMap"

		// ĳ���� �̵� ���� ����
		_tileX = rand() % (map->GetWidth() - 1) + 1;
		_tileY = rand() % (map->GetHeight() - 1) + 1;

		while (!map->CanMoveTileMap(_tileX, _tileY))
		{
			_tileX = rand() % map->GetWidth();
			_tileY = rand() % map->GetHeight();
		}

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);
		map->SetTileComponent(_tileX, _tileY, this, true);
	}
	InitMove();

	{
		State *state = new IdleState();
		state->Init(this);
		_stateMap[eStateType::ET_IDLE] = state;
	}

	{
		State *state = new MoveState();
		state->Init(this);
		_stateMap[eStateType::ET_MOVE] = state;
	}

	{
		State *state = new AttackState();
		state->Init(this);
		_stateMap[eStateType::ET_ATTACK] = state;
	}

	ChangeState(eStateType::ET_IDLE);
}

void Character::Deinit()
{
	std::map<eStateType, State*>::iterator it = _stateMap.begin();

	while (it != _stateMap.end())
	{
		State *state = it->second;
		delete state;
		it++;
	}
	_stateMap.clear();

	_state->Deinit();
}

void Character::Update(float deltaTime)
{
	_state->Update(deltaTime);
}

void Character::Render()
{
	_state->Render();
}

void Character::Relese()
{
	_state->Relese();
}

void Character::Reset()
{
	_state->Reset();
}

void Character::MoveDeltaPosition(float deltaX, float deltaY)
{
	_x += deltaX;
	_y += deltaY;
}

void Character::UpdateAI()
{
	_currentDirection = (eDirection)(rand() % 4);
	ChangeState(eStateType::ET_MOVE);
}

void Character::ChangeState(eStateType stateType)
{
	if (NULL != _state)
	{
		_state->Stop();
	}

	_state = _stateMap[stateType];
	_state->Start();
}

void Character::InitMove()
{
	_isMoving = false;
	_currentDirection = eDirection::DOWN;
}

void Character::MoveStart(int newTileX, int newTileY)
{
	Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");

	map->ResetTileComponent(_tileX, _tileY, this);
	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);

	_tileX = newTileX;
	_tileY = newTileY;

	// �ڿ������� �̵��� ���� ���� �۾�
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		// �̵��Ÿ�
		_targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);

		float distanceX = _targetX - _x;
		float distanceY = _targetY - _y;

		// �ּ� �̵��Ÿ�
		_moveDistancePerTimeX = distanceX / _moveTime;
		_moveDistancePerTimeY = distanceY / _moveTime;
	}
	_isMoving = true;
}

void Character::SetPosition(float PosX, float PosY)
{
	_x = PosX;
	_y = PosY;
}

void Character::RaceiveMessage(const sComponentMsgParam msgParam)
{
	if (L"Attack" == msgParam.message)
	{
		int attackPoint = msgParam.attackPoint;
		_hp -= attackPoint;

		if (_hp < 0)
		{
			// Dead
			_isLive = false;
			SetCanMove(true);

			// Stop
			_moveDistancePerTimeX = 0.0f;
			_moveDistancePerTimeY = 0.0f;
		}
	}
}

void Character::Collision(std::list<Component*> &collisonList)
{
	if (eComponentType::CT_MONSTER == _type)
	{
		//collisonList ��ȯ
		for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
		{
			Component *com = (*it);

			if (com->GetType() == eComponentType::CT_NPC ||
				com->GetType() == eComponentType::CT_PLAYER)
			{
				// Attack Effect ����
				sComponentMsgParam msgParam;
				msgParam.sender = this;
				msgParam.recevier = (*it);
				msgParam.message = L"Attack";
				ComponentSystem::GetInstance()->SendMsg(msgParam);
			}
		}
	}

	else
	{
		for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
		{
			sComponentMsgParam msgParam;
			msgParam.sender = this;
			msgParam.recevier = (*it);
			msgParam.message = L"Collision";
			ComponentSystem::GetInstance()->SendMsg(msgParam);
		}
	}
}

void Character::MoveStop()
{
	Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");

	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);

	_moveDistancePerTimeX = 0.0f;
	_moveDistancePerTimeY = 0.0f;

	_isMoving = false;
}

void Character::Moving(float deltaTime)
{
	float moveDistanceX = _moveDistancePerTimeX * deltaTime;
	float moveDistanceY = _moveDistancePerTimeY * deltaTime;

	_x += moveDistanceX;
	_y += moveDistanceY;
}