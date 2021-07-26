#include "BrownStuff.h"
#include "../../StaticObjects/Ground.h"
#include "../../Gimmick.h"
#include "../../SceneManager.h"

Sword::Sword(int x, int y) {
	this->x = x;
	this->y = y;
	this->state = 0;
	animation_set = CAnimationSets::GetInstance()->Get(SWORD_ANIMATION_SET_ID);
}

Sword::~Sword() {}

void Sword::Render() {
	animation_set->at(state)->Render(x, y);
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
	RenderBoundingBox();
}
void Sword::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + 22;
	bottom = top - 24;
}
void Sword::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vy -= 0.000015 * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	if (collidedWithGround && GetTickCount() - timeToRelease > 5000) {
		CGimmick::GetInstance()->GetLocked(false);
		CMenu::GetInstance()->win = 1;
		CMenu::GetInstance()->nextStage++;

		CSceneManager::GetInstance()->SwitchScene(41);
	}
	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

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
					if (state <= 3) {
						if (collidedWithGround) {
							x = x - min_tx * dx - nx * 0.1f+dx;
							y = y - min_ty * dy + ny * 0.1f+dy;
						}
						else {
							collidedWithGround = true;
							vy = SWORD_REBELLING_SPEED;
							vx = vx<0?-SWORD_REBELLING_SPEED: SWORD_REBELLING_SPEED;

						}
					}
					else {
						vx = vy = 0;
						collidedWithGround = true;
						timeToRelease = GetTickCount();
						state = 4;
					}
				}
			}
		}
	}
	if (!collidedWithGround) {
		if (GetTickCount() - switchingSpriteTimer > SWORD_TIME_TO_SWITCH_SPRITE) {
			state = (state + 1) % 6;
			switchingSpriteTimer = GetTickCount();
		}
	}
}

BrownStuff::BrownStuff(int x, int y) {
	this->x = x;
	this->y = y;
	this->state = BS_INACTIVE_STATE;
	width = BS_WIDTH;
	height = BS_HEIGHT;
	animation_set = CAnimationSets::GetInstance()->Get(BS_ANIMATION_SET_ID);
	remainingHit = BS_SURVIVING_TIMES;
}

BrownStuff::~BrownStuff() {}

void BrownStuff::Render() {
	if (state != BS_INACTIVE_STATE) {
		//animation_set->at(state)->Render(x, y);
		if(state==BS_ATTACKING_STATE)
			animation_set->at(state)->Render(x-12, y);

		else {
			if (state == BS_DEFEATED) {
				sword->Render();
			}
			animation_set->at(state)->Render(x, y);

		}

		/*switch (state) {
		case WALKING_STATE:
			animation_set->at(case)
		}*/
		RenderBoundingBox();
	}
	
}

void BrownStuff::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == BS_INACTIVE_STATE && x - CGimmick::GetInstance()->x < BS_MAX_DISTANCE_TO_GM) {
		state = JUMPING_STATE;
		vx = 0;
		vy = 0;
		waitToAttack = GetTickCount();
	}
	else if (state != BS_INACTIVE_STATE && state != BS_DEFEATED) {
		CGameObject::Update(dt);
		vy -= 0.000015 * dt;

		CGimmick* g = CGimmick::GetInstance();
		if (state==BS_WALKING_STATE || state == BS_BEING_INJURED ||  state==BS_JUMPING_STATE) {
			if (x > g->x + g->width + 10) {
				vx = -BS_X_SPEED;
			}
			else  if (x + 10 > g->x + g->width) {
				vx = BS_X_SPEED;

			}
			else
				vx = BS_X_FAST_SPEED;
		}
		

		if (state == BS_ATTACKING_STATE && GetTickCount() - attackingTimer > ATTACKING_TIME&&vy>0) {
			state = BS_JUMPING_STATE;
		}
		if (g->IsJumping()) {
			if (g->IsHavingStar()) {
				if (x - g->x - g->width > 10 && state==BS_WALKING_STATE) {
					if ((rand() % 4 < 3 || GetTickCount() - decideToJumpTimer < TIME_TO_DECIDE_NEXT_JUMP) && state == BS_WALKING_STATE) {
						vx = BS_X_SPEED;

					}
					else {
						state = BS_JUMPING_STATE;
						vy = BS_Y_SPEED;
						vx = -BS_X_FAST_SPEED;
						decideToJumpTimer = GetTickCount();
					}
				}

			}

		}

		if (x > (g->x + g->width) && x - (g->x + g->width) < 10) {
			if (GetTickCount() - waitToAttack > WAIT_FOR_NEXT_ATTACK) {
				state = BS_ATTACKING_STATE;
				vx = 0;
				waitToAttack = GetTickCount();
				attackingTimer = GetTickCount();
				g->GetInjured(INJURED_BY_ENEMY, false);
			}
			
		}

		if (g->IsHavingStar() && state == BS_JUMPING_STATE) {
			if (x - 10< g->x + g->width && x - 10 > g->x && y - 5< g->y - STAR_Y_MARGIN && y - 5> g->y - STAR_Y_MARGIN - STAR_HEIGHT) {
				state = BS_ATTACKING_STATE;
				vx = BS_X_SPEED;
				waitToAttack = GetTickCount();
				attackingTimer = GetTickCount();
			}
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(colliable_objects, coEvents);

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
						if (state == BS_JUMPING_STATE || (state == BS_ATTACKING_STATE && GetTickCount() - attackingTimer > ATTACKING_TIME)) {
							state = BS_WALKING_STATE;
							waitToAttack = GetTickCount();

						}
						else {
							vx = 0;
						}
					}
				}
			}
		}
	}
	else if(state == BS_DEFEATED){
		CGameObject::Update(dt);
		vy -= 0.000015 * dt;
		x += dx;
		y += dy;
		sword->Update(dt, colliable_objects);
	}
	
}
void BrownStuff::GetHit() {
	state = BS_BEING_INJURED;
	vx = BS_X_SPEED;
	vy = BS_Y_SPEED;
	untouchableTimer = GetTickCount();
	remainingHit -= 1;
	if (remainingHit <= 0) {
		state = BS_DEFEATED;
		vx = BS_X_SPEED;
		vy = BS_Y_SPEED;
		sword = new Sword(x, y);
		sword->vx = vx;
		sword->vy = BS_Y_SPEED * (1+0.1*(rand()%10));
	}
	
}
bool BrownStuff::ProcessStars(Star* s, float nx, float ny) {
	if (state == BS_WALKING_STATE) {
		if (nx != 0) {
			state = BS_ATTACKING_STATE;
			waitToAttack = GetTickCount();
			attackingTimer = GetTickCount();
			return true;
		}
		else {
			if (ny > 0) {
				if (s->x < x + 5) {
					state = BS_ATTACKING_STATE;
					waitToAttack = GetTickCount();
					attackingTimer = GetTickCount();
					return true;
				}
				else {
					GetHit();
					return false;
				}
			}
			else {
				GetHit();
				return false;
			}

		}
	}
	else if (state == BS_JUMPING_STATE) {
		state = BS_ATTACKING_STATE;
		return true;

	}
	
}
void BrownStuff::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}