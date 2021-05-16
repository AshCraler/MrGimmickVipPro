#pragma once
#include "StaticObject.h"
#define MOVING_STAIR_SPEED 0.008f
#define MOVING_RANGE	16
class MStair:public StaticObject
{
	int startAtX;
	bool direction;
	vector<LPANIMATION_SET> blocks;

public:
	MStair();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
	float GetSpeed() { return direction ? MOVING_STAIR_SPEED : -MOVING_STAIR_SPEED; }
	MStair(int width, int height);
	MStair(int x, int y, int w=32, int h=16, bool direction=true);
	~MStair();
};

