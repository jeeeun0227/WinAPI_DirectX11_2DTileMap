#pragma once

#include "State.h"

class Sprite;

class DeadState : public State
{
public:
	DeadState();
	~DeadState();

public:
	void Init(Character *character);
	void Update(float deltaTime);

	void Stop();
	void Start();

	void Render();
	void Relese();
	void Reset();
	void Deinit();

	void CreateSprite();
};