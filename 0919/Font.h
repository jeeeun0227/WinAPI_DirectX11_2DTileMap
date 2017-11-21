#pragma once

#include <D3DX9.h>
#include <string>

class Font
{
public:
	Font(std::wstring FontName, int FontSize, D3DCOLOR color);
	~Font();

public:
	void Render();

	void SetRect(int x, int y, int width, int height);
	void SetText(std::wstring text);

	void SetPosition(int x, int y);

private:
	ID3DXFont *_dxFont;

	D3DCOLOR _color;
	// RECT _rect;
	std::wstring _text;

	int _x;
	int _y;
	int _width;
	int _height;
};