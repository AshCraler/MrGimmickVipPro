#pragma once
#include "Boss.h"
#include "../../Gimmick.h"
#define BS_ANIMATION_SET_ID	12
#define SWORD_ANIMATION_SET_ID	-12
#define BS_INACTIVE_STATE	-1
#define BS_WALKING_STATE	0
#define BS_JUMPING_STATE	1
#define BS_ATTACKING_STATE	2
#define BS_BEING_INJURED	3
#define BS_DEFEATED	4
#define BS_SWORD	5
#define BS_WIDTH	33
#define BS_HEIGHT	33
#define BS_MAX_DISTANCE_TO_GM	112
#define BS_MAX_DISTANCE_TO_JUMP	16
#define BS_X_SPEED	0.006f
#define BS_X_FAST_SPEED	0.0145f
#define BS_Y_SPEED	0.05f
#define UNTOUCHABLE_TIME	2000
#define ATTACKING_TIME	1000
#define WAIT_FOR_NEXT_ATTACK	5000
#define TIME_TO_DECIDE_NEXT_JUMP	5000
#define BS_SURVIVING_TIMES	2
#define SWORD_TIME_TO_SWITCH_SPRITE	300
#define SWORD_REBELLING_SPEED	0.01f
class Sword :public CGameObject {
	bool collidedWithGround = false;
	
	DWORD switchingSpriteTimer, timeToRelease;
public:
	Sword(int x, int y);
	~Sword();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
class BrownStuff:public Boss
{
	DWORD untouchableTimer;
	DWORD attackingTimer, waitToAttack;
	DWORD decideToJumpTimer;
	int remainingHit;
	Sword* sword;

public:
	BrownStuff(int x, int y);
	~BrownStuff();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit();
	bool ProcessStars(Star* s, float nx, float ny);
};

