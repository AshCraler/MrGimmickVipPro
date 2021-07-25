#include "WheelGunMachine.h"
#include "../StaticObjects/Ground.h"
#include "../Gimmick.h"
FlyingShell::FlyingShell(int x, int y)
{
	this->x = x;
	this->y = y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(FS_ANIMATION_SET_ID));
	vx = -FS_VX;
	vy = 0;
	state = FS_GOING_STRAIGHT_STATE;
}

FlyingShell::~FlyingShell()
{
}
void FlyingShell::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void FlyingShell::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FS_WIDTH;
	b = y - FS_HEIGHT;
}
void FlyingShell::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	if (state == FALLING_STATE) {
		vy -= 0.00005 * dt;
	}
	if (coEvents.size() == 0) {
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + min_tx * dx + nx * 0.1f;
		y = y + min_ty * dy - ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj)) {
				if (e->nx != 0) {
					vx = FS_VX;
					vy = FS_VY_REFLECTED*10;
					state = FALLING_STATE;
				}
			}
			
		}
	}
}



WheelGunMachine::WheelGunMachine(int x, int y)
{
	this->x = x;
	this->y = y;
	markedX = x;
	width = WGM_WIDTH;
	height = WGM_HEIGHT;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(WGM_ANIMATION_SET_ID));
	timeToShot = GetTickCount();
}

WheelGunMachine::~WheelGunMachine()
{
}
void WheelGunMachine::Render()
{
	animation_set->at(wheel)->Render(x, y);

	for (int i = 0; i < shells.size(); i++) {
		shells[i]->Render();
	}

	RenderBoundingBox();
}

void WheelGunMachine::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WGM_WIDTH;
	b = y - WGM_HEIGHT;
}
void WheelGunMachine::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vy -= 0.00001 * dt;
	vx = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0) {
		x += dx;
		if (fallOnce)
			y += dy;
		else fallOnce = true;
	}
	else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + min_tx * dx + nx * 0.1f;
		y = y + min_ty * dy - ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj)) {
				vy = 0;
				fallOnce = false;
			}else if (dynamic_cast<CGimmick*>(e->obj)) {
				if (e->ny != 0) {
					y = y - min_ty * dy + ny * 0.1f;

				}
				
			}
		}
	}
	if (abs(x - markedX) > X_WHEEL_RANGE) {
		wheel = (wheel + 1) % 4;
		markedX = x;
	}
	
	if (GetTickCount() - timeToShot > STOPPING_TIME) {
		FlyingShell* s = new FlyingShell(x, y-1);
		shells.push_back(s);
		timeToShot = GetTickCount();
	}
	for (int i = 0; i < shells.size(); i++) {
		if (abs(shells[i]->x - CGimmick::GetInstance()->GetX()) > MAX_DISTANCE_TO_GM 
			//abs(shells[i]->y - CGimmick::GetInstance()->GetY()) > 100
			) {
			shells.erase(shells.begin() + i);
		}
	}
	for (int i = 0; i < shells.size(); i++) {
		shells[i]->Update(dt, colliable_objects);
	}
}
void WheelGunMachine::GetPushed(bool dir) {

	vx = dir?WGM_X_SPEED:-WGM_X_SPEED;
}