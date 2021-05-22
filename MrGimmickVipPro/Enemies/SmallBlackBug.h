#pragma once
#include"Enemy.h"
#define X_SPEED	0.004f
#define Y_SPEED	0.004f
#define DEATH_STATE	0
#define WALKING_STATE	1
#define JUMPING_STATE	2
#define BRAKING_STATE	3
#define ANIMATION_SET_ID	18
class SmallBlackBug:public Enemy
{

public:
	SmallBlackBug(int x, int y, int jar=-1);
	~SmallBlackBug();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};

