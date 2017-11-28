#pragma once

#include <windows.h>
#include <list>

class Component;
class Map;

class Stage
{
public:
	Stage();
	~Stage();

public:
	void Init(std::wstring  StageName);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Relese();
	void Reset();

	void CreateLifeNPC(int tileX, int tileY);
	void DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter);

private:
	std::list<Component*> _componentList;

	int _lifeNpcCount;

	// Load Map
private:
	Map *_map;
public:
	Map *GetMap() { return _map; }
};