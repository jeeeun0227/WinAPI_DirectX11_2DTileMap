#include "NPC.h"
#include "ComponentSystem.h"
#include "Map.h"
#include "ComponentMessage.h"
#include "MoveState.h"

NPC::NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename)
	: Character(name, scriptName, textureFilename)
{
	_type = eComponentType::CT_NPC;

	_hp = 100;
}

NPC::~NPC()
{

}

void NPC::UpdateAI()
{
	if (false == _isLive)
		return;

	if (false == _state->IsMoving())
	{
		Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");

		std::vector<eComponentType> compareTypeList;
		compareTypeList.push_back(eComponentType::CT_MONSTER);
		Component *FindEnemy = ComponentSystem::GetInstance()->FindComponentInRange(this, 4, compareTypeList);

		if (NULL != FindEnemy)
		{
			// �߰� ���� ����
			eDirection direction = eDirection::NONE;

			for (int findDir = 0; findDir < 4; findDir++)
			{
				int newTileX = _tileX;
				int newTileY = _tileY;

				switch (findDir)
				{
				case eDirection::LEFT:
					newTileX--;
					break;

				case eDirection::RIGHT:
					newTileX++;
					break;

				case eDirection::UP:
					newTileY--;
					break;

				case eDirection::DOWN:
					newTileY++;
					break;
				}

				if (map->CanMoveTileMap(newTileX, newTileY))
				{
					_currentDirection = (eDirection) findDir;
					_state->Start();
					break;
				}
			}
			_state->Start();
		}

		else
		{
			Character::UpdateAI();
		}
	}
}