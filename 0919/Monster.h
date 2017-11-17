#pragma once

#include "Character.h"
#include "Component.h"
#include "Map.h"
#include "ComponentSystem.h"

#include <list>

class ComponentSystem;
class MoveState;

class Monster : public Character
{
public:
	Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~Monster();

	void UpdateAI();

	Component *Collision(std::list<Component*> &collisonList);
};