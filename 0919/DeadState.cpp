#include "DeadState.h"
#include "Character.h"
#include "Sprite.h"

DeadState::DeadState()
{
	_nowState = eStateType::ET_DEAD;
}

DeadState::~DeadState()
{

}

void DeadState::Init(Character *character)
{
	State::Init(character);
}

void DeadState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void DeadState::Stop()
{
	State::Stop();
}

void DeadState::Start()
{
	State::Start();
}

void DeadState::Render()
{
	State::Render();
}

void DeadState::Relese()
{
	State::Relese();
}

void DeadState::Reset()
{
	State::Reset();
}

void DeadState::Deinit()
{
	State::Deinit();
}

void DeadState::CreateSprite()
{
	_spriteList.clear();

	WCHAR textureFileName[256];
	WCHAR scriptFileName[256];
	wsprintf(textureFileName, L"%s.png", _character->GetTextureFileName().c_str());

	{
		wsprintf(scriptFileName, L"%s_Dead Left.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName, 1.5f);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_Dead Right.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName, 1.5f);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_Dead Up.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName, 1.5f);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}

	{
		wsprintf(scriptFileName, L"%s_Dead Down.json", _character->GetScriptFileName().c_str());
		Sprite *_sprite = new Sprite(textureFileName, scriptFileName, 1.5f);
		_sprite->Init();
		_spriteList.push_back(_sprite);
	}
}