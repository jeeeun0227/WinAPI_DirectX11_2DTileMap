#include "Stage.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"

Stage::Stage()
{

}

Stage::~Stage()
{
	_componentList.clear();

	ComponentSystem::GetInstance()->RemoveAllComponents();
}

void Stage::Init(std::wstring StageName)
{
	_componentList.clear();

	// map ����
	_map = new Map(StageName.c_str());
	_componentList.push_back(_map);

	// player ����
	Player *_player = new Player(L"player", L"npc", L"Player_Sprite_00");
	// ������ (������Ʈ �̸�(���� �̸�), json ���� �̸�.json, �̹��� �̸�.png);
	_componentList.push_back(_player);

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

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Init();
	}

	_map->InitViewer(_player);
}

void Stage::Deinit()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}
}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
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
