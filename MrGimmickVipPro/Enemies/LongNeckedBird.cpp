#include "LongNeckedBird.h"
#include "../Gimmick.h"
LongNeckedBird::LongNeckedBird(int x, int y,  bool right) {
	this->x = x;
	this->y = y;
	this->direction = right;
	//vx = direction ? X_SPEED : -X_SPEED;
	this->state = LNB_STANDING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(LNB_ANIMATION_SET_ID);
	timeToBeReady = NULL;
}

LongNeckedBird::~LongNeckedBird() {}

void LongNeckedBird::Render() {
	int aniFrom = direction ? 0 : animation_set->size() / 2;
	animation_set->at(aniFrom + state)->Render(x, y);
	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void LongNeckedBird::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state != FLYING_STATE) {
		left = x;
		top = y -MARGIN_TO_TOP_WHEN_STANDING;
		right = left + WIDTH_STANDING;
		bottom = top - HEIGHT_STANDING;
	}
	else {
		if(direction)
			left = x+MARGIN_TO_LEFT_WHEN_FLYING;
		else
			left = x + MARGIN_TO_LEFT_WHEN_FLYING + 15;

		top = y - MARGIN_TO_TOP_WHEN_FLYING;
		right = left + WIDTH_FLYING;
		bottom = top - HEIGHT_FLYING;
	}
	
}

void LongNeckedBird::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	if (direction == true) {
		x += dx;
		y += dy;
		if (timeToBeReady == NULL) {
			if (x - CGimmick::GetInstance()->x < X_RANGE) {
				timeToBeReady = GetTickCount();
			}
		}
		else {
			if (state == LNB_STANDING_STATE && GetTickCount() - timeToBeReady > TIME_TO_BE_READY) {
				state = READY_STATE;
				readyTimer = GetTickCount();
			}
			else if (state == LNB_STANDING_STATE && x > 330) {
				state = READY_STATE;
				readyTimer = GetTickCount();
			}
			else if (state == READY_STATE && GetTickCount() - readyTimer > READY_TIME) {
				x = X_TO_FLYING_RIGHT;
				y = Y_TO_FLYING_RIGHT;
				vx = LNB_X_SPEED;
				state = FLYING_STATE;
			}
			else if (state == FLYING_STATE && x- X_TO_FLYING_RIGHT>FLYING_RANGE) {
				disappear = true;
				vy = LNB_X_SPEED;
			}
		}
		
		if (state != FLYING_STATE) {
			beingPushed = false;
			vx = 0;
		}
		else {
			if (!disappear) {
				if (vy > 0) {
					if (vy < LNB_Y_SPEED && !goDown) {
						vy += 0.000005 * dt;
					}
					else {
						if (vy > LNB_Y_SPEED) {
							goDown = true;
						}
						vy -= 0.000005 * dt;

					}
				}
				else {
					if (vy > -LNB_Y_SPEED && goDown) {
						vy -= 0.000005 * dt;
					}
					else {
						if (vy < -LNB_Y_SPEED) {
							goDown = false;
						}
						vy += 0.000005 * dt;

					}
				}
			}
		}
	}
	else {
		x += dx;
		y += dy;
		if (timeToBeReady == NULL) {
			if (x - CGimmick::GetInstance()->x < X_RANGE) {
				timeToBeReady = GetTickCount();
			}
		}
		else {
			if (state == LNB_STANDING_STATE && GetTickCount() - timeToBeReady > TIME_TO_BE_READY) {
				state = READY_STATE;
				readyTimer = GetTickCount();
			}
			else if (state == READY_STATE && GetTickCount() - readyTimer > READY_TIME) {
				x = X_TO_FLYING_LEFT;
				y = Y_TO_FLYING_LEFT;
				vx = -LNB_X_SPEED;
				state = FLYING_STATE;
			}
			else if (state == FLYING_STATE &&  - x + X_TO_FLYING_LEFT > FLYING_RANGE_OF_LEFT) {
				//disappear = true;
				vx = 0;
				//vy = 0;
				//vy = -LNB_X_SPEED;
			}
		}

		if (state != FLYING_STATE) {
			beingPushed = false;
			vx = 0;
		}
		else {
			if (!disappear) {
				if (vy > 0) {
					if (vy < LNB_Y_SPEED && !goDown) {
						vy += 0.000005 * dt;
					}
					else {
						if (vy > LNB_Y_SPEED) {
							goDown = true;
						}
						vy -= 0.000005 * dt;

					}
				}
				else {
					if (vy > -LNB_Y_SPEED && goDown) {
						vy -= 0.000005 * dt;
					}
					else {
						if (vy < -LNB_Y_SPEED) {
							goDown = false;
						}
						vy += 0.000005 * dt;

					}
				}
			}
		}
	}
}