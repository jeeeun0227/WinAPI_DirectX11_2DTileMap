#pragma once

#include "Character.h"

class LifeNpc : public Character
{
public:
	LifeNpc(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~LifeNpc();

public:
	void UpdateAI();
	void UpdateText();
};