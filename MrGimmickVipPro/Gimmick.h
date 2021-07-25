#ifndef GIMMICK_HEADER_H
#define GIMMICK_HEADER_H
#include "GameObject.h"
#include "Player.h"
#include "Utils.h"
#include "Game.h"
#include "DynamicObjects/Boat.h"
#include "DynamicObjects/WindowThrowingShell.h"
#include "DynamicObjects/WheelGunMachine.h"

#include "DynamicObjects/HangingMachine.h"
#include "DynamicObjects/HangingElevator.h"
#include "StaticObjects/MStair.h"
#include "StaticObjects/Hill.h"
#include "StaticObjects/Pipe.h"
#include "Enemies/SmallBlackBug.h"
#include "Enemies/SmallBlackBugWithAntena.h"
#include "Enemies/Bosses/GiantBlackBug.h"
#include "Enemies/LongNeckedBird.h"
#include "Enemies/Enemy.h"

#include "DynamicObjects/GunMachine.h"
#include "Enemies/GreenFatStuff.h"
//SPEED

#define GIMMICK_WALKING_SPEED		0.02f //0.1f
#define GIMMICK_JUMP_SPEED_Y		0.05f
#define GIMMICK_JUMP_DEFLECT_SPEED	0.2f
#define GIMMICK_GRAVITY				0.0001f
#define GIMMICK_FRICTION_FORCE		0.001f
#define GIMMICK_DIE_DEFLECT_SPEED	0.5f
#define GIMMICK_CRASH_Y_SPEED		-0.02f
#define GIMMICK_CRASH_X_SPEED		0.02f
//STATE
#define GIMMICK_STATE_IDLE	1
#define GIMMICK_STATE_WALKING_RIGHT	2
#define GIMMICK_STATE_WALKING_LEFT	102
#define GIMMICK_STATE_JUMP			3
#define GIMMICK_STATE_DIE			6
#define GIMMICK_STATE_FORMING_STAR	10
#define GIMMICK_STATE_IN_PIPE		11
#define GIMMICK_STATE_UP			12
#define GIMMICK_STATE_DOWN		13
//ANI
#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT		6
#define GIMMICK_ANI_WALKING_RIGHT	1
#define GIMMICK_ANI_WALKING_LEFT	7
#define GIMMICK_ANI_JUMPING_RIGHT	2
#define GIMMICK_ANI_JUMPING_LEFT	8
#define GIMMICK_ANI_IN_PIPE_RIGHT	3
#define GIMMICK_ANI_IN_PIPE_LEFT	9
#define GIMMICK_ANI_FAINTED_RIGHT	4
#define GIMMICK_ANI_DIE				106

#define INJURED_BY_ENEMY	-1.0f
#define INJURED_BY_OTHER	-0.5f
#define GET_JAR	1
//TIME
#define GIMMICK_UNTOUCHABLE_TIME	5000
#define GIMMICK_FORMING_STAR_PHASE_2	1000

#define GIMMICK_INJURED_TIME	15000
#define GIMMICK_FAINTED_TIME	4000
#define GIMMICK_STATE_INJURED	5
#define GIMMICK_VX_WHEN_INJURED	0.02f
#define GIMMICK_VY_WHEN_INJURED	0.02f

#define SCORE_INSCREASED_BY	2
#define STAR_WIDTH	16
#define STAR_HEIGHT	16

#define STAR_X_MARGIN	0
#define STAR_Y_MARGIN	25
#define STAR_TIME_DELAY	25

#define STAR_VX 0.008f
#define STAR_VY 0.01f

#define STAR_EXISTING_TIME	5000
#define STAR_SMOKE_TIME	1000
#define TIME_TO_PUSH_GM 1000
#define STAR_DISTANCE	5
#define STAR_SPEED_REDUCTION_FACTOR	0.8f

#define BOMB_VX					0.02f
#define BOMB_VY					0.02f
#define BOMB_ANIMATION_SET_ID	4
#define BOMB_SMOKE_STATE		0
#define BOMB_BOMB_STATE			1
#define BOMB_STAR				2
#define BOMB_SMALLER_STAR		3
#define BOMB_SMOKE_TIME			2000
#define BOMB_SPREADING_SPEED	0.02f
#define BOMB_STARS_BOUNDARY		24 
#define BOMB_SMALLER_STAR_TIME	2000
#define BOMB_NUMBER_OF_STARS	9
#define FIRE_ANIMATION_SET_ID	7
#define FIRE_BIG_BALL			0
#define FIRE_SMALL_FLAME		1
#define FIRE_VX					0.02f
#define FIRE_SMALL_FLAME_TIME	1000

#define KILL_ENEMY	30
#define GET_BIG_PRIZE	3000
#define GET_BOMB	2
#define GET_FIRE	3
#define GET_LIQUID	1
#define INJURED_BY_ENEMY	-1.0f
#define INJURE_BY_OTHERS	-0.5f

#define STARS_START_AT	60
#define RADIUS_SPEED	2.0f
#define DISTANCE_SPEED	0.03f
class CFire :public CGameObject {
public:
	bool gone = false;
	CFire();/*
	void SetVX(float vx);
	void SetVY(float vy);
	void SetXY(int x, int y);*/
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL, bool updateXY=false);
	CFire(int x, int y);
	~CFire();
};
class Star :public CGameObject {
public:
	int prev_x, prev_y;
	bool destroyed = false;
	bool inHillArea = false;
	Hill* hill;

	Star();
	void SetVX(float vx);
	void SetVY(float vy);
	void SetXY(int x, int y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetDestroyed() { destroyed = true; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL, bool updateXY = true);
	void SignalGimmickToDestroyStar();
	Star(int x, int y, Hill* hill);
	~Star();
};
class CBomb :public CGameObject {
public:
	bool destroyed = false;
	enum DestroyedPhase {
		SpreadingStars=1,
		SmallerStars=2,
	};
	bool gone = false;
	int times;
	DWORD smallerStarTimer;
	double toRadius = 3.14159 / 180;
	int destroyed_phase = SpreadingStars;
	float Xs[BOMB_NUMBER_OF_STARS] = { 0,0,0,0,0,0,0,0,0};
	float Ys[BOMB_NUMBER_OF_STARS] = { 0,0,0,0,0,0,0,0,0};
	double VXs[BOMB_NUMBER_OF_STARS] = { cos(90 * toRadius), cos(50 * toRadius) ,cos(10 * toRadius) ,cos(-30 * toRadius) ,cos(-70 * toRadius),
					-cos(50 * toRadius) ,-cos(10 * toRadius) ,-cos(-30 * toRadius) ,-cos(-70 * toRadius), };
	double VYs[BOMB_NUMBER_OF_STARS] = { sin(90 * toRadius), sin(50 * toRadius) ,sin(10 * toRadius) ,sin(-30 * toRadius) ,sin(-70 * toRadius),
				sin(50 * toRadius) ,sin(10 * toRadius) ,sin(-30 * toRadius) ,sin(-70 * toRadius), };

	CBomb();/*
	void SetVX(float vx);
	void SetVY(float vy);
	void SetXY(int x, int y);*/
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetDestroyed();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL, bool updateXY = true);
	CBomb(int x, int y);
	~CBomb();
};
class CGimmick : public CPlayer
{
public:
	//chua xu ly toi
	//int level;
	int untouchable;
	DWORD untouchable_start;
	
	enum whatToForm {
		Stars = 1,
		Bomb = 2,
		Fire = 3,
	};

	enum BombPhases {
		NotExist = 0,
		Forming = 1,
		Smoke = 2,
		OnHead = 3,
		Thrown = 4,
		SpreadingStars = 5
	};

	enum FirePhases {
		FireNotExist = 0,
		FireForming = 1,
		FireSmallFlame = 2,
		FireOnHead = 3,
		FireThrown = 4, 
	};
	int whatToForm = Stars;
	int bomb_phase = NotExist;
	CBomb* bomb=NULL;
	DWORD bomb_timer;

	int fire_phase = NotExist;
	CFire* fire = NULL;
	DWORD fire_timer;
	
	float radius = 0;
	float distance = STARS_START_AT;

	const int x1[10] = { -1,-7,-9,-10,-10,-9,-7,-5,-3,-1 };
	const int y1[10] = { -61,-50,-41,-32,-24,-17,-11,-6,-3,0 };
	const int x2[10] = { 0,1,3,7,12,18,26,35,44,56 };
	const int y2[10] = { -1,-4,-7,-10,-14,-18,-21,-23,-26,-24 };
	const int x3[10] = { 0,-1,-3,-7,-12,-18,-26,-35,-44,-56 };
	const int y3[10] = { -1,-4,-7,-10,-14,-18,-21,-23,-26,-24 };
	const int x4[10] = { 0,2,6,11,17,23,29,36,42,47 };
	const int y4[10] = { 0,0,1,2,4,8,12,19,26,37 };
	const int x5[10] = { -0,-2,-6,-11,-17,-23,-29,-36,-42,-47 };
	const int y5[10] = { 0,0,1,2,4,8,12,19,26,37 };


	vector<Star> stars;
	int count = 0, delay = 0;
	DWORD stars_timer, star_1, star_2;
	int star_phase = 0;
	bool aboutToDestroyStar = false;
	float X1, Y1, X2, Y2, radius1, radius2, distance2, distance1, distance0, radius0;

	//

	// elevator
	int standingOnElevator = 0;

	// moving stair
	float start_x;
	float start_y;
	bool standingOnMovingStair = false;
	float eFrom, eTo, sFrom, sTo, deltaXToMStair, deltaYToMStair;
	MStair* stair = NULL;

	// hill
	/*bool standingOnHill = false;
	bool inHillArea = false;*/
	GunMachine* gmachine = NULL;
	DWORD gm_timer;
	DWORD gm_timer_small;

	int climbingHill = 0;
	Hill* hill = NULL;

	// hanging elevator
	HangingElevator* he;
	float deltaHE_x;
	float deltaHE_y;
	bool standingOnHangingElevator = false;

	enum  ClimbingHillStages
	{
		OutOfArea = 0,
		Enter = 1,
		Climbing = 2,
	};

	//pipe
	Pipe* pipe;
	enum PipeTypes {
		top_right = 12,
		top_bottom = 13,
		top_left = 14,
		right_bottom = 23,
		right_left = 24,
		bottom_left = 34,
	};


	// Small bug
	SmallBlackBug* sbb;
	bool standingOnSBB = false;
	float deltaSBB_x;
	float deltaSBB_y;

	// BBA
	bool standingOnBBA = false;
	SmallBlackBugWithAntena* bba;


	//Boat
	bool standingOnBoat = false;
	Boat* boat;
	float deltaBoat_x;
	float deltaBoat_y;

	//LNB
	bool standingOnBird = false;
	LongNeckedBird* bird;
	float deltaBird_x;
	float deltaBird_y;

	bool standingOnShell = false;
	FlyingShell* shell;
	float deltaShell_x;
	float deltaShell_y;

	// WGM
	bool standingOnWGM = false;
	WheelGunMachine* wgm;
	float deltaWGM_x;
	float deltaWGM_y;

	// GFS
	bool standingOnGFS = false;
	GreenFatStuff* gfs;
	float deltaGFS_x;
	float deltaGFS_y;

	//
	bool standingOnStar = false;
	Star* str;
	float deltaSTAR_x;
	float deltaSTAR_y;

	/// <summary>
	///
	/// </summary>
	bool standingOnSB = false;
	SmallBug* sb;
	float deltaSB_x;
	float deltaSB_y;

	// hurt
	DWORD injuringTimer, faintedTimer;


	// gunmaching
	static CGimmick* instance;

	int score = 0;
	int finalScore = 0;
	float lives = 1.5f;
	vector<int> items;
	int pickedItem;
	DWORD timeToPickItem=GetTickCount();
	float finalVX;
	DWORD jumpAt;
	DWORD landedWhen;

	bool lockedInView = false;
	int viewFrom = 0;

	bool LockedToView = false;
	int	viewTo = 0;
public:
	bool finalScene = false;
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void GetLocked(bool l = false, int f = 0);
	void GetLockedIn(bool l = false, int f = 0);
	bool ISLocked() { return lockedInView; }
	int GetViewFrom() { return viewFrom; }
	bool IsLockedTo() { return LockedToView; }
	int GetViewTo() { return viewTo;
	}
	void SetStandingOnElevator(int s, float from = 0, float to = 0);
	//void SetStandingMovingStair(bool s, float from = 0, float to = 0, float sY = 0, MStair* st = NULL);
	void SetStandingMovingStair(bool s, float sX = 0, float sY = 0, MStair* st = NULL);
	void SetStandingHangingElevator(bool s, HangingElevator* st = NULL);
	void SetStandingOnSBB(bool s, SmallBlackBug* st = NULL);
	void SetStandingOnBoat(bool s, Boat* st = NULL);
	void SetStandingOnStar(bool s, Star* st = NULL);
	void SetStandingOnWGM(bool s, WheelGunMachine* st = NULL);
	void SetStandingOnGFS(bool s, GreenFatStuff* st = NULL);
	void SetStandingOnBird(bool s, LongNeckedBird* st = NULL);
	void SetStandingOnSB(bool s, SmallBug* st = NULL);
	void SetStandingOnShell(bool s, FlyingShell* st = NULL);
	void SetStangdingOnHill(bool on, Hill* h = NULL);
	float GetX() { return x; }
	float GetY() { return y; }
	
	void SetState(int state);
	void SetDirection(bool dir);
	void SetVY(float _vy);
	void SetX(float _x);
	void SetVX(float _vx);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();				//chua dinh nghia

	void Load(LPCWSTR filepath);

	void SetY(float _y);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom); //chua dinh nghia

	static CGimmick* GetInstance();
	void _checkSweptAABB(vector<LPGAMEOBJECT>* co);
	bool IsJumping() { return state == GIMMICK_STATE_JUMP; }
	bool IsHavingStar() { return stars.size() != 0 || star_phase==3; }
	void ExplodeStars();
	void GetInjured(float live,bool direction=true);
	void IncreaseLife(float l);
	void IncreaseScore(int x) { finalScore += x; }
	bool AddItem(int i);
	vector<int> GetItems() { return items; }
	void PickItem(int i);
	void ResetJumpingControl();
	bool CanJump();
};
#endif