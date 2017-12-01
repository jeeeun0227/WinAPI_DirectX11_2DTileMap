#pragma once

#include "Character.h"
#include "Map.h"

class MoveState;

class Player : public Character
{
public:
	Player(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	~Player();

	// AI
public:
	void UpdateAI();

public:
	Component *Collision(std::list<Component*> &collisonList);
};