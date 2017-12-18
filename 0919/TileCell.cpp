#include "TileCell.h"
#include "Sprite.h"
#include "Component.h"
#include "TileObject.h"

TileCell::TileCell(int tileX, int tileY)
{
	_posX = _posY = 0.0f;
	_componentList.clear();

	_tileX = tileX;
	_tileY = tileY;

	_distanceWeight = 1.0f;
}

TileCell::~TileCell()
{

}

void TileCell::Deinit()
{

}

void TileCell::Update(float deltaTime)
{
	for (std::list<Component *>::iterator it = _renderList.begin();
		it != _renderList.end();
		it++)
	{
		(*it)->Update(deltaTime);
	}
}

void TileCell::Render()
{
	for (std::list<Component *>::iterator it =_renderList.begin();
		it != _renderList.end();
		it++)
	{
		(*it)->Render();
	}
}

void TileCell::Relese()
{

}

void TileCell::Reset()
{

}

void TileCell::SetPosition(float PosX, float PosY)
{
	_posX = PosX;
	_posY = PosY;

	for (std::list<Component *>::iterator it = _componentList.begin();
		it != _componentList.end();
		it++)
	{
		(*it)->SetPosition(PosX, PosY);
	}
}

void  TileCell::MoveDeltaPosition(float _deltaX, float _deltaY)
{
	_posX += _deltaX;
	_posY += _deltaY;

	for (std::list<Component *>::iterator it = _componentList.begin();
		it != _componentList.end();
		it++)
	{
		(*it)->MoveDeltaPosition(_deltaX, _deltaY);
	}
}

float TileCell::GetPositionX()
{
	return _posX;
}

float TileCell::GetPositionY()
{
	return _posY;
}

void TileCell::AddComponent(Component *component, bool isRender)
{
	_componentList.push_back(component);

	if (eComponentType::CT_TILE_OBJECT == component->GetType())
	{
		_distanceWeight = ((TileObject*)component)->GetDistanceWeight();
	}

	if (isRender)
	{
		_renderList.push_back(component);
	}
}

void TileCell::RemoveComponent(Component *component)
{
	_componentList.remove(component);
	_renderList.remove(component);
}

bool TileCell::CanMove()
{
	for (std::list<Component *>::iterator it = _componentList.begin();
		it != _componentList.end();
		it++)
	{
		if (false == (*it)->CanMove())
			return false;
	}
	return true;
}

bool TileCell::GetTileCollisonList(std::list<Component*> &collisonList)
{
	collisonList.clear();

	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		if (false == (*it)->CanMove())
		{
			collisonList.push_back(*it);
		}
	}

	if (0 == collisonList.size())
		return true;
	else
		return false;
}

void TileCell::InitPathFinding()
{
	_isPathFindingMark = false;
	_prevPathFindingCell = NULL;

	_distanceFromStart = 0.0f;
	_distanceWeight = 1.0f;
	_heuristic = 0.0f;
}