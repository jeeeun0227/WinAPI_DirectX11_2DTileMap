#pragma once

#include "Component.h"

class Component;

struct sComponentMsgParam
{
	Component *sender;
	int attackPoint;

	Component *recevier;
	std::wstring message;
};