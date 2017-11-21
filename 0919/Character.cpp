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
#include "DefenseState.h"
#include "DeadState.h"
#include "Font.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Component(name)
{
	_state = NULL;
	_moveTime = 1.0f;
	_pngName = textureFilename;
	_scriptFileName = scriptName;
	_attackCoolTime = 1.0f;
	_attackCoolTimeDuration = 0.0f;

	// 공격력 수치 설정
	_attackPoint = 30;

	// HP 수치 설정
	_hp = 100;
}

Character::~Character()
{
	delete _font;
}

void Character::Init()
{
	{
		Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");		// L"tileMap"

		// 캐릭터 이동 시작 지점
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

	{
		State *state = new DefenseState();
		state->Init(this);
		_stateMap[eStateType::EF_DEFENSE] = state;
	}

	{
		State *state = new DeadState();
		state->Init(this);
		_stateMap[eStateType::ET_DEAD] = state;
	}

	ChangeState(eStateType::ET_IDLE);

	// Font
	{
		D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0);
		_font = new Font(L"Arial", 20, color);

		_font->SetRect(100, 100, 400, 100);
		// _font->SetText(L"Hello");
		UpdateText();
	}
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
	UpdateAttackCoolTime(deltaTime);
	_state->Update(deltaTime);

	UpdateText();
}

void Character::Render()
{
	_state->Render();

	// Font Test
	{
		_font->SetPosition(_x-200, _y-65);
		_font->Render();
	}
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
	_state->NextState(eStateType::ET_MOVE);
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

	// 자연스러운 이동을 위한 보간 작업
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		// 이동거리
		_targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);

		float distanceX = _targetX - _x;
		float distanceY = _targetY - _y;

		// 최소 이동거리
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
		_attackedPoint = msgParam.attackPoint;
		ChangeState(eStateType::EF_DEFENSE);
	}
}

Component *Character::Collision(std::list<Component*> &collisonList)
{
	for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
	{
		sComponentMsgParam msgParam;
		msgParam.sender = this;
		msgParam.recevier = (*it);
		msgParam.message = L"Collision";
		ComponentSystem::GetInstance()->SendMsg(msgParam);
	}
	return NULL;
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

void Character::DecreaseHP(int decreaseHP)
{
	_hp -= decreaseHP;

	if (_hp < 0)
	{
		_isLive = false;
	}
}

void Character::UpdateAttackCoolTime(float deltaTime)
{
	if (_attackCoolTimeDuration < _attackCoolTime)
	{
		_attackCoolTimeDuration += deltaTime;
	}
	else
	{
		_attackCoolTimeDuration = _attackCoolTime;
	}
}

bool Character::IsAttackCoolTime()
{
	if (_attackCoolTime <= _attackCoolTimeDuration)
		return true;
	return false;
}

void Character::ResetAttackCoolTime()
{
	_attackCoolTimeDuration = 0.0f;
}

void Character::UpdateText()
{
	// int coolTime = (int)(_attackCoolTimeDuration * 60.0f);

	WCHAR text[256];
	switch (GetType())
	{
	case CT_PLAYER:
		wsprintf(text, L"<Player>\nHP %d\nAttackPoint %d",_hp , _attackPoint);
		_font->SetText(text);
		break;

	case CT_MONSTER:
		wsprintf(text, L"<Monster>\nHP %d\nAttackPoint %d", _hp, _attackPoint);
		_font->SetText(text);
		break;

	case CT_NPC:
		wsprintf(text, L"<NPC>\nHP %d\nAttackPoint %d", _hp, _attackPoint);
		_font->SetText(text);
		break;

	default:
		break;
	}
}