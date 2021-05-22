#include "MStair.h"

MStair::MStair()
{
}

void MStair::Render()
{
	
	blocks[0]->at(0)->Render(x , y );
	blocks[1]->at(1)->Render(x + 16, y);

	RenderBoundingBox();
}

void MStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void MStair::Update(DWORD dt) {
	x += direction ? dt * MOVING_STAIR_SPEED : dt * (-MOVING_STAIR_SPEED);
	if (abs(startAtX - x) > MOVING_RANGE) direction = !direction;
}
MStair::MStair(int width, int height)
{
	this->width = width;
	this->height = height;
}
MStair::MStair(int x, int y, int width, int height, bool direction)
{
	this->x = x;
	this->startAtX = this->x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->direction = direction;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(530));
	blocks.push_back(animation_set);
	blocks.push_back(animation_set);

}

MStair::~MStair()
{
}
