#pragma once
#include"Enemy.h"
#define TURTLE_X_SPEED	0.004f
#define TURTLE_Y_SPEED	0.004f
#define TURTLE_DEATH_STATE	1
#define CRAWLING_STATE	0
#define TURTLE_ANIMATION_SET_ID	23
#define TURTLE_HEIGHT	16
#define	TURTLE_WIDTH	16
class Turtle:public Enemy
{
	int xFrom, xTo;
	bool startDeath = false;
public:
	Turtle(int x, int y, int x_from, int x_to);
	~Turtle();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetTurnedUp();
	bool IsTurnedUp() { return state == TURTLE_DEATH_STATE; }
};

