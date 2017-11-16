#include <time.h>
#include "GameSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)		// h = handle 의 약자 -> 'ID' 라고 생각하자
{
	srand(time(NULL));

	GameSystem::GetInstance()->InitSystem(hInstance, nCmdShow);
	return GameSystem::GetInstance()->Update();
}