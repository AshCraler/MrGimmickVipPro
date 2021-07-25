#pragma once
#include "StaticObject.h"
#define SPEED_REDUCTION_FACTOR	0.5f
class Hill:public StaticObject
{

	bool leftUpToRight;
public:
	void Render();
	bool isLeftUpToRight() { return leftUpToRight; }
	float GetSpeedReductionFactor(){ return SPEED_REDUCTION_FACTOR; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetXToYRatio();
	Hill(int x, int y, int w, int h, bool leftUpToRight=true);
	~Hill();
};

