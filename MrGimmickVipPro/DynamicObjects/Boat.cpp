#include "Boat.h"

Boat::Boat(int x, int y, int xTo)
{
	this->x = x;
	this->y = y;
	this->xTo = xTo;
	this->vy = 0;
	this->state = WAITING_STATE;
	width = BOAT_WIDTH;
	height = BOAT_HEIGHT;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(BOAT_ANIMATIO_SET_ID));
}

Boat::~Boat()
{
}
void Boat::Render()
{
	animation_set->at(ANIMATION_OFFSET)->Render(x, y);

	RenderBoundingBox();
}

void Boat::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y-2;
	r = x + width;
	b = y - height;
}
void Boat::Update(DWORD dt) {
	if (state == MOVING_STATE) {
		CGameObject::Update(dt);
		x += dx;
		if (x > xTo) {
			vx = 0;
			state = STOPPED_STATE;
		}
	}
}
void Boat::Go() {
	if (state == WAITING_STATE) {
		state = MOVING_STATE;;
		vx = BOAT_X_SPEED;
	}
}