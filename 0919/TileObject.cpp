#include "TileObject.h"
#include "Sprite.h"

TileObject::TileObject(LPCWSTR name, Sprite *sprite) : Component(name)
{
	_sprite = sprite;
	_posX = _posY = 0.0f;

	_type = eComponentType::CT_TILE_OBJECT;
}

TileObject::~TileObject()
{

}

void TileObject::Init()
{

}

void TileObject::Deinit()
{
	_sprite->Deinit();
}

void TileObject::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void TileObject::Render()
{
	_sprite->SetPosition(_posX, _posY);
	_sprite->Render();
}

void TileObject::Relese()
{
	_sprite->Relese();
}

void TileObject::Reset()
{
	_sprite->Reset();
}

void TileObject::MoveDeltaPosition(float _deltaX, float _deltaY)
{
	_posX += _deltaX;
	_posY += _deltaY;
}

void TileObject::SetPosition(float PosX, float PosY)
{
	_posX = PosX;
	_posY = PosY;
}