#pragma once

#include <windows.h>
#include <String>
#include <map>
#include <vector>
#include <queue>

#include "Map.h"
#include "ComponentMessage.h"

class Component;
struct sComponentMsgParam;

class ComponentSystem
{
	// Singleton
private:
	static ComponentSystem *_instance;

public:
	static ComponentSystem *GetInstance();

private:
	ComponentSystem();

	// Component Map
private:
	std::map<std::wstring , Component*> _componentMap;

public:
	~ComponentSystem();

public:
	void AddComponent(std::wstring  name, Component *component);
	void RemoveAllComponents();

	Component *FindComponent(std::wstring  name);

	void SendMsg(const sComponentMsgParam msgParam);

	// FindComponent
public:
	Component *ComponentSystem::FindComponentInRange(Component *MapComponent, Component *chaser, int range, std::vector<eComponentType> compareTypeList);

private:
	std::queue<sComponentMsgParam> _msgQueue;

public:
	void ProcessMessageQueue();
	void Update(float deltaTime);

	void ClearMessageQueue();

	void RemoveComponent(Component *component);
};