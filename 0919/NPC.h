#pragma once

#include "Character.h"

class MoveState;

class NPC : public Character
{
public:
	NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~NPC();

	void UpdateAI();
};