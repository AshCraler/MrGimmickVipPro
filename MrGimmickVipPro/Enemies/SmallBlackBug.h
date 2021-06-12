#pragma once
#include"Enemy.h"
#include "../StaticObjects/Elevator.h"
#include "../StaticObjects/Ground.h"
#include "../StaticObjects/Hill.h"

#define X_SPEED	0.008f
#define Y_SPEED	0.004f
#define WIDTH 16
#define HEIGHT 16

#define DEATH_STATE	0
#define WALKING_STATE	1
#define JUMPING_STATE	2
#define BRAKING_STATE	3

#define ANIMATION_SET_ID	18

class SmallBlackBug:public Enemy
{
	int standingOnElevator = 0;
	float eFrom, eTo, sFrom, sTo, deltaXToMStair, deltaYToMStair;
public:
	SmallBlackBug(int x, int y, int jar=-1);
	~SmallBlackBug();
	void SetStandingOnElevator(int s, float from = 0, float to = 0) {
		standingOnElevator = s;
		if (standingOnElevator != 0) {
			eFrom = from; eTo = to;
		}
		
	}

	void Jump();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void GetBoundingBox(float &left,float &top,float &right,float &bottom);
};

