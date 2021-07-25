#pragma once
#include "Enemy.h"
#define BBA_X_SPEED	0.02f
#define BBA_Y_SPEED	0.004f
#define INACTIVE_STATE	-1
#define DEATH_STATE	0
#define WALKING_STATE	1
#define JUMPING_STATE	2
#define CHARGING_STATE	6
#define STOOD_ON_STATE	7
#define DEFENDING_STATE	3
#define BBA_STATE_In_PIPE	11
#define SMALL_ELECTION	4
#define LARGE_ELECTION	5
#define BBA_ANIMATION_SET_ID	19
#define DENFENDING_TIME	1000
#define CHARGING_TIME	1000
#define TIME_TO_START_CHARGING	4000
class SmallBlackBugWithAntena :public Enemy
{
	DWORD defending_timer;
	DWORD charging_timer;
	bool charging = false;
	bool vyIncreasedOnce = false;
	DWORD changingDirectionTimer = GetTickCount();
	bool noReturn = false;
public:
	SmallBlackBugWithAntena(int x, int Y);
	~SmallBlackBugWithAntena();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetActive() { state = JUMPING_STATE; }
	void SetDefending();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsWalking() { return state == WALKING_STATE || state ==JUMPING_STATE; }
	void GetStoodOn();
	void GetReleased();
};

