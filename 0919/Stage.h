#pragma once

#include <windows.h>
#include <list>
#include <map>

#include "TileCell.h"

class Component;
class Map;
class StageLoader;

class Stage
{
private:
	std::list<Component*> _componentList;

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

	// StageLoader
private:
	std::map<std::wstring, StageLoader*> _loaderMap;
	StageLoader *_loader;

public:
	void AddStageComponent(Component *component);
	StageLoader *GetStageLoader(std::wstring name);

	// Load Map
private:
	Map *_map;

public:
	Map *GetMap() { return _map; }
	void SetMap(Map *map) { _map = map; }

private:
	std::list<Component*> _removeComponentList;
	std::list<Component*> _createBaseComponentList;

public:
	void CreateLifeNPC(Component *component);
	void DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter);

	void UpdateRemoveComponentList();
	void UpdateBaseComponentList();
	void CheckDestroyLifeNPC(Component *component);


	// Path Finding
public:
	void CreatePathFindingNPC(TileCell *tileCell);
};