#pragma once
#include "DynamicObject.h"
#define BOAT_ANIMATIO_SET_ID	54
#define WAITING_STATE	1
#define STOPPED_STATE	3
#define	MOVING_STATE	2
#define ANIMATION_OFFSET	0
#define BOAT_WIDTH	45
#define BOAT_HEIGHT	15
#define BOAT_X_SPEED	0.005f
class Boat :public DynamicObject
{
	int xTo;
public:
	Boat(int x, int y, int xT);
	~Boat();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
	void Go();
	bool IsWaiting() { return state == WAITING_STATE; }
};



