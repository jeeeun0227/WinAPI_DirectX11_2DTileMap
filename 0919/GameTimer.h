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
	double _deltaTime;		// 프레임 경과 시간
	__int64 _prevCount;
	double _secondPerCount;		// 1 count 당 시간 (sec)
};