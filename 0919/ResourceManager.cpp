#include <stdio.h>

#include "ResourceManager.h"

using namespace std;

ResourceManager *ResourceManager::_instance = NULL;

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{

}

ResourceManager *ResourceManager::GetInstance()
{
	if (_instance == NULL)
		_instance = new ResourceManager();
	return _instance;
}

Texture* ResourceManager::LoadTexture(std::wstring textureFileName)
{
	std::map<std::wstring, Texture*>::iterator it = _textureMap.find(textureFileName);

	if (it != _textureMap.end())
	{
		return it->second;
	}

	Texture *texture = new Texture();

	texture->Init(textureFileName.c_str());

	_textureMap[textureFileName] = texture;

	return texture;
}

vector<string> ResourceManager::LoadScript(std::wstring scriptFileName)
{
	map <std::wstring, vector<string>>::iterator it = _scriptMap.find(scriptFileName);

	if (it != _scriptMap.end())
	{
		return it->second;
	}

	vector<string> scriptTextList;

	char record[1000];

	ifstream infile(scriptFileName);

	while (!infile.eof())
	{
		infile.getline(record, 100);
		scriptTextList.push_back(record);
	}

	_scriptMap[scriptFileName] = scriptTextList;

	return scriptTextList;
}