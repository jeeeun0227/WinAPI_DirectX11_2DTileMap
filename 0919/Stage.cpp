#include "Stage.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "LifeNpc.h"

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

	// map 생성
	_map = new Map(StageName.c_str());
	_componentList.push_back(_map);

	if (L"Map3" == StageName)
	{
		for (int i = 0; i < 30; i++)
		{
			// CreateLifeNPC();
			_lifeNpcCount = 0;
			
			WCHAR name[256];
			wsprintf(name, L"Lifenpc_%d", _lifeNpcCount);
			_lifeNpcCount++;

			LifeNpc *npc = new LifeNpc(L"npc", L"npc", L"Npc_Sprite_01");
			_componentList.push_back(npc);
			
		}
	}
	else
	{
		// npc 생성
		NPC *_npc;
		for (int i = 0; i < 0; i++)
		{
			WCHAR name[256];
			wsprintf(name, L"npc_%d", i);
			_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
			_componentList.push_back(_npc);
		}

		// monster 생성
		for (int i = 0; i < 5; i++)
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
	}

		// player 생성
		WCHAR PlayerName[256];
		wsprintf(PlayerName, L"player");
		Player *_player = new Player(PlayerName, L"player", L"Player_Sprite_00");
		// 생성자 (컴포넌트 이름(고유 이름), json 파일 이름.json, 이미지 이름.png);
		_componentList.push_back(_player);

		for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
		{
			(*it)->Init();
		}

		_map->InitViewer(_player);
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

void Stage::CreateLifeNPC(int tileX, int tileY)
{
	WCHAR name[256];
	wsprintf(name, L"Lifenpc_%d", _lifeNpcCount);
	LifeNpc *npc = new LifeNpc(L"npc", L"npc", L"Npc_Sprite_01");
	npc->Init(tileX, tileY);
	_componentList.push_back(npc);
}

void Stage::DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);
	tileCharacter->SetCanMove(true);
	tileCharacter->SetLive(false);
}