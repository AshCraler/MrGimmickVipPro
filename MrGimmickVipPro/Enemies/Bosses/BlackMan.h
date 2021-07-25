#pragma once
#include "Boss.h"
#define BM_WIDTH	40
#define BM_HEIGHT	68
#define BM_ANIMATION_SET_ID	14
#define WARIOR_ANIMATION_SET_ID	16
#define FIRE_ANMATION_SET_ID	17
#define SPARK_ANMATION_SET_ID	15
#define FIRE_ANIMATION_SET_ID	17
#define BM_INACTIVE_STATE	-1
#define BM_ATTACKING_STATE	0
#define BM_DISAPPEARING_STATE	1
#define BM_INVISIBLE_STATE	2
#define BM_APPEARING_STATE	3

#define BM_INJURING_TIME	1000
#define BM_EXISTING_TIME	10000
#define DISAPPEARING_TIME	5000
#define APPEARING_TIME	5000
#define ABOUT_TO_ATTACK_TIME	2500
#define INVISBLE_TIME	5000
#define BM_SURVIVING_TIME	1
#define NUMBER_OF_SPARK	5
#define SPARK_HEIGHT	50
#define SPARK_WIDTH	10
#define SPARK_SPEED	0.01f
#define SPARK_VX	0.01f
#define BM_MAX_DISTANCE_TO_GM	256
#define BM_LEFT_X	304
#define BM_RIGHT_X	500
#define SPARK_EXISTING_TIME	5000
#define BM_SURVIVING_TIMES	1
#define WARIOR_TRANSFORMING_STATE	0
#define WARIOR_READY_STATE	1
#define WARIOR_WALKING_STATE	3
#define WARIOR_JUMPING_STATE	2
#define WARIOR_GETTING_INJURED	4
#define WARIOR_ATTACKING	5
#define WARIOR_RELEASING_FIRE	7
#define WARIOR_RELEASING_SPARK	8
#define WARIOR_STANDING	12
#define WARIOR_GETTING_THROWN	9
#define TRANSFORMING_TIME	3000
#define WARIOR_READY_TIME	3000
#define WARIOR_JUMPING_VX	-0.02f
#define WARIOR_JUMPING_VY	-0.035f
#define WARIOR_RELEASING_SPARK_TIME	3000
#define WARIOR_RELEASING_FIRE_TIME	3000
#define WARIOR_LONG_RANGE	90
#define WARIOR_MIDDLE_RANGE	60
#define WARIOR_SHORT_RANGE	45
#define NUMBER_OF_FIRES	10
#define WARIOR_HEIGHT	60
#define WARIOR_WIDTH	27
#define WARIOR_MARGIN_X_TO_READY	-25
#define FIRE_HEIGHT	4
#define FIRE_WIDTH	4
#define FIRE_FALLING	2
#define FIRE_EXPLODING	3
#define FIRE_EXPLODING_TIME	1000
#define WARIOR_WALKING_VX	0.01f
#define WARIOR_BACK_VX	0.02f;
#define WARIOR_BACK_VY	-0.03f;
#define WARIOR_INJURING_TIME	1000
#define WARIOR_NUMBER_OF_SPARK	1
#define WARIOR_WALKING_TIME	1000
#define WARIOR_RIGHT_BOUNDARY	500
#define WARIOR_SURVIVAL_TIMES	3
#define WARIOR_GETTING_INJURED_TIME	4000
#define WARIOR_INJURING_VX	0.01f
#define WARIOR_INJURING_VY	0.02f
class BFire :public CGameObject {
	DWORD explodingTimer;
public: 
	BFire(int x, int y, float vx, float vy);
	~BFire();
	void Render();
	void SetDestroyed();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
class Spark :public CGameObject {
	int ani;
public:
	Spark(int x, int y, bool toRight, int ani);
	~Spark();
	void Render();
	void Update(DWORD dt);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
class BlackMan:public Boss
{
	enum form {
		blackman=0,
		warior = 1,
		falling_sparks = 2,
	};
	float distance = 0;
	bool increase = false;
	float Xs[100];
	float Ys[100];
	float fire_VXs[NUMBER_OF_FIRES] = { -0.01f,-0.01f,-0.01f,-0.01f,-0.01f,-0.01f,-0.01f,-0.01f,-0.01f,-0.01f };
	float fire_VYs[NUMBER_OF_FIRES] = { 0.01f,0.005f,0.006f,0.007f,0.008f,0.009f,0.01f,0.01f,0.01f,0.01f };
	int bm_RemainingHits;
	int form;
	DWORD dyingTimer, spreadingSparkTimer=0, fallingSparkTimer=0;
	DWORD releasingSparkTimer, releasingFireTimer, walkingTimer;
	DWORD transformingTimer, readyTimer;
	DWORD invisibleTimer, sparkExistingTimer;
	DWORD injuringTimer,  existingTimer, disappearingTimer, appearingTimer, aboutToAttackTimer;
	int BMremainingHits;
	bool releasedSparks = false;
	vector<Spark*> sparks;
	vector<BFire*> fires;
public:
	BlackMan(int x, int y);
	~BlackMan();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit();
	vector<Spark*> GetSparks() { return sparks; }
	vector<BFire*> GetFires() { return fires; }
};

