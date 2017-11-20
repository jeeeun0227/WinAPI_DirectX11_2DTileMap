#include "Texture.h"
#include "GameSystem.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Init(LPCWSTR fileName)
{
	// Texture
	{
		// �̹��� ���Ϸκ��� ���� �ʺ��� ������ �����´�.
		HRESULT hr = D3DXGetImageInfoFromFile(fileName, &_textureInfo);

		if (FAILED(hr))
		{
			MessageBox(0, L"D3DXGetImageInfoFromFile �����Դϴ�.", L"ErrorMessage", 0);
			return;
		}

		hr = D3DXCreateTextureFromFileEx(
			GameSystem::GetInstance()->GetDevice(),
			fileName,
			_textureInfo.Width,
			_textureInfo.Height,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			&_textureInfo,
			NULL,
			&_texture
		);
	}
}

void Texture::Deinit()
{

}

void Texture::Relese()
{
	RELEASE_COM(_texture);
}

LPDIRECT3DTEXTURE9 Texture::GetTexture()
{
	return _texture;
}