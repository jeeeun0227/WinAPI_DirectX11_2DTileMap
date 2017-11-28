#pragma once

#include "Character.h"

class MoveState;

class NPC : public Character
{
public:
	NPC(std::wstring name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~NPC();

	void UpdateAI();
};