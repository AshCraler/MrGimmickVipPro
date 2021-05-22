#include "SmallBlackBug.h"
SmallBlackBug::SmallBlackBug(int x, int y, int jar) {
	this->x = x;
	this->y = y;
	this->carryJar = jar;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = direction ? X_SPEED : -X_SPEED;
	this->state = WALKING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(ANIMATION_SET_ID);
}

SmallBlackBug::~SmallBlackBug(){}

void SmallBlackBug::Render() {
	int aniFrom = direction ? 0 : animation_set->size() / 2;
	animation_set->at(aniFrom + state)->Render(x, y);
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void SmallBlackBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {

}