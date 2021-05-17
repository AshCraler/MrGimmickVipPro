#pragma once
#include "../GameObject.h"
class Item:public CGameObject
{
protected:
	int type;
	bool taken;
	string category = "ITEM";
public:
	int GetType() { return type; }
	bool IsTaken() { return taken; }
	void SetTaken(bool taken) { this->taken = taken; x = y = width = height = 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

