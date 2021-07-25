#include "MStair.h"

MStair::MStair()
{
}

void MStair::Render()
{
	if (startAtX == 193)
		startAtX = startAtX;
	blocks[0]->at(0)->Render(x , y );
	blocks[1]->at(1)->Render(x + 16, y);

	RenderBoundingBox();
}

void MStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y - height;
}
void MStair::Update(DWORD dt) {
	if (circle) {
		radius -= 0.03 * dt;
		x = this->centerX + distance * cos(D3DXToRadian(radius));
		y = this->centerY + distance * sin(D3DXToRadian(radius));

	}
	else {
		x += direction ? dt * MOVING_STAIR_SPEED : dt * (-MOVING_STAIR_SPEED);
		if (abs(startAtX - x) > MOVING_RANGE) direction = !direction;
	}
}
MStair::MStair(int width, int height)
{
	this->width = width;
	this->height = height;
}
MStair::MStair(int x, int y, int width, int height, bool direction, bool cirle)
{
	if (!cirle) {
		this->x = x;
		this->startAtX = this->x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->direction = direction;
		SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
		blocks.push_back(animation_set);
		blocks.push_back(animation_set);
		circle = cirle;
	}
	else {
		this->centerX = x - distance;
		this->centerY = y;
		this->y = y;
		this->width = width;
		this->height = height;
		this->direction = direction;
		SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
		blocks.push_back(animation_set);
		blocks.push_back(animation_set);
		circle = cirle;
	}
}

MStair::~MStair()
{
}
