#pragma once

#include "Character.h"

class LifeNpc : public Character
{
public:
	LifeNpc(std::wstring name, std::wstring scriptName, std::wstring textureFilename);
	~LifeNpc();

public:
	void UpdateAI();
	void UpdateText();
};