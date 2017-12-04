#include "Stage.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "LifeNpc.h"
#include "LifePlayer.h"
#include "Part.h"

Stage::Stage()
{
	_part = NULL;
}

Stage::~Stage()
{
	// _componentList.clear();

	if (NULL != _part)
	{
		delete _part;
		_part = NULL;
	}

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}

	ComponentSystem::GetInstance()->RemoveAllComponents();

	delete _part;
}

void Stage::Init(std::wstring  StageName)
{
	_componentList.clear();

	// map 생성
	_map = new Map(StageName.c_str());
	_componentList.push_back(_map);

	// _part->_lifeNpcCount = 0;

	Player *player = NULL;

	if (L"Map3" == StageName)
	{
		_part = new Part(this);
		_part->CreateComponents(100);
	}

	else
	{
		// npc 생성
		NPC *_npc;

		for (int i = 0; i < 10; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
			_componentList.push_back(_npc);
		}

		// monster 생성
		for (int i = 0; i < 20; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			Monster *_monster = new Monster(name, L"monster", L"monster");
			_componentList.push_back(_monster);
		}

		// 회복 item 생성
		for (int i = 0; i < 10; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"recover_item_%d", i);
			RecoveryItem *_item = new RecoveryItem(name, L"recovery_item", L"item_sprites");
			_componentList.push_back(_item);
		}
		player = new Player(L"player", L"player", L"Player_Sprite_00");
		// 생성자 (컴포넌트 이름(고유 이름), json 파일 이름.json, 이미지 이름.png);
	}

	_componentList.push_back(player);

	/*
	if(L"Map3" == mapNmae)
	{
		_part = new LifePart(this);
	}

	else
	{
		_part = new DefaultPart(this);
	}
	*/

	/*
	< 최종적인 리팩토링의 결과 모습 >

	if(find(mapName) == true)
		_partMap[mapName]->CreateComponents();
	else
		_partMap["default"]->CreateComponents();
	*/

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Init();
	}

	_map->InitViewer(player);
}

void Stage::Deinit()
{

}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Update(deltaTime);
	}

	UpdateBaseComponentList();
	UpdateRemoveComponentList();
}

void Stage::Render()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Render();
	}
}

void Stage::Relese()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Relese();
	}
}

void Stage::Reset()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Reset();
	}
}

void Stage::CreateLifeNPC(Component *component)
{
	_createBaseComponentList.push_back(component);
}

void Stage::DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);
	// tileCharacter->SetCanMove(true);
	// tileCharacter->SetLive(false);

	_componentList.remove(tileCharacter);
	ComponentSystem::GetInstance()->RemoveComponent(tileCharacter);
}

void Stage::CheckDestroyLifeNPC(Component *component)
{
	_removeComponentList.push_back(component);
}

void Stage::UpdateRemoveComponentList()
{
	for (std::list<Component*>::iterator it = _removeComponentList.begin(); it != _removeComponentList.end(); it++)
	{
		Component *com = (*it);
		DestoryLifeNPC(com->GetTileX(), com->GetTileY(), com);
	}
	_removeComponentList.clear();
}

void Stage::UpdateBaseComponentList()
{
	for (std::list<Component*>::iterator it = _createBaseComponentList.begin(); it != _createBaseComponentList.end(); it++)
	{
		Component *baseComponent = (*it);
		
		LifeNpc *npc = (LifeNpc*)(_part->CreateLifeNPC(L"npc", L"Npc_Sprite_01"));
		// _componentList.push_back(npc);

		npc->Init(baseComponent->GetTileX(), baseComponent->GetTileY());
	}
	_createBaseComponentList.clear();
}

void Stage::AddStageComponent(Component *component)
{
	_componentList.push_back(component);
}