#pragma once

#include <D3DX9.h>

class Texture;

class Frame
{
public:
	Frame();
	~Frame();

	void Init(Texture *texture, int x, int y, int width, int height, float rotate, float frameDelay);
	void Deinit();
	void Render();
	void Relese();
	void Reset();

	float GetFrameDelay();

	void SetPosition(float x, float y);

private:
	LPD3DXSPRITE _sprite;

	// Texture
	Texture *_texture;
	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

	D3DXIMAGE_INFO _textureInfo;

	int _width;
	int _height;

	float _frameDelay;

	float _x;
	float _y;

	float _rotate;
};