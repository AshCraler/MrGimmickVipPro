#pragma once
#include "StaticObject.h"
#define ANIMATIO_SET_ID	-10
class Pipe:public StaticObject
{
	
	int spriteId;
	int type;
	bool canWalk = false;
public:
	enum PipeTypes {
		top_right = 12,
		top_bottom = 13,
		top_left = 14,
		right_bottom = 23,
		right_left = 24,
		bottom_left = 34,
	};
	Pipe(int x, int y, int width, int height, int spriteId, bool canWalk=false);
	~Pipe();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool CanWalk() { return canWalk; }
	int GetType() { return type; }
};

