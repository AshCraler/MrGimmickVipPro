#ifndef SBB_HEADER_H
#define SBB_HEADER_H
#include"Enemy.h"
#include "../StaticObjects/Hill.h"
#include "../StaticObjects/Pipe.h"

#include "../Items/Jar.h"
#define SBB_X_SPEED	0.02f
#define SBB_Y_SPEED	0.03f
#define SBB_VX_WHEN_HIT	0.02f
#define SBB_VY_WHEM_HIT	0.02f
#define DEATH_STATE	0
#define WALKING_STATE	1
#define JUMPING_STATE	2
#define STANDING_STATE	3
#define SBB_IN_PIPE_STATE	4
#define INACTIVE_STATE	10
#define ANIMATION_SET_ID	18
#define WIDTH	16
#define HEIGHT	16

#define SSB_STANDING_STATE	3
#define SSB_DEATH_STATE	0
#define SSB_X_SPEED	0.04f
#define SSB_Y_SPEED	0.04f
#define SSB_ANIMATION_SET_ID	18

#define BCB_MAX_DISTANCE_TO_GIMMICK	50
#define BCB_WALKING_STATE	1
#define BCB_DEATH_STATE	0
#define BCB_X_SPEED	0.04f
#define BCB_Y_SPEED	0.04f
#define BCB_ANIMATION_SET_ID	18
#define TIME_TO_CHANGE_DIRECTION	3000

class StandingStillBug :public Enemy {
public:
	StandingStillBug(int x, int y);
	~StandingStillBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit(bool direction = true);
	//bool IsDeath() { return state == BCB_DEATH_STATE; }
};
//class BombOnBug :public DynamicObject {
//
//};
class BombCaryingBug :public Enemy {
	int xFrom, xTo;
	Jar* jar = NULL;

	//OnBugHeadShell* shell;
public:
	BombCaryingBug(int x, int y, int xFrom, int xto, Jar* j);
	~BombCaryingBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit(bool direction = true);
	bool IsDeath() { return state == BCB_DEATH_STATE; }
	//void GetShell(OnBugHeadShell* s) { shell = s; }

};
class SmallBlackBug:public Enemy
{	
	enum  ClimbingHillStages
	{
		OutOfArea =0,
		Enter =1,
		Touching=2,
	};
	bool moving=false;
	int onHill = 0;
	Hill* hill;
	bool noReturn = false;
	bool carryingGimmick = false;
	Jar* jar = NULL;
	DWORD changingDirectionTimer = GetTickCount();
	enum PipeTypes {
		top_right = 12,
		top_bottom = 13,
		top_left = 14,
		right_bottom = 23,
		right_left = 24,
		bottom_left = 34,
	};
	bool toX;
public:
	SmallBlackBug(int aniId, int x, int y, bool moving = true, Jar* jar = NULL, bool toX = false);
	~SmallBlackBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);

	void GoDead(bool shotDirection);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetVX(float vx);
	void SetVY(float vy);
	void SetX(float x);
	void SetY(float y);
	void SetCarringGimmick() { carryingGimmick = true; }
	void StopCarringGimmick() { carryingGimmick = false; }
	void GetHit(bool dir = true);
};

#endif