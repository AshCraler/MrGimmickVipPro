#pragma once
#include "GameObject.h"

//SPEED
#define GIMMICK_WALKING_SPEED		0.15f //0.1f
#define GIMMICK_JUMP_SPEED_Y		0.5f
#define GIMMICK_JUMP_DEFLECT_SPEED	0.2f
#define GIMMICK_GRAVITY				0.002f
#define GIMMICK_DIE_DEFLECT_SPEED	0.5f

//STATE
#define GIMMICK_STATE_IDLE			0
#define GIMMICK_STATE_WALKING_RIGHT	100
#define GIMMICK_STATE_WALKING_LEFT	200
#define GIMMICK_STATE_JUMP			300
#define GIMMICK_STATE_DIE			400

//ANI
#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT		1
#define GIMMICK_ANI_WALKING_RIGHT	2
#define GIMMICK_ANI_WALKING_LEFT	3

#define GIMMICK_ANI_DIE				4

//TIME
#define GIMMICK_UNTOUCHABLE_TIME	5000

class CGimmick :CGameObject
{
	//chua xu ly toi
	//int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;
public:
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();				//chua dinh nghia

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom); //chua dinh nghia
};