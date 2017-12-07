#include "PathFinderStageLoader.h"
#include "Stage.h"
#include "Map.h"
#include "PathFinderPlayer.h"

PathFinderStageLoader::PathFinderStageLoader(Stage *stage) : StageLoader(stage)
{

}

PathFinderStageLoader::~PathFinderStageLoader()
{

}

void PathFinderStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);
	
	// 1개의 몬스터 생성

	// 1개의 플레이어 생성 ( 추후 길찾기 전용 플레이어 )
	Player *player = new PathFinderPlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);

	// 뷰어를 플레이어가 아닌, 맵 중간에 타일 오브젝트로 세팅
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetWidth() / 2;
	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_stage->GetMap()->InitViewer(comList.front());
}