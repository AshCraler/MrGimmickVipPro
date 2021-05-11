#pragma once
#include "GameObject.h"
class CPlayer : public CGameObject
{
public:
	virtual void Load(LPCWSTR simonFile) = 0;
};

