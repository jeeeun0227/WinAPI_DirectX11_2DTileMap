#pragma once

#include <string>

class Component;
class Stage;

class Part
{
public:
	Part(Stage *stage);
	~Part();

private:
	std::wstring _name;
	int _lifeNpcCount;
	Stage *_stage;

public:
	Component *CreateLifeNPC(std::wstring scriptName, std::wstring textureFilename);
	void SetName(std::wstring name);
	void CreateComponents(int count);
};