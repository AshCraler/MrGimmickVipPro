#include "HangingElevator.h"

HangingElevator::HangingElevator(int x, int y)
{
	this->x = x;
	this->y = y;
	this->state = WAITING_STATE;
	this->rolledX = x;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
}

HangingElevator::~HangingElevator()
{
}
void HangingElevator::Render()
{
	if (state == RUNNING_STATE) {
		animation_set->at(WAITING_STATE)->Render(x , y + MARGIN_TO_Y_STAND);

	}
	else 
		animation_set->at(state)->Render(x , y + MARGIN_TO_Y_STAND);
	animation_set->at(wheelCount+4)->Render(x+15, y + MARGIN_TO_Y_STAND);
	RenderBoundingBox();
}

void HangingElevator::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (state != RELEASED_STATE) {
		r = x + MARGIN_TO_X_STAND*2 + 8;
		b = t-HEIGHT;
	}
	else {
		r = x;
		b = y;
	}
}
void HangingElevator::Update(DWORD dt) {
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (x - markedX > RUNNING_RANGE&&state==RUNNING_STATE) {
		state = RELEASING_STATE;
		vx = 0;
		releasingTimer = GetTickCount();
	}
	if (state == RELEASING_STATE&&GetTickCount() - releasingTimer > RELEASING_TIME) {
		state = RELEASED_STATE;
	}
	if (x - rolledX > 16) {
		rolledX = x;
		wheelCount = (wheelCount + 1) % 4;
	}
}
void HangingElevator::Go() {
	if (state == WAITING_STATE) {
		state = RUNNING_STATE;
		vx = RUNNING_SPEED;
		markedX = x;
	}
}