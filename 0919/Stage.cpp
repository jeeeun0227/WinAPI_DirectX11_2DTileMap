#include "Stage.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "LifeNpc.h"
#include "LifePlayer.h"
#include "StageLoader.h"
#include "DefaultStageLoader.h"
#include "LifeStageLoader.h"

Stage::Stage()
{
	_loader = NULL;
}

Stage::~Stage()
{
	if (NULL != _loader)
	{
		delete _loader;
		_loader = NULL;
	}

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}

	ComponentSystem::GetInstance()->RemoveAllComponents();

	delete _loader;
}

void Stage::Init(std::wstring mapName)
{
	_loaderMap[L"Default"] = new DefaultStageLoader(this);
	_loaderMap[L"Map3"] = new LifeStageLoader(this);
	_loaderMap[L"Map4"] = new PathFinderStageLoader(this);

	_componentList.clear();

	_loader = GetStageLoader(mapName);
	_loader->CreateComponents(mapName);

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Init();
	}
}

void Stage::Deinit()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Deinit();
	}
}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Update(deltaTime);
	}

	UpdateBaseComponentList();
	UpdateRemoveComponentList();
}

void Stage::Render()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Render();
	}
}

void Stage::Relese()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Relese();
	}
}

void Stage::Reset()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Reset();
	}
}

void Stage::CreateLifeNPC(Component *component)
{
	_createBaseComponentList.push_back(component);
}

void Stage::DestoryLifeNPC(int tileX, int tileY, Component *tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);

	_componentList.remove(tileCharacter);
	ComponentSystem::GetInstance()->RemoveComponent(tileCharacter);
}

void Stage::CheckDestroyLifeNPC(Component *component)
{
	_removeComponentList.push_back(component);
}

void Stage::UpdateRemoveComponentList()
{
	for (std::list<Component*>::iterator it = _removeComponentList.begin(); it != _removeComponentList.end(); it++)
	{
		Component *com = (*it);
		DestoryLifeNPC(com->GetTileX(), com->GetTileY(), com);
	}
	_removeComponentList.clear();
}

void Stage::UpdateBaseComponentList()
{
	for (std::list<Component*>::iterator it = _createBaseComponentList.begin(); it != _createBaseComponentList.end(); it++)
	{
		Component *baseComponent = (*it);
		
		LifeNpc *npc = (LifeNpc*)(_loader->CreateLifeNPC(L"npc", L"Npc_Sprite_01"));

		npc->Init(baseComponent->GetTileX(), baseComponent->GetTileY());
	}
	_createBaseComponentList.clear();
}

void Stage::AddStageComponent(Component *component)
{
	_componentList.push_back(component);
}

StageLoader *Stage::GetStageLoader(std::wstring name)
{
	std::map<std::wstring, StageLoader*>::iterator it = _loaderMap.find(name);

	if (it != _loaderMap.end())
	{
		return it->second;
	}
	return _loaderMap[L"Default"];
}
