#pragma once

#include <D3DX9.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	void Init(LPCWSTR fileName);

	void Deinit();
	void Relese();

	LPDIRECT3DTEXTURE9 GetTexture();

private:
	LPDIRECT3DTEXTURE9 _texture;
	D3DXIMAGE_INFO _textureInfo;
};