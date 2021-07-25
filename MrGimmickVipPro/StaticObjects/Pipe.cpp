#include "Pipe.h"
Pipe::Pipe(int x, int y,int width, int height, int spriteId, bool cw) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->spriteId = spriteId;
	this->canWalk = cw;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
	if (spriteId == 0)
		type = right_left;
	else if (spriteId == 1)
		type = top_bottom;
	else if (spriteId == 2)
		type = bottom_left;
	else if (spriteId == 3)
		type = right_bottom;
	else if (spriteId == 27)
		type = top_right;
	else if (spriteId == 28)
		type = top_left;

}
Pipe::~Pipe(){}
void Pipe::Render() {
	if (spriteId == 13) {
		//animation
	}
	else {
		animation_set->at(spriteId)->Render(x, y);
	}
	RenderBoundingBox();
}
void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (type == top_left) {
		l = x-2; r = x + width;
		t = y; b = y - height;
	}
	else if (type == top_bottom) {
		l = x + 2; r = x + width;
		t = y; b = y - height+1;
	}
	else {
		l = x; r = x + width;
		t = y; b = y - height;
	}
	
	//RenderBoundingBox();
}