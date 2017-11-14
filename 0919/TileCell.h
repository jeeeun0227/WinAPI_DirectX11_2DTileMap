#pragma once

#include <list>

class Component;

class TileCell
{
public:
	TileCell();
	~TileCell();

	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Relese();
	void Reset();

	void SetPosition(float PosX, float PosY);

	void MoveDeltaPosition(float _deltaX, float _deltaY);

	float GetPositionX();
	float GetPositionY();

	void AddComponent(Component *component, bool isRender);
	void RemoveComponent(Component *component);

	bool CanMove();

	bool GetTileCollisonList(std::list<Component*> &collisonList);

private:
	float _posX;
	float _posY;

	std::list<Component *>_componentList;
	std::list<Component *>_renderList;
};