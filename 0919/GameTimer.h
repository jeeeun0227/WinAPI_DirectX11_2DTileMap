#pragma once
#include <Windows.h>

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

public:
	void Reset();
	void Update();
	float GetDeltaTime();

private:
	double _deltaTime;		// ������ ��� �ð�
	__int64 _prevCount;
	double _secondPerCount;		// 1 count �� �ð� (sec)
};