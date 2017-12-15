#pragma once

#include <list>

class Component;

class TileCell
{
public:
	TileCell(int tileX, int tileY);
	~TileCell(); 

	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Relese();
	void Reset();

	void SetPosition(float PosX, float PosY);

	void MoveDeltaPosition(float _deltaX, float _deltaY);

	float GetPositionX();
	float GetPositionY();

	void AddComponent(Component *component, bool isRender);
	void RemoveComponent(Component *component);

	bool CanMove();

	bool GetTileCollisonList(std::list<Component*> &collisonList);

	std::list<Component*> GetComponentList() { return _componentList; }

private:
	float _posX;
	float _posY;

	std::list<Component *>_componentList;
	std::list<Component *>_renderList;

	// PathFinding
private:
	bool _isPathFindingMark;
	TileCell *_prevPathFindingCell;

	float _distanceFromStart;		// 계산된 값을 저장하기 위한 변수
	float _distanceWeight;		// 고정된 값, 맵을 로딩할때 세팅한다.

	int _tileX;
	int _tileY;

	float _heuristic;

public:
	void InitPathFinding();
	bool IsPathFindingMark() { return _isPathFindingMark; }
	void PathFinded() { _isPathFindingMark = true; }

	TileCell *GetPrevPathFindingCell() { return _prevPathFindingCell; }
	void SetPrevPathFindingCell(TileCell *tileCell) { _prevPathFindingCell = tileCell; }

	int GetTileX() { return _tileX; }
	int GetTileY() { return _tileY; }

	float GetDistanceFromStart() { return _distanceFromStart; }
	float GetDistanceWeight() { return _distanceWeight; }
	void SetDistanceFromStart(float distance) { _distanceFromStart = distance; }

	void SetHeuristic(float heuristic) { _heuristic = heuristic; }
	float GetHeuristic() { return _heuristic; }
};