#pragma once

#include "State.h"

class Sprite;

class DefenseState : public State
{
public:
	DefenseState();
	~DefenseState();

public:
	void Init(Character *character);
	void Update(float deltaTime);

	void Stop();
	void Start();

	void Render();
	void Relese();
	void Reset();
	void Deinit();
};