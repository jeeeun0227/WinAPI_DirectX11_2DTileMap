#include <stdio.h>

#include "ResourceManager.h"

using namespace std;

// 헤더에서 선언한 정적 멤버들을 다시 선언해줘야만, 일반 멤버 변수처럼 사용할 수 있다.
ResourceManager *ResourceManager::_instance = NULL;		// 정적 변수 -> 메모리를 하나만 잡는다.

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{

}

ResourceManager *ResourceManager::GetInstance()		// 정적 함수 -> 메모리를 하나만 잡는다.
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