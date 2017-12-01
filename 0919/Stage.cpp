#include "Stage.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "LifeNpc.h"
#include "LifePlayer.h"

Stage::Stage()
{

}

Stage::~Stage()
{
	// _componentList.clear();

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}

	ComponentSystem::GetInstance()->RemoveAllComponents();
}

void Stage::Init(std::wstring  StageName)
{
	_componentList.clear();

	// map ����
	_map = new Map(StageName.c_str());
	_componentList.push_back(_map);

	Player *player = NULL;

	if (L"Map3" == StageName)
	{
		for (int i = 0; i < 100; i++)
		{
			// CreateLifeNPC();
			_lifeNpcCount = 0;

			WCHAR name[256];
			wsprintf(name, L"Lifenpc_%d", _lifeNpcCount);
			_lifeNpcCount++;
			LifeNpc *npc = new LifeNpc(L"npc", L"npc", L"Npc_Sprite_01");
			_componentList.push_back(npc);
		}
		player = new LifePlayer(L"player", L"player", L"Player_Sprite_00");
	}
	else
	{
		// npc ����
		NPC *_npc;
		for (int i = 0; i < 0; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
			_componentList.push_back(_npc);
		}

		// monster ����
		for (int i = 0; i < 5; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			Monster *_monster = new Monster(name, L"monster", L"monster");
			_componentList.push_back(_monster);
		}

		// ȸ�� item ����
		for (int i = 0; i < 10; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"recover_item_%d", i);
			RecoveryItem *_item = new RecoveryItem(name, L"recovery_item", L"item_sprites");
			_componentList.push_back(_item);
		}
		player = new LifePlayer(L"player", L"player", L"Player_Sprite_00");
	}

	// ������ (������Ʈ �̸�(���� �̸�), json ���� �̸�.json, �̹��� �̸�.png);

	/*
	// player ����
	WCHAR PlayerName[256];
	wsprintf(PlayerName, L"player");
	Player *_player = new Player(PlayerName, L"player", L"Player_Sprite_00");
	*/

	_componentList.push_back(player);

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
	tileCharacter->SetCanMove(true);
	tileCharacter->SetLive(false);

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

		WCHAR name[256];
		wsprintf(name, L"Lifenpc_%d", _lifeNpcCount);
		_lifeNpcCount++;
		LifeNpc *npc = new LifeNpc(name, L"npc", L"Npc_Sprite_01");
		npc->Init(baseComponent->GetTileX(), baseComponent->GetTileY());
		_componentList.push_back(npc);
	}
	_createBaseComponentList.clear();
}