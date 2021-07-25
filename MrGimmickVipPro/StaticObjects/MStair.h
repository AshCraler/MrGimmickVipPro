#pragma once
#include "StaticObject.h"
#define MOVING_STAIR_SPEED 0.008f
#define MOVING_RANGE	16
#define ANIMATIO_SET_ID	5300
class MStair:public StaticObject
{
	int startAtX=0;
	int startAtY=0;
	bool direction;
	vector<LPANIMATION_SET> blocks;
	bool circle = false;
	float radius = 0;
	float distance = 32;
	float centerX, centerY;
public:
	MStair();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
	float GetSpeed() { return direction ? MOVING_STAIR_SPEED : -MOVING_STAIR_SPEED; }
	int GetStartingX() { return startAtY; }
	int GetStartingY() { return startAtY; }
	MStair(int width, int height);
	MStair(int x, int y, int w=32, int h=16, bool direction=true, bool circle =false);
	~MStair();
};

