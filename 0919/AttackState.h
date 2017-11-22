#pragma once

#include <vector>
#include <Windows.h>

#include "State.h"

class Character;
class Sprite;
class Player;
class Component;

class AttackState : public State
{
public:
	AttackState();
	~AttackState();

public:
	void Init(Character *character);
	void Update(float deltaTime);

	void Stop();
	void Start();

	void Render();
	void Relese();
	void Reset();
	void Deinit();

private:
	Player *_player;
};