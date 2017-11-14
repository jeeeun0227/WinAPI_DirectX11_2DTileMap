#include <stdio.h>
#include <list>

#include "Character.h"
#include "Sprite.h"
#include "Map.h"
#include "ComponentSystem.h"
#include "ComponentMessage.h"
#include "MoveState.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Component(name)
{
	_state = new MoveState();
	_spriteList.clear();
	_moveTime = 1.0f;
	_pngName = textureFilename;
	_scriptFileName = scriptName;

	// 공격력 수치 설정
	_attackPoint = 10;

	// HP 수치 설정
	_hp = 100;
}

Character::~Character()
{
	delete _state;
}

void Character::Init()
{
	WCHAR textureFileName[256];
	wsprintf(textureFileName, L"%s.png", _pngName.c_str());

	WCHAR scriptFileName[256];

	{
		wsprintf(scriptFileName, L"%s_left.json", _scriptFileName.c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_right.json", _scriptFileName.c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_up.json", _scriptFileName.c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_down.json", _scriptFileName.c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");		// L"tileMap"

		// 캐릭터 이동 시작 지점
		_tileX = rand() % (map->GetWidth() - 1) + 1;
		_tileY = rand() % (map->GetHeight() - 1) + 1;

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);

		_targetX = _x;
		_targetY = _y;

		map->SetTileComponent(_tileX, _tileY, this, false);
	}
	InitMove();
}

void Character::Deinit()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Character::Update(float deltaTime)
{
	_spriteList[(int)_currentDirection]->Update(deltaTime);

	UpdateAI();
	//UpdateMove(deltaTime);
	_state->Update(deltaTime);
}

void Character::Render()
{
	_spriteList[(int)_currentDirection]->SetPosition(_x, _y);

	_spriteList[(int)_currentDirection]->Render();
}

void Character::Relese()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Relese();
	}
}

void Character::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void Character::MoveDeltaPosition(float deltaX, float deltaY)
{
	_x += deltaX;
	_y += deltaY;
}

void Character::UpdateAI()
{
	if (false == _isLive)
		return;

	if (false == _state->IsMoving())
	{
		_currentDirection = (eDirection)(rand() % 4);
		_state->Start();
	}
}

void Character::UpdateMove(float deltaTime)
{
	if (_moveTime <= _state->GetMovingDuration())
	{
		_state->Stop();

		Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");

		_x = map->GetPositionX(_tileX, _tileY);
		_y = map->GetPositionY(_tileX, _tileY);

		_moveDistancePerTimeX = 0.0f;
		_moveDistancePerTimeY = 0.0f;
	}
	else
	{
		_state->UpdateMove(deltaTime);

		float moveDistanceX = _moveDistancePerTimeX * deltaTime;
		float moveDistanceY = _moveDistancePerTimeY * deltaTime;

		_x += moveDistanceX;
		_y += moveDistanceY;
	}
}

void Character::InitMove()
{
	_state->Init(this);
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
		//collisonList 순환
		for (std::list<Component*>::iterator it = collisonList.begin(); it != collisonList.end(); it++)
		{
			Component *com = (*it);

			if (com->GetType() == eComponentType::CT_NPC ||
				com->GetType() == eComponentType::CT_PLAYER)
			{
				// Attack Effect 구현
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