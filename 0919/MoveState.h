#pragma once

class Character;

class MoveState
{
public:
	MoveState();
	~MoveState();

private:
	bool _isMoving;
	float _movimgDuration;
	Character *_character;

public:
	bool IsMoving();
	float GetMovingDuration();
	void Start();
	void Stop();
	void UpdateMove(float deltaTime);
	void Update(float deltaTime);
	void Init(Character *character);
	void SetMoving(bool isMoving);
};