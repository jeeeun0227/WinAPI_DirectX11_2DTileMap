#pragma once

#include <D3DX9.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "Texture.h"

class ResourceManager 
{
private:
	static ResourceManager *_instance;

public:
	static ResourceManager *GetInstance();

private:
	ResourceManager();

public:
	~ResourceManager();

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