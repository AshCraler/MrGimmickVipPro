#pragma once
#include"Enemy.h"
#define GFS_X_SPEED	0.01f
#define GFS_Y_SPEED	0.04f
#define GFS_INACTIVE_STATE	-1
#define GFS_WALKING_STATE	0
#define GFS_JUMPING_STATE	1
#define GFS_LANDING_STATE	2
#define GFS_DEATH_STATE		3
#define GFS_BRAKING	10
#define WALKING_AFTER_ATTACKED_STATE	5
#define JUMPING_AFTER_ATTACKED_STATE	6
#define LANDING_AFTER_ATTACKED_STATE	7
#define GFS_ANIMATION_SET_ID	25
#define GFS_WIDTH	24
#define GFS_HEIGHT	24
#define GFS_MAX_DISTANCE	150
#define SURVIVAL_TIMES	1
#define UNTOUCHABLE_TIME	4000
class GreenFatStuff :public Enemy
{
	enum JumpPoints{
		JP1=879,
		JP2=928,
		JP3=1040,
	};
	int remainingHits;
	DWORD untouchableTimer;
public:
	GreenFatStuff(int x, int y);
	~GreenFatStuff();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void GetHit();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetVX(float x);
	void SetVY(float x);
	void SetX(float v);
	void SetY(float v);
	//void SetVY(float _vy);
};

