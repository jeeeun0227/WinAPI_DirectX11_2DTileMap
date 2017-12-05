#pragma once

#include <string>

class Component;
class Stage;

class StageLoader
{
public:
	StageLoader(Stage *stage);
	~StageLoader();

protected:
	Stage *_stage;

	std::wstring _name;
	
	int _lifeNpcCount;

public:
	virtual void CreateComponents(std::wstring mapName);

	Component *CreateLifeNPC(std::wstring scriptName, std::wstring textureFilename);
};