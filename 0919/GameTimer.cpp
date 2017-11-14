#include <Windows.h>

#include "GameTimer.h"

GameTimer::GameTimer() : _secondPerCount(0.0), _deltaTime(-1.0), _prevCount(0)		// 초기화
{
	// 한번 만들어 두면 변할 일이 없기 떄문에 생성자에서 만들어준다.
	__int64 countPerSecond;		// 1 sec 당 count
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSecond);		// 초당 셀 수 있는 프레임
	_secondPerCount = 1.0 / (double) countPerSecond;
}

GameTimer::~GameTimer()
{

}

void GameTimer::Reset()
{
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
	_prevCount = currentCount;
}

void GameTimer::Update()
{
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);		// 윈도우 명령어
	_deltaTime = (currentCount - _prevCount) * _secondPerCount;
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTime()
{
	return (float)_deltaTime;
}