#pragma once
#include "Item.h"
#include "../Sprites.h"
#define PINK_LIQUID		0
#define ORANGE_LIQUID		1
#define PINK_BALL		2
#define BLACK_BALL		3
#define ORANGE_BALL		4
#define JAR_SPRITE_ID_STARTS_AT	600101
#define JAR_HEIGHT	16
#define JAR_WIDTH	14
class Jar: public Item
{
public:
	void Render();
	// void GetBoundingBox(float& l, float& t, float& r, float& b);
	Jar(int x, int y, int type);
	~Jar();
};

