#pragma once
#include "DynamicObject.h"
#define FALLING_SPEED 0.005f
#define GRAVITY	0.00005f
#define SMOKE_STATE	2
#define FALLING_STATE	1
#define WAITING_STATE	0
#define ANIMATIO_SET_ID	50
#define HEIGHT_WHEN_WAITING	/*160*/160
#define HEIGHT_WHEN_FALLING	18
#define WIDTH_WHEN_WAITING	32
#define WIDTH_WHEN_FALLING	14
#define SMOKING_TIME	5000
class HangingMachine:public DynamicObject
{
	DWORD smokeTimer;
public:
	HangingMachine(int x, int y);
	~HangingMachine();
	void Go();
	void GetDestroyed();
	bool isWaiting() { return state == WAITING_STATE; }
	bool isFalling() { return state == FALLING_STATE; }
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
};

