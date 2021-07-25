#include "GiantBlackBug.h"
#include "../../StaticObjects/Ground.h"
#include "../../Gimmick.h"

SmallBug::SmallBug(float x, float y, float vx, float vy) {
	this->x = x;
	this->y = y;
	this->width =	SB_WIDTH;
	this->height =	SB_HEIGHT;
	this->vx = vx;
	this->vy = vy;
	this->state = SB_FALLING_STATE;
	
	animation_set = CAnimationSets::GetInstance()->Get(SB_ANIMATION_SET_ID);
}

SmallBug::~SmallBug() {}
void SmallBug::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state != SB_DEATH_STATE) {
		left = x;
		top = y;
		right = left + SB_WIDTH;
		bottom = top- SB_HEIGHT;
	}
	else {
		left = right = 0;
		top = bottom = 0 ;
	}
}

void SmallBug::Render() {
	int aniFrom = true ? 0 : animation_set->size() / 2;
	if (state == SB_DEATH_STATE) aniFrom = 0;
	animation_set->at(aniFrom + state)->Render(x, y);
	RenderBoundingBox();
	
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}
void SmallBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vy -= dt * 0.00001;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);


	if (coEvents.size() == 0 || state == SB_DEATH_STATE) {
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
			if (dynamic_cast<SmallBug*>(e->obj)) {
				x = x - min_tx * dx - nx * 0.1f + dx;
				y = y - min_ty * dy + ny * 0.1f + dy;

			}
			else if (dynamic_cast<Ground*>(e->obj)) {
				if (state == SB_FALLING_STATE) {
					state = SB_WALKING_STATE;
					vx = SB_X_SPEED/1.5;
				}
			}
			if (e->ny != 0)vy = 0;
		}
	}
}

void SmallBug::GetHit(bool dir) {
	this->state = SB_DEATH_STATE;
	vx = dir ? SB_X_SPEED : -SB_X_SPEED;
	vy = SB_Y_SPEED;
	//shell->GetReleased();
}


GiantBlackBug::GiantBlackBug(int x, int y, int x_from, int x_to) {
	this->x = x;
	this->y = y;
	this->xFrom = x_from;
	this->xTo = x_to;
	vx = GBB_X_SPEED;
	this->state = GBB_WALKING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(GBB_ANIMATION_SET_ID);
	bugs.clear();

}

GiantBlackBug::~GiantBlackBug() {}

void GiantBlackBug::Render() {
	//int aniFrom = direction ? 0 : animation_set->size() / 2;
	if(state==GBB_ABOUT_TO_ATTACK_STATE)
		animation_set->at(state)->Render(x, y+14);
	else if(state==GBB_BEING_ATTACKED_STATE)
		animation_set->at(state)->Render(x, y+3);
	else 
		animation_set->at(state)->Render(x, y-1);

	if (state == GBB_WALKING_STATE) {
		animation_set->at(ANTENA_WHEN_WALKING)->Render(x + 6, y + 11);
	}else if(state==GBB_STANDING_STATE)
		animation_set->at(ANTENA_WHEN_STANDING)->Render(x + 6, y + 11);


	if (bugs.size() != 0) {
		for (int i = 0; i < bugs.size(); i++) {
			bugs.at(i)->Render();
		}
	}

	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void GiantBlackBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);
	vy -= X_FRICTION * dt;
	/*if (GetTickCount() - dumb > 20000) {
		SentFlying();
		dumb = GetTickCount();
	}*/
	if(state ==GBB_BEING_ATTACKED_STATE){
		if (vx > 0) {
			/*vx = GBB_X_SPEED;
			state = GBB_WALKING_STATE;
			walkingTime = GetTickCount();*/
			state = GBB_STANDING_STATE;
			vx = 0;
			standingTime = GetTickCount();
		}
		else {
			if(touchedGroundWhenAttacked)
				vx += X_FRICTION * dt;
		}
	}
	else if(state == GBB_WALKING_STATE) {
		if (GetTickCount() - walkingTime > WALKING_TIME || x+GBB_WIDTH>xTo) {
			state = GBB_STANDING_STATE;
			vx = 0;
			standingTime = GetTickCount();
		}
	}
	else if (state == GBB_STANDING_STATE) {
		if (GetTickCount() - standingTime > STANDING_TIME && x + GBB_WIDTH > xTo) {
			state = GBB_ABOUT_TO_ATTACK_STATE;
			attacking = false;
			preparingTime = GetTickCount();
		}
		else if (GetTickCount() - standingTime > STANDING_TIME_BEFORE_WALKING && x + GBB_WIDTH < xTo) {
			vx = GBB_X_SPEED;
			state = GBB_WALKING_STATE;
			walkingTime = GetTickCount();
		}
	}
	else if (state == GBB_ABOUT_TO_ATTACK_STATE) {
		if (GetTickCount() - preparingTime > PREPARING_TIME && !attacking) {
			attacking = true;
			if (bugs.size() != 0)bugs.clear();
			for (int i = 0; i < NUMBER_OF_BUGS; i++) {
				bugs.push_back(new SmallBug(x + 20, y + 11, bugs_vx[i], bugs_vy[i]));
			}
		}
		if (GetTickCount() - preparingTime > PREPARING_TIME * 1.5 && attacking) {
			state = GBB_STANDING_STATE;
			vx = 0;
			standingTime = GetTickCount();
		}
	}
	if (state == GBB_DEATH_STATE) {
		if (GetTickCount() - defeatedWhen > 5000) {
			CGimmick::GetInstance()->GetLocked(false);

		}
	}
	if (coEvents.size() == 0 || state == GBB_DEATH_STATE) {
		/*if (state != STOOD_ON_STATE) {
			if (state != JUMPING_STATE && vyIncreasedOnce)
				state = JUMPING_STATE;
			x += dx;
			y += dy;
			if (!vyIncreasedOnce)vyIncreasedOnce = true;
		}*/
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
				if (e->ny != 0) {
					vy = 0;
					if (state == GBB_BEING_ATTACKED_STATE && !touchedGroundWhenAttacked) touchedGroundWhenAttacked = true;
				}
			}
			
		}
	}
	if (bugs.size() != 0) {
		for (int i = 0; i < bugs.size(); i++) {
			if (bugs.at(i)->x > x + 200) bugs.erase(bugs.begin() + i);
			else if (bugs.at(i)->y < y - 200)  bugs.erase(bugs.begin() + i);
			else bugs.at(i)->Update(dt, colliable_objects);
		}
	}
	
}
void GiantBlackBug::SentFlying() {
	remainingHits--;
	if (remainingHits != 0) {
		vx = -GBB_X_WHEN_ATTACKED;
		vy = GBB_Y_WHEN_ATTACKED;
		state = GBB_BEING_ATTACKED_STATE;
		touchedGroundWhenAttacked = false;
	}
	else {
		vx = -GBB_X_WHEN_ATTACKED;
		vy = GBB_Y_WHEN_ATTACKED*2;
		state = GBB_DEATH_STATE;
		defeatedWhen = GetTickCount();
	}
}

void GiantBlackBug::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + GBB_WIDTH;
	bottom = top - GBB_HEIGHT;
}