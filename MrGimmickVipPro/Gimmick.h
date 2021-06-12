#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Utils.h"
#include "Game.h"
#include "StaticObjects/MStair.h"
#include "Textures.h"
//SPEED
#define GIMMICK_WALKING_SPEED		0.02f //0.1f
#define GIMMICK_JUMP_SPEED_Y		0.05f
#define GIMMICK_JUMP_DEFLECT_SPEED	0.2f
#define GRAVITY						0.00001f
#define GIMMICK_FRICTION_FORCE		0.0001f
#define GIMMICK_DIE_DEFLECT_SPEED	0.5f

//STATE
#define GIMMICK_STATE_IDLE			1
#define GIMMICK_STATE_WALKING_RIGHT	2
#define GIMMICK_STATE_WALKING_LEFT	102
#define GIMMICK_STATE_JUMP			3
#define GIMMICK_STATE_DIE			6

//ANI
#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT		6
#define GIMMICK_ANI_WALKING_RIGHT	1
#define GIMMICK_ANI_WALKING_LEFT	7

#define GIMMICK_ANI_DIE				106

//TIME
#define GIMMICK_UNTOUCHABLE_TIME	5000

class CGimmick : public CPlayer
{
	//chua xu ly toi
	//int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;
	bool standingOnMovingStair = false;
	int standingOnElevator = 0;
	float eFrom, eTo, sFrom, sTo, deltaXToMStair, deltaYToMStair;
	MStair* stair = NULL;
	static CGimmick* instance;

public:
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetStandingOnElevator(int s, float from=0, float to=0) {
		standingOnElevator = s;
		if (standingOnElevator!=0) {
			eFrom = from; eTo = to;
		}
		standingOnMovingStair = false;
	}
	void SetStandingMovingStair(bool s, float from = 0, float to = 0, float sY = 0, MStair* st = NULL) {
		standingOnMovingStair = s;
		if (standingOnMovingStair != 0) {
			sFrom = from; 
			sTo = to;
			deltaXToMStair = ceil(x - from);
			deltaYToMStair = ceil(y - sY);
		}
		stair = st;
		standingOnElevator = 0;
	}
	float GetX() { return x; }
	float GetY() { return y; }
	
	void SetState(int state);
	void SetVY(float _vy);
	void SetX(float _x);
	void SetVX(float _vx);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();				//chua dinh nghia

	void Load(LPCWSTR filepath);

	void SetY(float _y);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom); //chua dinh nghia
	void RenderBoundingBox();
	static CGimmick* GetInstance();
	void _checkSweptAABB(vector<LPGAMEOBJECT>* co);
};