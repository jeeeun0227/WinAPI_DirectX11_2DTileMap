#pragma once

#include <D3DX9.h>
#include <vector>
#include <list>

#include "Component.h"

class Sprite;
class TileCell;

class Map : public Component
{
public:
	Map(std::wstring name);
	~Map();

	void Init();
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Relese();
	void Reset();

	void Scroll(float deltaX, float deltaY);

private:
	std::vector<std::vector<TileCell*>> _tileMap;

	int _width;
	int _height;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

	std::vector<Sprite*> _spriteList;

	int _tileSize;

	Component *_viewer;

public:
	int GetPositionX(int tileX, int tileY);
	int GetPositionY(int tileX, int tileY);

	void SetTileComponent(int tileX, int tileY, Component *component, bool isRender);
	void ResetTileComponent(int tileX, int tileY, Component *component);

	bool CanMoveTileMap(int tileX, int tileY);

	void InitViewer(Component *viewer);

	bool GetTileCollisonList(int newTileX, int newTileY, std::list<Component*> &collisonList);

	int GetWidth();
	int GetHeight();

	std::list<Component*> GetTileComponentList(int tileX, int tileY);
};