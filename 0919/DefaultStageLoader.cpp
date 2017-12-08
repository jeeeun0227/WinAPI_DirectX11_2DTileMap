#include "DefaultStageLoader.h"
#include "Stage.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "PathFinderMonster.h"

DefaultStageLoader::DefaultStageLoader(Stage* stage) : StageLoader(stage)
{

}

DefaultStageLoader::~DefaultStageLoader()
{

}

void DefaultStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);

	// Create NPC
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		NPC *_npc = new NPC(L"npc", L"npc", L"Npc_Sprite_01");
		_stage->AddStageComponent(_npc);
	}

	// Create Monster
	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		Monster *_monster = new Monster(name, L"monster", L"monster");
		_stage->AddStageComponent(_monster);
	}

	// Create Recovery Item
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recover_item_%d", i);
		RecoveryItem *_item = new RecoveryItem(name, L"recovery_item", L"item_sprites");
		_stage->AddStageComponent(_item);
	}

	// Create Player
	Player *player = new Player(L"player", L"player", L"Player_Sprite_00");
	// 생성자 (컴포넌트 이름(고유 이름), json 파일 이름.json, 이미지 이름.png);

	_stage->AddStageComponent(player);

	_stage->GetMap()->InitViewer(player);
}
