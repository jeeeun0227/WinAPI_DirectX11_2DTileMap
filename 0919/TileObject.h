#pragma once

#include "Component.h"

class Sprite;

class TileObject : public Component
{
public:
	TileObject(std::wstring name, Sprite *sprite, int tileX, int tileY);
	virtual ~TileObject();

public:
	void Init();
	void Deinit();

	virtual void Update(float deltaTime);
	void Render();

	void Relese();
	void Reset();

	void MoveDeltaPosition(float _deltaX, float _deltaY);
	void SetPosition(float PosX, float PosY);

private:
	Sprite *_sprite;
	float _posX; 
	float _posY;
};