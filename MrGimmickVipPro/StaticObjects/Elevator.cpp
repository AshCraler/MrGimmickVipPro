#include "Elevator.h"

Elevator::Elevator()
{
}

void Elevator::Render()
{
	/*int numBlocks = width / 16;
	for (int i = 0; i < numBlocks; i++) {
		CAnimationSets::GetInstance()->Get(520)->at(0)->Render(x+i*16, y+16);
	}
	CAnimationSets::GetInstance()->Get(520)->at(1)->Render(startAtX-16, y+16);
	CAnimationSets::GetInstance()->Get(520)->at(2)->Render(startAtX +width-16, y + 16);*/
	int numBlocks = width / 16;
	for (int i = 0; i < numBlocks; i++) {
		blocks[i]->at(direction?0:1)->Render(x + i * 16, y + 16);
	}
	RenderBoundingBox();
}

void Elevator::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

Elevator::Elevator(int width, int height)
{
	this->width = width;
	this->height = height;
}
Elevator::Elevator(int x, int y, int width, int height, bool direction)
{
	this->x = x;
	this->startAtX = this->x;
	this->y = y;
	this->width = width ;
	this->height = height;
	this->direction = direction;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(520));
	int numBlocks = width / 16;
	for (int i = 0; i < numBlocks; i++) {
		blocks.push_back(animation_set);
	}
}

Elevator::~Elevator()
{
}
