#pragma once

#include "Player.h"

class PathFinderPlayer : public Player
{
public:
	PathFinderPlayer(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	~PathFinderPlayer();

	// AI
public:
	void UpdateAI();

	// State
	void InitState();
};