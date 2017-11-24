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
	void Init(std::wstring StageName);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Relese();
	void Reset();

private:
	std::list<Component*> _componentList;

	// Load Map
private:
	Map *_map;
public:
	Map *GetMap() { return _map; }
};