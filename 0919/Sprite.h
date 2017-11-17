#pragma once

#include <vector>
#include <D3DX9.h>

class Frame;
class Texture;

class Sprite
{
private:
	std::vector<Frame*>_frameList;

	int _currentFrame;

	std::vector<Frame*>::iterator it = _frameList.begin();

	float _frameTime;

	LPCWSTR _textureFileName;
	LPCWSTR _scriptFileName;

public:
	Sprite(LPCWSTR textureFileName, LPCWSTR scriptFileName, float rotate = 0.0f);
	~Sprite();

	void Init();
	void Init(int srcX, int srcY, int width, int height, float framaDelay);
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Relese();
	void Reset();

	void SetPosition(float x, float y);

private:
	Texture *_srcTexture;

	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;

	float _x;
	float _y;

	float _rotate;
};