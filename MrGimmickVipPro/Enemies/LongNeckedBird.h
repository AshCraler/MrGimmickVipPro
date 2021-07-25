#pragma once
#include"Enemy.h"
#define X_SPEED	0.004f
#define Y_SPEED	0.004f
#define LNB_STANDING_STATE	0
#define READY_STATE		1
#define FLYING_STATE	2
#define LNB_ANIMATION_SET_ID	24
#define X_RANGE	112
#define WIDTH_STANDING	16
#define HEIGHT_STANDING	22
#define MARGIN_TO_TOP_WHEN_STANDING	16
#define WIDTH_FLYING	16
#define HEIGHT_FLYING	16
#define	MARGIN_TO_TOP_WHEN_FLYING	14
#define	MARGIN_TO_LEFT_WHEN_FLYING	9
#define TIME_TO_BE_READY	10000
#define READY_TIME	1000
#define X_TO_FLYING_RIGHT	336
#define Y_TO_FLYING_RIGHT	65
#define LNB_X_SPEED	0.01f
#define LNB_Y_SPEED	0.01f
#define LNB_X_PUSHED_SPEED	0.002f
#define FLYING_RANGE	512
#define X_TO_FLYING_LEFT	1664
#define Y_TO_FLYING_LEFT	90
#define FLYING_RANGE_OF_LEFT	55

class LongNeckedBird :public Enemy
{
	DWORD timeToBeReady, readyTimer;
	bool disappear = false, beingPushed=false;
	bool goDown = true;
public:
	LongNeckedBird(int x, int y, bool right = FALSE);
	~LongNeckedBird();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void GetPushed() { beingPushed = true; vx = LNB_X_PUSHED_SPEED; }
	bool IsWaiting() { return state != FLYING_STATE; }
};

