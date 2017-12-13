#pragma once

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

typedef struct TilePosition
{
	int x;
	int y;
	eDirection to;
	eDirection from;
};

TilePosition GetNextTilePosition(TilePosition currentTilePos, eDirection direction);

TilePosition GetDirection(TilePosition startTile, TilePosition destinationTile);

TilePosition _direction;