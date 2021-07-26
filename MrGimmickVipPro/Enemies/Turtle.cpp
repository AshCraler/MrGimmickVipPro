#include "Turtle.h"
#include "../Gimmick.h"
#include "../StaticObjects/Ground.h"

Turtle::Turtle(int x, int y, int x_from, int x_to) {
	this->x = x;
	this->y = y;
	this->xFrom = x_from;
	this->xTo = x_to;
	vx = TURTLE_X_SPEED;
	direction = vx > 0 ? true : false;
	this->state = CRAWLING_STATE;
	width = TURTLE_WIDTH;
	height = TURTLE_HEIGHT;
	animation_set = CAnimationSets::GetInstance()->Get(TURTLE_ANIMATION_SET_ID);
}

Turtle::~Turtle() {}

void Turtle::Render() {
	if (state == TURTLE_DEATH_STATE) {
		int aniFrom = direction ? 0 : animation_set->size() / 2;
		animation_set->at(aniFrom + state)->Render(x, y+3);
	}
	else {
		int aniFrom = direction ? 0 : animation_set->size() / 2;
		animation_set->at(aniFrom + state)->Render(x, y);
	}
	/*int aniFrom = direction ? 0 : animation_set->size() / 2;
	animation_set->at(aniFrom + state)->Render(x, y);*/
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
	RenderBoundingBox();
}

void Turtle::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	if (state == TURTLE_DEATH_STATE && startDeath) {
		vy -= GIMMICK_GRAVITY * dt;
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
				if (e->ny != 0) {
					vy = 0;
					startDeath = false;
				}
			}
		}
	}

	if (x < xFrom) {
		x = xFrom;
		vx = TURTLE_X_SPEED;
		direction = !direction;
	}
	else if (x + width>xTo) {
		x = xTo - width;
		vx = -TURTLE_X_SPEED;
		direction = !direction;

	}
}
void Turtle::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void Turtle::GetTurnedUp() {
	if (state != TURTLE_DEATH_STATE) {
		this->state = TURTLE_DEATH_STATE;
		vx = 0;
		vy = TURTLE_Y_SPEED*10;
		startDeath = true;
	}
	
}