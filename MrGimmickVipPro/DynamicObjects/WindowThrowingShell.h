#pragma once
#include"DynamicObject.h"
#define WTS_FALLING_SPEED 0.008f
#define NUMBER_OF_ANIMATIONS	2
#define WTS_ANIMATIO_SET_ID	-15
#define WTS_WAITING_STATE	3
#define WTS_FALLING_STATE	0
#define WTS_HITTING_TARGET	1
#define WTS_GOING_TO_WATER	2
#define WTS_WIDTH 16
#define WTS_HEIGHT 16
#define FALLING_RANGE	155
#define EXPLODING_TIME	1000
#define WATER_TIME	1000
//#define SHELL_GRAVITY 0.0001f
//#define SHELL_X_RANGE	10
class WindowThrowingShell :public DynamicObject
{
	int y_Start_Falling;
	DWORD explodeAt, hitWaterAt;
public:
	enum ShellTypes {
		rolling = 1,
		flying = 2,
	};
	WindowThrowingShell(int x, int y);
	~WindowThrowingShell();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void ThrowShell();
	void Explode();
	bool IsWaiting() {
		return state == WTS_WAITING_STATE;
	}
	bool IsFalling(){ return state == WTS_FALLING_STATE; }
};
