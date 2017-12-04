#include "Part.h"
#include "Stage.h"
#include "LifeNpc.h"
#include "LifePlayer.h"

Part::Part(Stage *stage)
{
	_stage = stage;
	_lifeNpcCount = 0;
}

Part::~Part()
{

}

Component *Part::CreateLifeNPC(std::wstring scriptName, std::wstring textureFilename)
{
	SetName(L"Lifenpc_%d");
	_lifeNpcCount++;
	Component *component =  new LifeNpc(_name.c_str(), L"npc", L"Npc_Sprite_01");
	_stage->AddStageComponent(component);

	return component;
}

void Part::SetName(std::wstring name)
{
	WCHAR wname[256];
	wsprintf(wname, L"Lifenpc_%d", name.c_str(), _lifeNpcCount);
	_name = wname;
}

void Part::CreateComponents(int count)
{
	for (int i = 0; i < count; i++)
	{
		Component *component = CreateLifeNPC(L"npc", L"Npc_Sprite_01");
	}
	LifePlayer *player = new LifePlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);
}