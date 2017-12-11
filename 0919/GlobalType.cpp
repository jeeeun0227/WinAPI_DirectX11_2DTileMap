#include "GlobalType.h"

TilePosition GetNextTilePosition(TilePosition currentTilePos, eDirection direction)
{
	TilePosition tilePosition = currentTilePos;
	
	/*
	int newTileX = _character->currentTilePos;
	int newTileY = _character->GetTileY();
	*/

	switch (direction)
	{
	case eDirection::LEFT:
		tilePosition.x--;
		break;

	case eDirection::RIGHT:
		tilePosition.x++;
		break;

	case eDirection::UP:
		tilePosition.y--;
		break;

	case eDirection::DOWN:
		tilePosition.y++;
		break;
	}
	return tilePosition;
}