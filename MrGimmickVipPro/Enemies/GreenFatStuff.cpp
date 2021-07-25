 #include "GreenFatStuff.h"
#include "../Gimmick.h"
#include "../StaticObjects/Ground.h"
#include "Utility/JumpingCommandBox.h"
GreenFatStuff::GreenFatStuff(int x, int y) {
	this->x = x;
	this->SetY(y);
	this->state = GFS_INACTIVE_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(GFS_ANIMATION_SET_ID);
	remainingHits = SURVIVAL_TIMES;
	width = GFS_WIDTH;
	height = GFS_HEIGHT;
}

GreenFatStuff::~GreenFatStuff() {}
void GreenFatStuff::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state != GFS_DEATH_STATE) {
		left = x;
		top = y;
		right = left + width;
		bottom = top - height;
	}
	else {
		left = right=  0;
		top = bottom = 0;
	}
	
}
void GreenFatStuff::Render() {
	if (state != GFS_INACTIVE_STATE && state != GFS_BRAKING) {
		int aniFrom = direction ? 0 : animation_set->size() / 2;
		if(GetTickCount()-untouchableTimer>UNTOUCHABLE_TIME)
			animation_set->at(aniFrom + state)->Render(x, y+5);
		else
			animation_set->at(aniFrom + state + 4)->Render(x, y+5);

	}
	else if(state==GFS_BRAKING){
		int aniFrom = direction ? 0 : animation_set->size() / 2;
		if (GetTickCount() - untouchableTimer > UNTOUCHABLE_TIME)
			animation_set->at(aniFrom + GFS_LANDING_STATE)->Render(x, y+3);
		else
			animation_set->at(aniFrom + GFS_LANDING_STATE + 4)->Render(x, y+3);

	}
	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void GreenFatStuff::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == GFS_INACTIVE_STATE) {
		if (abs(x-CGimmick::GetInstance()->GetX()) < GFS_MAX_DISTANCE) {
			vx = GFS_X_SPEED;
			direction = true;
			state = GFS_LANDING_STATE;
			SetVY(0);
		}
	}
	else if (state == GFS_DEATH_STATE) {
		CGameObject::Update(dt);

		vy -= 0.00001 * dt;
		x += dx;
		SetY(y + dy);
		
	}
	else {
		CGameObject::Update(dt);

		vy -= 0.00001 * dt;
		if (vy > 0.05)
			SetVY(vy);
		if (vy > 0 && state != GFS_JUMPING_STATE && state!=GFS_DEATH_STATE)
			state = GFS_JUMPING_STATE;
		else if (vy < 0 && state != GFS_LANDING_STATE && state != GFS_WALKING_STATE && state!=GFS_BRAKING && state != GFS_DEATH_STATE) {
			state = GFS_LANDING_STATE;

		}
		if (state == GFS_BRAKING) {
			if (vx < 0) {
				vx += 0.00005 * dt;
				if (vx > 0) {
					vx = 0;
					state = GFS_WALKING_STATE;
				}
			}
			else {
				vx -= 0.00005 * dt;
				if (vx < 0) {
					vx = 0;
					state = GFS_WALKING_STATE;
				}
			}
		}
		if (state == GFS_WALKING_STATE) {
			CGimmick* g = CGimmick::GetInstance();
			if (x > g->GetX() + g->width) {
				vx = -GFS_X_SPEED;
				direction = false;
			}
			else if (x + width < g->GetX()) {
				vx = GFS_X_SPEED;
				direction = true;
			}
			else {
				vx = direction ? GFS_X_SPEED : -GFS_X_SPEED;
			}
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(colliable_objects, coEvents);
		if (coEvents.size() == 0 || state==GFS_DEATH_STATE) {
			x += dx;
			SetY(y + dy);
		}
		else {
			float min_tx, min_ty, nx, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x = x + min_tx * dx + nx * 0.1f;
			SetY(y + min_ty * dy - ny * 0.1f);

			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny != 0)
					SetVY(0);
				if (dynamic_cast<Ground*>(e->obj)) {
					if (state == GFS_LANDING_STATE) {
						state = GFS_BRAKING;
					}
					SetVY(0);

				}
				else if (dynamic_cast<LockingViewPoint*>(e->obj)) {
					LockingViewPoint* p = dynamic_cast<LockingViewPoint*>(e->obj);
					

					SetX(x - min_tx * dx - nx * 0.3f + dx);
					if (e->ny != 0)
						SetY(y - min_ty * dy + ny * 0.3f + dy);
				}
				else if (dynamic_cast<LockingViewToPoint*>(e->obj)) {
					LockingViewToPoint* p = dynamic_cast<LockingViewToPoint*>(e->obj);


					SetX(x - min_tx * dx - nx * 0.3f + dx);
					if (e->ny != 0)
						SetY(y - min_ty * dy + ny * 0.3f + dy);
				}
			}
		}
		if (y > 176)
			SetY(y);
		if (abs(x - JP1) < 1 && vx>0) {
			SetVY(GFS_Y_SPEED);
			state = GFS_JUMPING_STATE;
		}
		else if (abs(x - JP2) < 1 && vx < 0) {
			SetVY(GFS_Y_SPEED);
			state = GFS_JUMPING_STATE;
		}
		else if (abs(x - JP3) < 1) {
			SetVY(GFS_Y_SPEED);
			state = GFS_JUMPING_STATE;
		}
	}
}
void GreenFatStuff::SetVX(float v) {
	this->vx = v;
}
void GreenFatStuff::SetVY(float v) {
	if (abs(v) > 0.1)
		v = v;
	this->vy =v;
}
void GreenFatStuff::SetX(float v) {
	if (abs(v - x) > 5)
		x = x;
	this->x = v;
}
void GreenFatStuff::SetY(float v) {
	if (abs(v - y) > 5)
		x = x;
	this->y=v;
}
void GreenFatStuff::GetHit() {
	if (GetTickCount() - untouchableTimer > UNTOUCHABLE_TIME) {
		remainingHits -= 1;
		if (remainingHits == 0) {
			state = GFS_DEATH_STATE;
			SetVY(GFS_Y_SPEED);
			vx = GFS_X_SPEED;
			direction = true;
			CGimmick::GetInstance()->GetLocked(false);
			//CGimmick::GetInstance()->GetLockedIn(false);

		}
		else {
			untouchableTimer = GetTickCount();
		}
	}
	
}