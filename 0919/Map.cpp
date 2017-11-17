#include <Windows.h>
#include <fstream>

#include "Map.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "GameSystem.h"


Map::Map(LPCWSTR name) : Component(name)
{
	_startX = _startY = _deltaX = _deltaY = 0.0f;
	_spriteList.clear();
	_tileSize = 32;		// +2 �� ���̸� Ÿ�� ������ ����
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
			Sprite *sprite = new Sprite(L"terrain_atlas.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, 32, 32, 1.0f);
			_spriteList.push_back(sprite);
			srcX += 32;
		}
		srcX = 0;
		srcY += 32;
	}

	_width = 32;
	_height = 32;

	// Load Map Script (first floor)
	{
		int line = 0;
		char record[1024 * 8];

		std::ifstream infile("MapData_layer1.csv");

		while (!infile.eof())
		{
			infile.getline(record, 1024 * 8);		// ���� ũ�⿡ ���� �����ϸ� 1024�� ���� �����Ѵ�.

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
						TileCell *tileCell = new TileCell();
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer01_%d_%d", line, x);
						TileObject *tileObject = new TileObject(componentName, _spriteList[index]);
						tileObject->SetCanMove(true);
						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
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

		std::ifstream infile("MapData_layer2.csv");

		while (!infile.eof())
		{
			infile.getline(record, 1024 * 8);		// ���� ũ�⿡ ���� �����ϸ� 1024�� ���� �����Ѵ�.

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

						if (0 <= index)
						{
							TileCell *tileCell = rowList[x];
							WCHAR componentName[256];
							wsprintf(componentName, L"map_layer02_%d_%d", line, x);
							TileObject *tileObject = new TileObject(componentName, _spriteList[index]);
							tileCell->AddComponent(tileObject, true);
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
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->MoveDeltaPosition(_deltaX, _deltaY);
			_tileMap[y][x]->Update(deltaTime);
		}
	}

	if (NULL != _viewer)
	{
		float deltaX = _viewer->GetMoveDeltaX() * deltaTime;
		float deltaY = _viewer->GetMoveDeltaY() * deltaTime;
		Scroll(-deltaX, -deltaY);
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
	// ��ũ�� �ӵ� ����
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