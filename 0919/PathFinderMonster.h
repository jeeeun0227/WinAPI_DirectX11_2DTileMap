#pragma once

#include "Monster.h"

class PathFinderMonster : public Monster
{
public:
	PathFinderMonster(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	~PathFinderMonster();

public:
	void UpdateAI();
};