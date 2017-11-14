#pragma once

#include "Character.h"
#include "Map.h"

class MoveState;

class Player : public Character
{
public:
	Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~Player();

	// AI
public:
	void UpdateAI();
};