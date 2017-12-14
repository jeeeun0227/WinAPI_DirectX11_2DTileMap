#include "LifeStageLoader.h"
#include "LifePlayer.h"

LifeStageLoader::LifeStageLoader(Stage *stage) : StageLoader(stage)
{

}

LifeStageLoader::~LifeStageLoader()
{

}

void LifeStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);

	for (int i = 0; i < 100; i++)
	{
		CreateLifeNPC(L"npc", L"Npc_Sprite_00");
	}

	LifePlayer* player = new LifePlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);

	_stage->GetMap()->InitViewer(player);
}