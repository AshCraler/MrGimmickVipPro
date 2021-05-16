#pragma once
#include "GameObject.h"
#include "Player.h"
//SPEED
#define GIMMICK_WALKING_SPEED		0.008f //0.1f
#define GIMMICK_JUMP_SPEED_Y		0.5f
#define GIMMICK_JUMP_DEFLECT_SPEED	0.2f
#define GIMMICK_GRAVITY				0.0001f
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

	static CGimmick* instance;
public:
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();				//chua dinh nghia

	void Load(LPCWSTR filepath);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom); //chua dinh nghia

	static CGimmick* GetInstance();
	void _checkSweptAABB(vector<LPGAMEOBJECT>* co);
};