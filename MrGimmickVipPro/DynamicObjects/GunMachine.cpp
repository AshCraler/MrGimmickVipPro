#include "GunMachine.h"
#include "../Gimmick.h"
#include "../StaticObjects/Ground.h"

GunMachine::GunMachine(int x, int y)
{
	this->x = x;
	this->y = y;
	width = WIDTH;
	height = 16;
	vy = 0;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(GM_ANIMATIO_SET_ID));
}

GunMachine::~GunMachine()
{
}
void GunMachine::Render()
{
	animation_set->at(state)->Render(x, y);

	for (int i = 0; i < shells.size(); i++) {
		shells[i]->Render();
	}
	RenderBoundingBox();
}

void GunMachine::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GM_WIDTH;
	b = y - GM_HEIGHT;
}
void GunMachine::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (beingPushed) {
		
	}
	for (int i = 0; i < shells.size(); i++) {
		shells[i]->Update(dt, colliable_objects);
	}
	
	if (onHill != Touching) {
		if (vy < GM_Y_SPEED)
			//vy += GM_GRAVITY*dt;
		if (vx > 0) {
			vx -= FRICTION_FORCE*dt;
			if (vx < 0)vx = 0;
		}
		if (vx < 0) {
			vx += FRICTION_FORCE*dt;
			if (vx > 0)vx = 0;
		}
		if (beingPushed > 0) {
			vx = X_SPEED_WHEN_PUSHED;
		}
		else if (beingPushed < 0) {
			vx = -X_SPEED_WHEN_PUSHED;

		}
	}
	else {

	}
	
	CGameObject::Update(dt);
	vy -= 0.00001 * dt;
	if (abs(x - CGimmick::GetInstance()->GetX()) < X_RANGE_TO_ACTIVE) {
		if (GetTickCount() - shottingTimer > TIME_GAP) {
			shottingTimer = GetTickCount();
			state = SHOTING_STATE;
			Shell* s = new Shell(x, y+1, 1);
			shells.clear();
			shells.push_back(s);
			shottingTimer = GetTickCount();
			
		}
		if (GetTickCount() - shottingTimer > ACTIVE_TIME) {
			/*Shell* s = new Shell(x, y, 1);
			shells.clear();
			shells.push_back(s);
			shottingTimer = GetTickCount();*/
			state = WAITING_STATE;
		}
	}

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0) {
		if (onHill==OutOfArea) {
			landed = false;
			x += dx;
			y += dy;
		}
		else if (onHill == Enter) {
			if (hill->isLeftUpToRight()) {
				if ((x + width - hill->x+6) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
					y = (-hill->height + hill->y + height + (x + width - hill->x+6) / hill->GetXToYRatio());
					onHill = Touching;
					vx = -GM_X_SPEED * hill->GetSpeedReductionFactor();
					vy = vx / hill->GetXToYRatio();

				}
				else {
					x += dx;
					y += dy;
				}
			}
			else {
				if ((-x - 4 + hill->width + hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
					y = ((-hill->height + hill->y + height + (-x + hill->width - 4 + hill->x) / hill->GetXToYRatio()));
					onHill = Touching;
					vx = GM_X_SPEED * hill->GetSpeedReductionFactor();
					vy = -vx / hill->GetXToYRatio();

				}
				else {
					x += dx;
					y += dy;
				}
			}
			if (x > hill->width + hill->x || x + height < hill->x || (vy > 0 && y - height > hill->y) || (vy < 0 && y < hill->y - hill->height)) {
				onHill = OutOfArea;
				if (hill->isLeftUpToRight()) {
					vx = -GM_X_SPEED;
					vy = 0;
					y += dy;
				}
				else {
					vx = X_SPEED;
					y += dy;
				}
				/*SetX(x + dx);
				SetY(y - dt * vy);*/
				//SetVY(GIMMICK_GRAVITY * dt);
			}
		}
		else if (onHill == Touching) {
			if (x > hill->width + hill->x || x + width < hill->x || (vy > 0 && y - height < hill->y) || (vy < 0 && y < hill->y - hill->height)) {
				/*if (x + width < 400)
					x = x;
				DebugOut(L"x %f\n", x);

				onHill = Enter;*/
				if (hill->isLeftUpToRight()) {
					if (x +width< hill->x) {
						vy = 0;
						vx = -MOVING_SPEED;
						onHill = OutOfArea;
					}
				}
				x = (x + dx);
				//SetY(y + dy);
			}
			
			else {
				DebugOut(L"x %f\n", x+width-hill->x);
				if (x + width - hill->x < 0.5)
					x = x;
				x=(x + dx);
				y=(y + dy);
			}
		}

	}
	else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + min_tx * dx + nx * 0.1f;
		y = y + min_ty * dy - ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			if (dynamic_cast<Hill*>(coEventsResult[i]->obj)) {
				if (coEventsResult[i]->ny != 0) {
					onHill = Enter;
					hill = dynamic_cast<Hill*>(coEventsResult[i]->obj);
					y = y - (min_ty * dy - ny * 0.1f) + dy;
				}
				
			}
			else if (dynamic_cast<CGimmick*>(coEventsResult[i]->obj)) {
				if (coEventsResult[i]->ny != 0) {
					y = y - min_ty * dy + ny * 0.1f;

				}
			}
			else if (dynamic_cast<Ground*>(coEventsResult[i]->obj)) {
				if (onHill == Touching) {
					onHill = OutOfArea;
					if (hill->isLeftUpToRight()) {
						vx = -GM_X_SPEED;
						vy = 0;
						//y += dy;
					}
					else {
						vx = GM_X_SPEED;
						y += dy;
						vx = 0;
						vy = 0;
					}
				} 
				if (onHill == OutOfArea) {
					landed = true;
					vy = 0;
				}
				
			}
		}/*
		vy = 0;*/
	}
}