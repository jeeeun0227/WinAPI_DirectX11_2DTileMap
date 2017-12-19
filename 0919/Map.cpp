#include <Windows.h>
#include <fstream>

#include "Map.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "GameSystem.h"
#include "LifeTileObject.h"

Map::Map(std::wstring name) : Component(name)
{
	_startX = _startY = _deltaX = _deltaY = 0.0f;
	_spriteList.clear();
	_tileSize = 32 - 2;		// +2 를 붙이면 타일 간격이 생성
}

Map::~Map()
{

}

void Map::Init()
{
	int srcX = 0;
	int srcY = 0;

	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			Sprite *sprite = new Sprite(L"Map_Sprite_00.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, 32, 32, 1.0f);
			_spriteList.push_back(sprite);
			srcX += 32;
		}
		srcX = 0;
		srcY += 32;
	}

	_width = 41;		// 맵의 가로 수치 ( 스크립트와 일치해야 한다. )
	_height = 26;		// 맵의 세로 수치 ( 스크립트와 일치해야 한다. )

	std::wstring  wname = _name;
	std::string name = "";
	name.assign(wname.begin(), wname.end());

	char layer01Name[256];
	sprintf(layer01Name, "%s_MapData_layer1.csv", name.c_str());

	char layer02Name[256];
	sprintf(layer02Name, "%s_MapData_layer2.csv", name.c_str());

	// Load Map Script (first floor)
	{
		int line = 0;
		int row = 0;
		char record[1024 * 8];

		std::ifstream infile(layer01Name);

		while (!infile.eof())
		{
			infile.getline(record, 1024 * 8);		// 맵의 크기에 따라 증가하면 1024의 값도 증가한다.

			char *token = strtok(record, ",");

			switch (line)
			{
			case 0:
				if (NULL != token)
				{
					token = strtok(NULL, ",");
					_width = atoi(token);
					token = strtok(NULL, ",");
					_height = atoi(token);
				}
				break;

			case 1:
				break;

			default:
				if (NULL != token)
				{
					std::vector<TileCell*> rowList;
					for (int x = 0; x < _width; x++)
					{
						int index = atoi(token);
						TileCell *tileCell = new TileCell(x, row);
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer01_%d_%d", line, x);
						TileObject *tileObject = new TileObject(componentName, _spriteList[index], x, row);
						
						// for test
						switch (index)
						{
						case 70:		// 속도 가중 타일
							// 1.5f;
							tileObject->SetDistanceWeight(1.5f);
							break;
						}

						tileObject->SetCanMove(true);
						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
					row++;
				}
				break;
			}
			line++;
		}
	}

	// Load Map Script (second floor)
	{
		int line = 0;
		int row = 0;
		int index = 0;
		char record[1024 * 8];

		std::ifstream infile(layer02Name);

		while (!infile.eof())
		{
			infile.getline(record, 1024 * 8);		// 맵의 크기에 따라 증가하면 1024의 값도 증가한다.

			char *token = strtok(record, ",");

			switch (line)
			{
			case 0:
				break;
			case 1:
				break;
			default:
				// map data
				if (NULL != token)
				{
					std::vector<TileCell*> rowList = _tileMap[row];
					for (int x = 0; x < _width; x++)
					{
						index = atoi(token);

						TileCell *tileCell = rowList[x];
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer02_%d_%d", line, x);
						if (0 <= index)
						{
							if (100100 == index)
							{
								LifeTileObject *tileObject = new LifeTileObject(x, row, componentName, _spriteList[235]);
								tileObject->SetCanMove(true);
								tileCell->AddComponent(tileObject, true);
							}
							else
							{
								TileObject *tileObject = new TileObject(componentName, _spriteList[index], x, row);
								tileCell->AddComponent(tileObject, true);
							}
						}
						token = strtok(NULL, ",");
					}
					row++;
				}
				break;
			}
			line++;
		}
	}
}

void Map::Deinit()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Deinit();
			delete _tileMap[y][x];
		}
	}
}

void Map::Update(float deltaTime)
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	float posX = _startX;
	float posY = _startY;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			_tileMap[y][x]->Update(deltaTime);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}

void Map::Render()
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 2;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 2;
	int maxY = _viewer->GetTileY() + (midX / _tileSize) + 2;

	if (minX < 0)
		minX = 0;

	if (_width <= maxX)
		maxX = _width;

	if (minY < 0)
		minY = 0;

	if (_height <= maxY)
		maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			_tileMap[y][x]->Render();
		}
	}
}

void Map::Relese()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Relese();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Reset();
		}
	}
}

void Map::Scroll(float deltaX, float deltaY)
{
	// 스크롤 속도 조정
	_deltaX = deltaX;
	_deltaY = deltaY;
}

int Map::GetPositionX(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionX();
}

int Map::GetPositionY(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionY();
}

void Map::SetTileComponent(int tileX, int tileY, Component *component, bool isRender)
{
	_tileMap[tileY][tileX]->AddComponent(component, isRender);
}

void Map::ResetTileComponent(int tileX, int tileY, Component *component)
{
	_tileMap[tileY][tileX]->RemoveComponent(component);
}

bool Map::CanMoveTileMap(int tileX, int tileY)
{
	if (tileX < 0)
		return false;

	if (_width <= tileX)
		return false;

	if (tileY < 0)
		return false;

	if (_height <= tileY)
		return false;

	return _tileMap[tileY][tileX]->CanMove();
}

bool Map::CanMoveTileMap(TilePosition nextTilePos)
{
	return CanMoveTileMap(nextTilePos.x, nextTilePos.y);
}

void Map::InitViewer(Component *viewer)
{
	_viewer = viewer;

	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;
	
	float posX = _startX;
	float posY = _startY;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}

bool Map::GetTileCollisonList(int newTileX, int newTileY, std::list<Component*> &collisonList)
{
	if (_width <= newTileX)
		return false;
	if (newTileX < 0)
		return false;
	if (_height <= newTileY)
		return false;
	if (newTileY < 0)
		return false;

	return _tileMap[newTileY][newTileX]->GetTileCollisonList(collisonList);
}

int Map::GetWidth()
{
	return _width;
}

int Map::GetHeight()
{
	return _height;
}

std::list<Component*> Map::GetTileComponentList(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetComponentList();
}

TileCell *Map::GetTileCell(int tileX, int tileY)
{
	return _tileMap[tileY][tileX];
}

TileCell *Map::GetTileCell(TilePosition nextTilePos)
{
	return GetTileCell(nextTilePos.x, nextTilePos.y);
}

TileCell *Map::FindTileCellWithMousePosition(int mouseX, int mouseY)
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 2;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 2;
	int maxY = _viewer->GetTileY() + (midX / _tileSize) + 2;

	if (minX < 0)
		minX = 0;

	if (_width <= maxX)
		maxX = _width;

	if (minY < 0)
		minY = 0;

	if (_height <= maxY)
		maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			RECT rect;
			rect.left = _tileMap[y][x]->GetPositionX() - _tileSize / 2.0f;
			rect.right = rect.left + _tileSize;
			rect.top = _tileMap[y][x]->GetPositionY() - _tileSize / 2.0f;
			rect.bottom = rect.top + _tileSize;

			if (rect.left <= mouseX && mouseX <= rect.right &&
				rect.top <= mouseY && mouseY <= rect.bottom)
			{
				return _tileMap[y][x];
			}
		}
	}
	return NULL;
}