#pragma once

#include "PathFindingState.h"

class PathFindingImmediateState : public PathFindingState
{
public:
	PathFindingImmediateState();
	~PathFindingImmediateState();

public:
	void Start();
};