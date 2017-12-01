#pragma once

#include <string>

#include "Player.h"

class LifePlayer : public Player
{
public:
	LifePlayer(std::wstring name, std::wstring scriptName, std::wstring pngName);
	~LifePlayer();

public:
	void InitState();
};