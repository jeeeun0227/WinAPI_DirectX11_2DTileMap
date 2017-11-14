#include "Component.h"
#include "ComponentSystem.h"
#include "ComponentMessage.h"

Component::Component(LPCWSTR name)
{
	_name = name;
	ComponentSystem::GetInstance()->AddComponent(_name, this);
	_canMove = false;

	_moveDistancePerTimeX = _moveDistancePerTimeY = 0.0f;

	_type = eComponentType::CT_NONE;

	_isLive = true;
}

Component::~Component()
{

}

void Component::RaceiveMessage(const sComponentMsgParam msgParam)
{
	if (L"Collision" == msgParam.message)
	{

	}
}