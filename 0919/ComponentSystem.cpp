#include <stdio.h>

#include "ComponentSystem.h"
#include "Component.h"

ComponentSystem *ComponentSystem::_instance = NULL;

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();		// 초기화 중요
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

void ComponentSystem::AddComponent(std::wstring name, Component *component)
{
	if (NULL != component)
	{
		std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);

		if (it == _componentMap.end())
		{
			_componentMap[name] = component;
		}
	}
}

void ComponentSystem::RemoveAllComponents()
{
	for (std::map<std::wstring, Component*>::iterator it = _componentMap.begin();
		it != _componentMap.end();
		it++)
	{
		delete it->second;
	}
	_componentMap.clear();
}

Component *ComponentSystem::FindComponent(std::wstring name)
{
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);

	if (it != _componentMap.end())
	{
		return it->second;
	}
	return 0;
}

Component *ComponentSystem::FindComponentInRange(Component *chaser, int range, std::vector<eComponentType> compareTypeList)
{
	/*
	< 구현 알고리즘 >
	1. 'Player'를 기준으로 주변 타일 검사 범위(+, -)를 정한다.
	2. 범위 내 모든 타일을 검사하여 'Npc / Player'가 있으면 적을 찾았다.
	3. 찾지 못하였으면 평소 캐릭터 움직임대로 움직인다.
	*/

	Map *map = (Map*)FindComponent(L"tileMap");

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