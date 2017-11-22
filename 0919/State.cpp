#include <vector>

#include "State.h"
#include "Character.h"
#include "Sprite.h"

State::State()
{
	_nextState = eStateType::ET_NONE;
	_nowState = eStateType::ET_NONE;
}

State::~State()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void State::Init(Character *character)
{
	_character = character;

	CreateSprite();
}

void State::Update(float deltaTime)
{
	_spriteList[(int)_character->GetDirection()]->Update(deltaTime);
}

void State::Stop()
{

}

void State::Start()
{
	_nextState = eStateType::ET_NONE;
}

void State::Render()
{
	_spriteList[(int)_character->GetDirection()]->SetPosition(_character->GetX(), _character->GetY());
	_spriteList[(int)_character->GetDirection()]->Render();
}

void State::Relese()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Relese();
	}
}

void State::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void State::Deinit()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
}

void State::CreateSprite()
{
	_spriteList.clear();

	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];
	wsprintf(textureFileName, L"%s.png", _character->GetTextureFileName().c_str());

	{
		wsprintf(scriptFileName, L"%s_left.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_right.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_up.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_down.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}
}

void State::NextState(eStateType nextStateType)
{
	_nextState = nextStateType;
}
