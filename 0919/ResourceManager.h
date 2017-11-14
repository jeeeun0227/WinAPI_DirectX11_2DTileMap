#pragma once

#include <D3DX9.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "Texture.h"		// include > 전방선언 (but. 헤더파일에 include 하는 것은 좋지 않다.)

class ResourceManager 
{
	// Singleton -> 한번만 생성되도록 하는 기능을 보장
private:
	static ResourceManager *_instance;

public:
	static ResourceManager *GetInstance();

private:
	ResourceManager();		// 다른 곳에서 실수로 생성하지 않도록, 생성자를 private로 만든다.

public:
	~ResourceManager();

	// Singleton 끝

	// Texture
private:
	std::map<std::wstring, Texture*> _textureMap;

public:
	Texture *LoadTexture(std::wstring textureFileName);

	// Script
private:
	std::map<std::wstring, std::vector<std::string>> _scriptMap;
public:
	std::vector<std::string> LoadScript(std::wstring scriptFileName);
};