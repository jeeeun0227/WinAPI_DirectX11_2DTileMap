#include <stdio.h>

#include "ResourceManager.h"

using namespace std;

ResourceManager *ResourceManager::_instance = NULL;

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{
	for (std::map<wstring, Texture*>::iterator it = _textureMap.begin(); it != _textureMap.end(); it++)
	{
		_textureMap.erase((*it).first);
		(*it).second->Deinit();
		delete (*it).second;
	}
	_textureMap.clear();
}

ResourceManager *ResourceManager::GetInstance()
{
	if (_instance == NULL)
		_instance = new ResourceManager();
	return _instance;
}

Texture* ResourceManager::LoadTexture(const std::wstring textureFileName)
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

vector<string> ResourceManager::LoadScript(std::wstring  scriptFileName)
{
	map <std::wstring , vector<string>>::iterator it = _scriptMap.find(scriptFileName);

	if (it != _scriptMap.end())
	{
		return it->second;
	}

	vector<string> scriptTextList;

	char record[1100];

	ifstream infile(scriptFileName);

	while (!infile.eof())
	{
		infile.getline(record, 100);
		scriptTextList.push_back(record);
	}

	_scriptMap[scriptFileName] = scriptTextList;

	return scriptTextList;
}