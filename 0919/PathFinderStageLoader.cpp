#include "PathFinderStageLoader.h"
#include "Stage.h"
#include "Map.h"
#include "Monster.h"
#include "PathFinderPlayer.h"
#include "PathFinderMonster.h"
#include "TileCell.h"
#include "Component.h"

PathFinderStageLoader::PathFinderStageLoader(Stage *stage) 
	: StageLoader(stage)
{
	_midTileX = NULL;
	_midTileY = NULL;
}

PathFinderStageLoader::~PathFinderStageLoader()
{

}

void PathFinderStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);
	
	// 1���� ���� ����
	Monster *monster = new PathFinderMonster(L"PathFinderMonster", L"monster", L"monster");
	_stage->AddStageComponent(monster);

	// 1���� �÷��̾� ���� ( ���� ��ã�� ���� �÷��̾� )
	Player *player = new PathFinderPlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);

	TileCell *targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);

	// �� �÷��̾ �ƴ�, �� �߰��� Ÿ�� ������Ʈ�� ����
	_midTileX = _stage->GetMap()->GetWidth() / 2;
	_midTileY = _stage->GetMap()->GetWidth() / 2;
	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(_midTileX, _midTileY);
	_stage->GetMap()->InitViewer(comList.front());
}