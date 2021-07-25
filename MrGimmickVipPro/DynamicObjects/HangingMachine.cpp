#include "HangingMachine.h"

HangingMachine::HangingMachine(int x, int y)
{
	this->x = x;
	this->y = y;
	this->state = WAITING_STATE;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
}

HangingMachine::~HangingMachine()
{
}
void HangingMachine::Render()
{
	if (!(state == SMOKE_STATE && GetTickCount() - smokeTimer > SMOKING_TIME)) {
		animation_set->at(state)->Render(x, y);

		RenderBoundingBox();
	}
	
}

void HangingMachine::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
	if (state == WAITING_STATE) {
		l = x-WIDTH_WHEN_WAITING;
		t = y;
		r = x + WIDTH_WHEN_WAITING;
		b = y - HEIGHT_WHEN_WAITING;
	}
	else if (state == FALLING_STATE) {
		l = x;
		t = y;
		r = x + WIDTH_WHEN_FALLING;
		b = y - HEIGHT_WHEN_FALLING;
	}
	else {
		l = x;
		t = y;
		r = x; b = y;
	}
	
}
void HangingMachine::GetDestroyed() {
	this->state = SMOKE_STATE;
	smokeTimer = GetTickCount();
	vy = 0;
}
void HangingMachine::Go() {
	this->state = FALLING_STATE;
	
}
void HangingMachine::Update(DWORD dt) {
	
	CGameObject::Update(dt);
	if (state == FALLING_STATE) {
		if(vy<FALLING_SPEED)
			vy -= dt * GRAVITY;
	}
	y = y + dy;
}