#pragma once
#include "DynamicObject.h"

#define WGM_X_SPEED	0.004f
#define WGM_ANIMATION_SET_ID	53
#define FS_ANIMATION_SET_ID	52
#define X_WHEEL_RANGE	5
#define STOPPING_TIME	1000
#define MAX_DISTANCE_TO_GM	150
#define WGM_WIDTH	24
#define WGM_HEIGHT	28
#define FS_WIDTH	12
#define FS_HEIGHT	12
#define FS_VX	0.01f
#define FS_VY_REFLECTED	0.004f
#define FS_GOING_STRAIGHT_STATE	0
#define FS_FALLING_STATE	1
class FlyingShell :public DynamicObject {
public:
	FlyingShell(int x, int y);
	~FlyingShell();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};


class WheelGunMachine :public DynamicObject
{
	bool beingPushed = false;
	int wheel = 0;
	int markedX;
	DWORD timeToShot;
	vector<FlyingShell*> shells;
	bool fallOnce = false;
public:
	WheelGunMachine(int x, int y);
	~WheelGunMachine();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void GetPushed(bool dir);
	vector<FlyingShell*> GetShells() { return shells; }
};

