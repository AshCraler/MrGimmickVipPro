#pragma once
#include "Item.h"

#define FLOWER_POT	0
#define HOUR_GLASS	1
#define POT_WIDTH	22
#define POT_HEIGHT	43
#define GLASS_WIDTH	17
#define GLASS_HEIGHT	28
#define NUMBER_OF_SPARKLES	10
#define PRIZE_ANIMATION_ID_STARTS_AT	500
#define SPARKLE_ANIMATION_ID_STARTS_AT	2
#define ANIMATION_SET_ID	50
class BigPrize:public Item
{
	vector<CAnimation*> sparkles;
	vector<int> sparkleXs;
	vector<int> sparkleYs;
public:
	BigPrize(int x, int y, int type);
	~BigPrize();
	void Render();

};

