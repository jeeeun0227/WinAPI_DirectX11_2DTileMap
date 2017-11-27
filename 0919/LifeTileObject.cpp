#include "LifeTileObject.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Component.h"

#include <stdio.h>
#include <vector>

LifeTileObject::LifeTileObject(LPCWSTR name, Sprite *sprite) :
	TileObject(name, sprite)
{

}

LifeTileObject::~LifeTileObject()
{

}

void LifeTileObject::Update(float deltaTime)
{
	TileObject::Update(deltaTime);

	// (���� ������) �ֺ� 8ĭ�� Ž���Ͽ� ĳ����(NPC, MONSTER)�� ��� �ִ��� Ȯ��
	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

	int range = 1;

	int minTileX = GetTileX() - range;
	int maxTileX = GetTileX() + range;
	int minTileY = GetTileY() - range;
	int maxTileY = GetTileY() + range;

	if (minTileX < 0)
		minTileX = 0;

	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 8;

	if (minTileY < 0)
		minTileY = 0;

	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 8;

	std::list<Component*> componentList;
	std::vector<eComponentType> compareTypeList;

	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			if (false == map->GetTileCollisonList(x, y, componentList))
			{
				for (std::list<Component*>::iterator it = componentList.begin();
					it != componentList.end(); it++)
				{
					Component *component = (*it);

					if (component->isLive())
					{
						// Ž�� �Ÿ��� ���� �ִ��� Ȯ��
						for (int i = 0; i <compareTypeList.size(); i++)
						{
							if (compareTypeList[i] == component->GetType())
							{
								// return component;
							}
						}
					}
				}
			}
		}
	}
	// return NULL;
}