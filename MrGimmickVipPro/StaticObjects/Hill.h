#pragma once
#include "StaticObject.h"
#define SPEED_REDUCTION_FACTOR	0.5f
class Hill:public StaticObject
{
	bool up;
public:
	void Render();
	bool GetDirection() { return up; }
	float GetSpeedReductionFactor(){ return SPEED_REDUCTION_FACTOR; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	Hill(int x, int y, int w, int h, bool up=true);
	~Hill();
};

