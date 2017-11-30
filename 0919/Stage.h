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

	void CreateLifeNPC(Component *component);
	void DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter);
	void CheckDestroyLifeNPC(Component *component);
	void UpdateRemoveComponentList();
	void UpdateBaseComponentList();

private:
	std::list<Component*> _componentList;
	std::list<Component*> _removeComponentList;
	std::list<Component*> _createBaseComponentList;

	int _lifeNpcCount;

	// Load Map
private:
	Map *_map;
public:
	Map *GetMap() { return _map; }
};