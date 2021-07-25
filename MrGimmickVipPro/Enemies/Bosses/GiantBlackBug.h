#pragma once


#include "Boss.h"
#define GBB_X_SPEED	0.003f
#define GBB_X_WHEN_ATTACKED	0.015f
#define X_FRICTION	0.00001f
#define GBB_Y_WHEN_ATTACKED	0.01f
#define GBB_Y_SPEED	0.004f
#define INACTIVE_STATE	-1
#define GBB_DEATH_STATE	4
#define GBB_WALKING_STATE	2
#define GBB_BEING_ATTACKED_STATE	1
#define GBB_ABOUT_TO_ATTACK_STATE	3
#define GBB_STANDING_STATE 0
#define DEFENDING_STATE	3
#define SMALL_ELECTION	4
#define LARGE_ELECTION	5
#define GBB_ANIMATION_SET_ID	10
#define GBB_WIDTH 32
#define GBB_HEIGHT	32
#define WALKING_TIME	2000
#define STANDING_TIME	5000
#define STANDING_TIME_BEFORE_WALKING	2000
#define PREPARING_TIME	3000
#define ANTENA_WHEN_WALKING	5
#define ANTENA_WHEN_STANDING	8
#define GBB_SURVIVAL_TIMES	3

#define SB_WIDTH	16
#define SB_HEIGHT	16
#define SB_FALLING_STATE	2
#define SB_ANIMATION_SET_ID	18
#define SB_DEATH_STATE	0
#define SB_WALKING_STATE	1
#define SB_X_SPEED	0.03f
#define SB_Y_SPEED	0.03f
#define NUMBER_OF_BUGS	6
class SmallBug :public Enemy {
public:
	SmallBug(float x, float y,float vx, float vy);
	~SmallBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit(bool direction = false);
	//bool IsDeath() { return state == BCB_DEATH_STATE; }
};
class GiantBlackBug :public Boss
{/*
	DWORD defending_timer;
	DWORD charging_timer;
	bool charging = false;
	bool vyIncreasedOnce = false;*/
	int xFrom, xTo;
	DWORD walkingTime;
	DWORD standingTime;
	DWORD preparingTime, defeatedWhen;
	DWORD dumb;
	bool attacking = false;
	bool touchedGroundWhenAttacked = false;
	vector<SmallBug*> bugs;
	int remainingHits = GBB_SURVIVAL_TIMES;
	float bugs_vx[NUMBER_OF_BUGS] = { 0.01f, 0.015f, 0.02f, 0.025f, 0.03f, 0.035f };
	float bugs_vy[NUMBER_OF_BUGS] = {-0.020f, -0.018f, -0.016f, -0.014f, -0.012f, -0.01f};
public:
	GiantBlackBug(int x, int y, int xFrom, int xTo);
	~GiantBlackBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	/*void SetActive() { state = JUMPING_STATE; }
	void SetDefending();*/
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	/*bool IsWalking() { return state == WALKING_STATE || state == JUMPING_STATE; }
	void GetStoodOn();
	void GetReleased();*/
	void SentFlying();
	vector<SmallBug*> GetBugs() { return bugs; }
};

