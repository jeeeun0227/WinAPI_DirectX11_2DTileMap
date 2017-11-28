#include "LifeNpc.h"

LifeNpc::LifeNpc(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) :
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