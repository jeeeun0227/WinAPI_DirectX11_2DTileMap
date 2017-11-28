#include "RecoveryItem.h"
#include "Sprite.h"
#include "ComponentSystem.h"
#include "Character.h"
#include "GameSystem.h"
#include "Map.h"
#include "Stage.h"

RecoveryItem::RecoveryItem(std::wstring name, LPCWSTR scriptFileName, LPCWSTR textureFileName) : Component(name)
{
	// _posX = _posY = 0.0f;
	_type = eComponentType::CT_ITEM;
	_sprite = NULL;

	_textureFileName = textureFileName;
	_scriptFileName = scriptFileName;
}

RecoveryItem::~RecoveryItem()
{

}

void RecoveryItem::Init()
{
	// Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");		// L"tileMap"
	Map *map = GameSystem::GetInstance()->GetStage()->GetMap();

	_tileX = rand() % (map->GetWidth() - 1) + 1;
	_tileY = rand() % (map->GetHeight() - 1) + 1;

	while (!map->CanMoveTileMap(_tileX, _tileY))
	{
		_tileX = rand() % map->GetWidth();
		_tileY = rand() % map->GetHeight();
	}

	_posX = map->GetPositionX(_tileX, _tileY);
	_posY = map->GetPositionY(_tileX, _tileY);
	map->SetTileComponent(_tileX, _tileY, this, true);

	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];
	wsprintf(textureFileName, L"%s.png", _textureFileName.c_str());

	{
		wsprintf(scriptFileName, L"%s.json", _scriptFileName.c_str());
		_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
	}

	_canMove = true;
}

void RecoveryItem::Deinit()
{
	_sprite->Deinit();
}

void RecoveryItem::Update(float deltaTime)
{
	if (false == _isLive)
		return;

	_sprite->Update(deltaTime);
}

void RecoveryItem::Render()
{
	if (false == _isLive)
		return;

	_sprite->SetPosition(_posX, _posY);
	_sprite->Render();
}

void RecoveryItem::Relese()
{
	_sprite->Relese();
}

void RecoveryItem::Reset()
{
	_sprite->Reset();
}

void RecoveryItem::MoveDeltaPosition(float _deltaX, float _deltaY)
{
	_posX += _deltaX;
	_posY += _deltaY;
}

void RecoveryItem::SetPosition(float PosX, float PosY)
{
	_posX = PosX;
	_posY = PosY;
}

void RecoveryItem::RaceiveMessage(const sComponentMsgParam msgParam)
{
	if (L"Use" == msgParam.message)
	{
		Component *sender = msgParam.sender;

		switch (sender->GetType())
		{
		case eComponentType::CT_NPC:
		case eComponentType::CT_MONSTER:
		case eComponentType::CT_PLAYER:
			((Character*)sender)->IncreaseHP(500);
			// Map *map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"tileMap");
			Map *map = GameSystem::GetInstance()->GetStage()->GetMap();
			map->ResetTileComponent(_tileX, _tileY, this);
			_isLive = false;
			break;
		}
	}
}