#include "PinkWorm.h"
#include "../Gimmick.h"

PinkWorm::PinkWorm(int x, int y, int x_from, int x_to) {
	this->x = x;
	this->y = y;
	this->xFrom = x_from;
	this->xTo = x_to;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = direction ? 0.005 : -0.005;
	this->state = CRAWLING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(22);
}

PinkWorm::~PinkWorm() {}

void PinkWorm::Render() {
	int aniFrom = vx>0 ? 0 : animation_set->size() / 2;
	animation_set->at(aniFrom + state)->Render(x, y);
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void PinkWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == CRAWLING_STATE) {
		CGameObject::Update(dt);
		x += dx;
		if (x < xFrom) {
			vx = 0.005;
		}if (x + 23 > xTo) {
			vx = -0.005;
		}
	}
	else {
		CGameObject::Update(dt);
		vy -= 0.00001 * dt;
		x += dx;
		y += dy;
	}
}
void PinkWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == CRAWLING_STATE) {
		left = x;
		right = x + 23;
		top = y;
		bottom = y - 14;
	}
	else {
		left = right = bottom = top = 0;
	}
}
void PinkWorm::GetHit(bool dir) {
	vy = 0.02;
	vx = dir ? 0.01f : -0.01f;
	state = 1;
}