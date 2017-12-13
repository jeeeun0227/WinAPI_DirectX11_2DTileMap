#include "StageLoader.h"
#include "Stage.h"
#include "LifeNpc.h"
#include "LifePlayer.h"

StageLoader::StageLoader(Stage *stage)
{
	_stage = stage;
	_lifeNpcCount = 0;
}

StageLoader::~StageLoader()
{

}

Component *StageLoader::CreateLifeNPC(std::wstring scriptName, std::wstring textureFilename)
{
	WCHAR wname[256];
	wsprintf(wname, L"Lifenpc_%d", _lifeNpcCount);
	_lifeNpcCount++;

	Component *component =  new LifeNpc(_name.c_str(), scriptName, textureFilename);
	_stage->AddStageComponent(component);

	return component;
}

void StageLoader::CreateComponents(std::wstring mapName)
{
	Map *map = new Map(mapName.c_str());
	_stage->SetMap(map);
	_stage->AddStageComponent(map);
}