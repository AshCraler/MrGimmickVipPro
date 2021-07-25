#pragma once

#include "DynamicObject.h"
#define MOVING_SPEED 0.008f
#define RELEASED_STATE	3
#define RELEASING_STATE	2
#define RUNNING_STATE	1
#define WAITING_STATE	0
#define ANIMATIO_SET_ID	55
#define MARGIN_TO_Y_STAND	62	
#define MARGIN_TO_X_STAND	16
#define HEIGHT	8
#define RUNNING_SPEED	0.005f
#define RUNNING_RANGE	64
#define RELEASING_TIME	700

class HangingElevator :public DynamicObject
{
	int markedX = 0;
	DWORD releasingTimer;
	int rolledX = 0;
	int wheelCount = 0;
public:
	HangingElevator(int x, int y);
	~HangingElevator();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
	void Go();
	int GetX() { return x; }
	int GetY() { return y; }
	int GetStart(){ 
		if (state != RELEASED_STATE)
			return x - MARGIN_TO_X_STAND;
		else return 0;
	}
	int GetEnd() {
		if (state != RELEASED_STATE)
			return x +8+ MARGIN_TO_X_STAND;
		else return 0;
	}
};


