#pragma once

#include <string>

#include "Component.h"

class Sprite;

class RecoveryItem : public Component
{
public:
	RecoveryItem(LPCWSTR name, LPCWSTR scriptFileName, LPCWSTR textureFileName);
	~RecoveryItem();

public:
	void Init();
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Relese();
	void Reset();

	void MoveDeltaPosition(float _deltaX, float _deltaY);
	void SetPosition(float PosX, float PosY);

private:
	Sprite *_sprite;
	float _posX;
	float _posY;

	std::wstring _textureFileName;
	std::wstring _scriptFileName;
};