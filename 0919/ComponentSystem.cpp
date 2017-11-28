#include <stdio.h>

#include "ComponentSystem.h"
#include "Component.h"

ComponentSystem *ComponentSystem::_instance = NULL;

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem()
{

}

ComponentSystem *ComponentSystem::GetInstance()
{
	if (_instance == NULL)
		_instance = new ComponentSystem();
	return _instance;
}

void ComponentSystem::AddComponent(std::wstring  name, Component *component)
{
	if (NULL != component)
	{
		std::map<std::wstring , Component*>::iterator it = _componentMap.find(name);

		if (it == _componentMap.end())
		{
			_componentMap[name] = component;
		}
	}
}

void ComponentSystem::RemoveAllComponents()
{
	for (std::map<std::wstring , Component*>::iterator it = _componentMap.begin();
		it != _componentMap.end();
		it++)
	{
		delete it->second;
	}
	_componentMap.clear();
}

Component *ComponentSystem::FindComponent(std::wstring  name)
{
	std::map<std::wstring , Component*>::iterator it = _componentMap.find(name);

	if (it != _componentMap.end())
	{
		return it->second;
	}
	return 0;
}

Component *ComponentSystem::FindComponentInRange(Component *MapComponent, Component *chaser, int range, std::vector<eComponentType> compareTypeList)
{
	// Map *map = (Map*)FindComponent(L"tileMap");
	Map *map = (Map*)MapComponent;

	int minTileX = chaser->GetTileX() - range;
	int maxTileX = chaser->GetTileX() + range;
	int minTileY = chaser->GetTileY() - range;
	int maxTileY = chaser->GetTileY() + range;

	if (minTileX < 0)
		minTileX = 0;

	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 1;

	if (minTileY < 0)
		minTileY = 0;

	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 1;

	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			std::list<Component*> componentList;
			if (false == map->GetTileCollisonList(x, y, componentList))
			{
				for (std::list<Component*>::iterator it = componentList.begin();
					it != componentList.end(); it++)
				{
					Component *component = (*it);

					if (component->isLive())
					{
						// 탐색 거리에 적이 있는지 확인
						for (int i = 0; i < compareTypeList.size(); i++)
						{
							if (compareTypeList[i] == component->GetType())
							{
								return component;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

void ComponentSystem::SendMsg(const sComponentMsgParam msgParam)
{
	_msgQueue.push(msgParam);
}

void ComponentSystem::ProcessMessageQueue()
{
	while (0 < _msgQueue.size())
	{
		sComponentMsgParam msgParam = _msgQueue.front();
		_msgQueue.pop();
		msgParam.recevier->RaceiveMessage(msgParam);
	}
}

void ComponentSystem::Update(float deltaTime)
{
	ProcessMessageQueue();
}

void ComponentSystem::ClearMessageQueue()
{
	while (0 < _msgQueue.size())
	{
		_msgQueue.pop();
	}
}