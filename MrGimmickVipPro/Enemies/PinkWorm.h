#pragma once
#include"Enemy.h"
#define X_SPEED	0.004f
// #define Y_SPEED	0.004f
#define DEATH_STATE	1
#define CRAWLING_STATE	0
#define ANIMATION_SET_ID	22

class PinkWorm :public Enemy
{
	int xFrom, xTo;
public:
	PinkWorm(int x, int y, int x_from, int x_to);
	~PinkWorm();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetHit(bool dir);
};

