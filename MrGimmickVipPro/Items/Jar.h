#pragma once
#include "Item.h"
#include "../Sprites.h"
#include "../StaticObjects/Elevator.h"
#define PINK_LIQUID		0
#define ORANGE_LIQUID		1
#define PINK_BALL		2
#define BLACK_BALL		3
#define ORANGE_BALL		4
#define JAR_SPRITE_ID_STARTS_AT	600101
#define JAR_HEIGHT	16
#define JAR_WIDTH	14
#define JAR_VX	0.01f
#define JAR_VY	0.01f
class Jar: public Item
{
	bool collidedWithGround = false;
	bool hidden;
	Elevator* elevator=NULL;
public:
	void Render();
	// void GetBoundingBox(float& l, float& t, float& r, float& b);
	Jar(int x, int y, int type, bool hidden=true);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void Appear(float x, float y, bool dir = true);
	void GetPushed(float dx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetMoved(bool dir) { vx = dir ? JAR_VX : -JAR_VX; }
	~Jar();
};

