#include <fstream>
#include <reader.h>		// json ��ũ��Ʈ �Ľ��� �����ش�.

#include "GameSystem.h"
#include "Sprite.h"
#include "Frame.h"
#include "Texture.h"
#include "ResourceManager.h"

Sprite::Sprite(LPCWSTR textureFileName, LPCWSTR scriptFileName)
	:_currentFrame(0), _frameTime(0), _srcTexture(NULL),
	_textureFileName(textureFileName), _scriptFileName(scriptFileName)
{

}

Sprite::~Sprite()
{
	if (NULL != _srcTexture)
	{
		_srcTexture->Deinit();
		delete _srcTexture;
		_srcTexture = NULL;
	}
}

void Sprite::Init()
{
	
	_device3d = GameSystem::GetInstance()->GetDevice();
	_sprite = GameSystem::GetInstance()->GetSprite();
	

	_srcTexture = new Texture();
	_srcTexture->Init(_textureFileName);

	_srcTexture = ResourceManager::GetInstance()->LoadTexture(_textureFileName);

	// json test ( ��ũ��Ʈ ���� �κ� ) , json parsing
	{
		// ���� �о� �鿩��, �ؽ�Ʈ ������
		// �ؽ�Ʈ ������ �Ľ�
		// �Ľ̵� ������ ��ū -> �ǹ��ִ� ���� ���� ��ȯ
		// ��ȯ�� ������ �̿��ؼ� Frame ����

		// char inputBuffer[1000];
		// std::ifstream infile(_scriptFileName);

		// ������ ����� �о� �鿴���� Ȯ��
		// while (!infile.eof())
		std::vector<std::string> scriptTextList = ResourceManager::GetInstance()->LoadScript(_scriptFileName);
		for(int i=0; i<scriptTextList.size(); i++)
		{
			// infile.getline(inputBuffer, 100);
			std::string record = scriptTextList[i];

			Json::Value root;
			Json::Reader reader;
			bool isSuccess = reader.parse(record, root);
			if (isSuccess)
			{
				std::string texture = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();

				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double delay = root["framedelay"].asDouble();

				Frame *frame = new Frame();
				frame->Init(_srcTexture, x, y, width, height, delay);
				_frameList.push_back(frame);
			}
		}
	}

	_currentFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Init(int srcX, int srcY, int width, int height, float framaDelay)
{
	
	_device3d = GameSystem::GetInstance()->GetDevice();
	_sprite = GameSystem::GetInstance()->GetSprite();
	
	_srcTexture = ResourceManager::GetInstance()->LoadTexture(_textureFileName);

	{
		Frame *frame = new Frame();
		frame->Init(_srcTexture, srcX, srcY, width, height, framaDelay);
		_frameList.push_back(frame);
	}

	_currentFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Deinit()
{
	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame *_frame = (*it);
		_frame->Deinit();
		delete _frame;
	}
	_frameList.clear();

	_srcTexture->Deinit();
}

void Sprite::Update(float deltaTime)
{
	// ������ �ð��� ������ �����̸� �Ѿ�� ���� ���������� �Ѿ��, ������ �ð��� �ٽ� ���µȴ�.
	_frameTime += deltaTime;

	if (_frameList[_currentFrame]->GetFrameDelay() <= _frameTime)
	{
		_frameTime = 0.0f;
		_currentFrame = (_currentFrame + 1) % _frameList.size();
	}
}

void Sprite::Render()
{
	if (_currentFrame < _frameList.size())
	{
		_frameList[_currentFrame]->SetPosition(_x, _y);
		_frameList[_currentFrame]->Render();
	}
}

void Sprite::Relese()
{
	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame *_frame = (*it);
		_frame->Relese();
	}

	_srcTexture->Relese();
}

void Sprite::Reset()
{
	Init();

	for (it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame *_frame = (*it);
		_frame->Reset();
	}
}

void Sprite::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}