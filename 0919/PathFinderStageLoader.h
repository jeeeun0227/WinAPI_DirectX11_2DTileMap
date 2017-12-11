#pragma once

#include "StageLoader.h"

class PathFinderStageLoader : public StageLoader
{
public:
	PathFinderStageLoader(Stage *stage);
	~PathFinderStageLoader();

public:
	void CreateComponents(std::wstring mapName);

private:
	int _midTileX;
	int _midTileY;
};