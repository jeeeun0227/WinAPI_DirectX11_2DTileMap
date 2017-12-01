#pragma once

#include "Character.h"

class MoveState;

class NPC : public Character
{
public:
	NPC(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	~NPC();

	void UpdateAI();
};