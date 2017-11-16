#pragma once

#include <vector>
#include <Windows.h>

class Character;
class Sprite;

class State
{
public:
	State();
	~State();

protected:
	Character *_character;
	std::vector<Sprite*> _spriteList;

public:
	virtual void Init(Character *character);
	virtual void Update(float deltaTime);

	virtual void Stop();
	virtual void Start();

	virtual void Render();
	virtual void Relese();
	virtual void Reset();
	virtual void Deinit();
};