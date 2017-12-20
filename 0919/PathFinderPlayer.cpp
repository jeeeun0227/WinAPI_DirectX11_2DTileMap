#include "PathFinderPlayer.h"
#include "IdleState.h"
#include "PathFindingState.h"
#include "PathFindingMoveState.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "PathFindingImmediateState.h"
#include "AttackState.h"

PathFinderPlayer::PathFinderPlayer(std::wstring name, std::wstring scriptName, std::wstring textureFilename) 
	: Player(name, scriptName, textureFilename)
{

}

PathFinderPlayer::~PathFinderPlayer()
{

}

void PathFinderPlayer::UpdateAI()
{
	// 마우스 인풋 처리
	if (GameSystem::GetInstance()->IsMouseDown())
	{
		int mouseX = GameSystem::GetInstance()->GetMouseX();
		int mouseY = GameSystem::GetInstance()->GetMouseY();

		TileCell *targetTileCell = GameSystem::GetInstance()->GetStage()->GetMap()->FindTileCellWithMousePosition(mouseX, mouseY);

		if (NULL != targetTileCell)
		{
			if (targetTileCell->CanMove() == true)		// 장애물은 타겟으로 지정할 수 없게 만든다.
			{
					SetTargetTileCell(targetTileCell);
			}
		}
	}
}

void PathFinderPlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_MOVE, new PathFindingMoveState());
	// ReplaceState(eStateType::ET_PATHFINDING, new PathFindingState());
	ReplaceState(eStateType::ET_PATHFINDING, new PathFindingImmediateState());
}