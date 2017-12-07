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
	
	// 1���� ���� ����

	// 1���� �÷��̾� ���� ( ���� ��ã�� ���� �÷��̾� )
	Player *player = new PathFinderPlayer(L"player", L"player", L"Player_Sprite_00");
	_stage->AddStageComponent(player);

	// �� �÷��̾ �ƴ�, �� �߰��� Ÿ�� ������Ʈ�� ����
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetWidth() / 2;
	std::list<Component*> comList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);
	_stage->GetMap()->InitViewer(comList.front());
}