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
	
	// 1개의 몬스터 생성
	Monster *monster = new PathFinderMonster(L"PathFinderMonster", L"monster", L"monster");
	_stage->AddStageComponent(monster);

	// 1개의 플레이어 생성 ( 추후 길찾기 전용 플레이어 )
	Player *player = new PathFinderPlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);

	TileCell *targetCell = _stage->GetMap()->GetTileCell(monster->GetTileX(), monster->GetTileY());
	player->SetTargetTileCell(targetCell);

	// 뷰어를 플레이어가 아닌, 맵 중간에 타일 오브젝트로 세팅
	_midTileX = _stage->GetMap()->GetWidth() / 2;
	_midTileY = _stage->GetMap()->GetWidth() / 2;
	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(_midTileX, _midTileY);
	_stage->GetMap()->InitViewer(comList.front());
}