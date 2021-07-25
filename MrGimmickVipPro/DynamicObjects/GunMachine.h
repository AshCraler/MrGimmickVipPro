#pragma once
#pragma once
#include "DynamicObject.h"
#include "Shell.h"
#include "../StaticObjects/Hill.h"
#define SHOTING_SPEED 0.008f
#define MOVING_SPEED	0.01f
#define SHOTING_STATE	1
#define WAITING_STATE	0
#define GM_ANIMATIO_SET_ID	51
#define GM_WIDTH	16
#define GM_HEIGHT	16
#define X_RANGE_TO_ACTIVE	160
#define TIME_GAP	10000
#define ACTIVE_TIME	3000
#define GM_GRAVITY	0.0001f
#define FRICTION_FORCE	0.00001f
#define GM_Y_SPEED	0.01f
#define GM_X_SPEED	0.02f
#define X_SPEED_WHEN_PUSHED	0.005f
class GunMachine :public DynamicObject
{
	vector<Shell*> shells;
	DWORD shottingTimer;
	int onHill=0;
	enum  onHillStages
	{
		OutOfArea = 0,
		Enter = 1,
		Touching = 2,
	};
	Hill* hill;
	int beingPushed = 0;
	 


	bool landed = true;
public:
	GunMachine(int x, int ys);
	~GunMachine();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);

	int isBeingPushed() { return beingPushed; }
	void GetPushed(int dir) { beingPushed = dir; }
	void StopBeingPushed() { beingPushed = 0; }
	int GetX() { return x; }
	bool isLanded() { return landed; }
};

