#include "LifeTileObject.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Component.h"

#include <stdio.h>
#include <vector>

LifeTileObject::LifeTileObject(int tileX, int tileY, std::wstring name, Sprite *sprite) :
	TileObject(name, sprite)
{
	_tileX = tileX;
	_tileY = tileY;
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

	// ���� ���ϱ� ( ���� ������ )
	int minTileX = _tileX - range;
	int maxTileX = _tileX + range;
	int minTileY = _tileY - range;
	int maxTileY = _tileY + range;

	// map�� ����� �ʰ�
	if (minTileX < 0)
		minTileX = 0;

	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 1;

	if (minTileY < 0)
		minTileY = 0;

	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 1;

	int surroundedCharacter = 0;

	Component *tileCharacter = NULL;

	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			if (x != _tileX || y != _tileY)
			{
				std::list<Component*> componentList;

				if (false == map->GetTileCollisonList(x, y, componentList))
				{
					for (std::list<Component*>::iterator it = componentList.begin();
						it != componentList.end(); it++)
					{
						Component *component = (*it);

						switch (component->GetType())
						{
						case eComponentType::CT_NPC:
						case eComponentType::CT_PLAYER:
							surroundedCharacter++;
							break;
						default:
							break;
						}
					}
				}
			}
			else
			{
				std::list<Component*> componentList;

				if (false == map->GetTileCollisonList(x, y, componentList))
				{
					for (std::list<Component*>::iterator it = componentList.begin();
						it != componentList.end(); it++)
					{
						Component *component = (*it);

						switch (component->GetType())
						{
						case eComponentType::CT_NPC:
						case eComponentType::CT_PLAYER:
							tileCharacter = component;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	if (3 == surroundedCharacter)
	{
		if (NULL == tileCharacter)
		{
			GameSystem::GetInstance()->GetStage()->CreateLifeNPC(this);
		}
	}
	else if (2 == surroundedCharacter)
	{
		// skip
	}
	else
	{
		// dead
		if (NULL != tileCharacter)
		{
			if (eComponentType::CT_PLAYER != tileCharacter->GetType())
			{
				GameSystem::GetInstance()->GetStage()->CheckDestroyLifeNPC(tileCharacter);
				tileCharacter = NULL;
			}
		}
	}
}