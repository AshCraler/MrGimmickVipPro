#pragma once

#include "StaticObject.h"

#define ELEVATOR_SPEED	0.00001f

#define SPEED_MANIPULATION 0.01f
class Elevator : public StaticObject
{
	bool direction;
	int startAtX;
	vector<LPANIMATION_SET> blocks;
public:
	Elevator();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetSpeed() { return direction?ELEVATOR_SPEED:-ELEVATOR_SPEED; }
	bool GetDirection() { return direction; }
	static float GetSpeedManipulation() { return SPEED_MANIPULATION; }
	Elevator(int width, int height);
	Elevator(int x, int y, int w, int h, bool direction);
	~Elevator();
};