#include "LifeNpc.h"

LifeNpc::LifeNpc(std::wstring name, std::wstring scriptName, std::wstring textureFilename) :
	Character(name, scriptName, textureFilename)
{
	_type = eComponentType::CT_NPC;
}

LifeNpc::~LifeNpc()
{

}

void LifeNpc::UpdateAI()
{

}

void LifeNpc::UpdateText()
{

}