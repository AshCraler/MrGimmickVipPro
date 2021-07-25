#include "WindowThrowingShell.h"

WindowThrowingShell::WindowThrowingShell(int x, int y)
{
	this->x = x;
	this->y = y;
	this->state = WTS_WAITING_STATE;
	vx = vy = 0;
	vx = -0.005;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(WTS_ANIMATIO_SET_ID));
	height = 16;
}

WindowThrowingShell::~WindowThrowingShell()
{
}
void WindowThrowingShell::Render()
{
	if (state != WTS_WAITING_STATE) {
		if (state == WTS_HITTING_TARGET && GetTickCount() - explodeAt < EXPLODING_TIME)
			animation_set->at(state)->Render(x, y);
		if (state == WTS_GOING_TO_WATER && GetTickCount() - hitWaterAt < WATER_TIME)
			animation_set->at(state)->Render(x, y);
		if(state== WTS_FALLING_STATE)
			animation_set->at(state)->Render(x, y);


	}

	RenderBoundingBox();
}

void WindowThrowingShell::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == WTS_WAITING_STATE) {
		l = x;
		t = y;
		r = x + 32;
		b = y - 32 * 13;
	}
	else if (state == WTS_FALLING_STATE) {
		l = x;
		t = y;
		r = x + 16;
		b = y - 16;
	}
	else {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
	
}
void WindowThrowingShell::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == WTS_FALLING_STATE) {
		CGameObject::Update(dt);
		vy -= 0.000005 * dt;
		y += dy;
		if (y -height<16 /*y_Start_Falling - FALLING_RANGE*/) {
			vy = 0;
			state = WTS_GOING_TO_WATER;
			hitWaterAt = GetTickCount();
			y += 32;
		}
	}
}
void WindowThrowingShell::ThrowShell() {
	x = x + 12;
	y = y - 24;
	y_Start_Falling = y;
	state = WTS_FALLING_STATE;
	vy = WTS_FALLING_SPEED;
}
void WindowThrowingShell::Explode() {
	state = WTS_HITTING_TARGET;
	vy = 0;
	explodeAt = GetTickCount();
}
