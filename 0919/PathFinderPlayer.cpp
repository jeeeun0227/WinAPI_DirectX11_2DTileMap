#include "PathFinderPlayer.h"
#include "IdleState.h"
#include "PathFindingState.h"
#include "PathFindingMoveState.h"

PathFinderPlayer::PathFinderPlayer(std::wstring name, std::wstring scriptName, std::wstring textureFilename) 
	: Player(name, scriptName, textureFilename)
{

}

PathFinderPlayer::~PathFinderPlayer()
{

}

void PathFinderPlayer::UpdateAI()
{

}

void PathFinderPlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_MOVE, new PathFindingMoveState());
	ReplaceState(eStateType::ET_PATHFINDING, new PathFindingState());
}