#include "GameSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)		// h = handle �� ���� -> 'ID' ��� ��������
{
	GameSystem::GetInstance()->InitSystem(hInstance, nCmdShow);
	return GameSystem::GetInstance()->Update();
}