#include "LifePlayer.h"
#include "IdleState.h"

LifePlayer::LifePlayer(std::wstring name, std::wstring scriptName, std::wstring pngName) : Player(name, scriptName, pngName)
{

}

LifePlayer::~LifePlayer()
{

}

void LifePlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_ATTACK, new IdleState());
}