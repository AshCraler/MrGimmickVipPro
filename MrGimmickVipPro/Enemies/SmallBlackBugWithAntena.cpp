#include "SmallBlackBugWithAntena.h"
#include "../Gimmick.h"
#include "../StaticObjects/Ground.h"
#include "Utility/JumpingCommandBox.h"

SmallBlackBugWithAntena::SmallBlackBugWithAntena(int x, int y) {
	this->x = x;
	this->y = y;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = direction ? BBA_X_SPEED : -BBA_X_SPEED;
	this->state = INACTIVE_STATE;
	this->width = 16;
	this->height = 16;
	animation_set = CAnimationSets::GetInstance()->Get(BBA_ANIMATION_SET_ID);
}

SmallBlackBugWithAntena::~SmallBlackBugWithAntena() {}

void SmallBlackBugWithAntena::Render() {
	if (state != INACTIVE_STATE) {
		int aniFrom =direction? 0 : animation_set->size() / 2;
	
		if(state!=STOOD_ON_STATE && state!=BBA_STATE_In_PIPE)
			animation_set->at(aniFrom + state)->Render(x, y+6);
		else 
			animation_set->at(aniFrom + 0)->Render(x, y+6);

		if (state == DEFENDING_STATE) {
			animation_set->at(aniFrom + 5)->Render(x-8, y+13);
			//animation_set->at(aniFrom + 5)->Render(x+8, y);
		}
		if (charging) {
			animation_set->at(aniFrom + 4)->Render(x, y + 13);

		}
	}
	
	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void SmallBlackBugWithAntena::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == DEATH_STATE) {
		CGameObject::Update(dt);
		vy -= GIMMICK_GRAVITY * dt;
		x += dx;
		y += dy;
	}
	else if (state != INACTIVE_STATE && state!=DEATH_STATE) {
		CGameObject::Update(dt);
		if(state!=STOOD_ON_STATE)
			vy -= GIMMICK_GRAVITY * dt;

		if (state == WALKING_STATE) {
			if (!charging && GetTickCount() - charging_timer > TIME_TO_START_CHARGING) {
				charging = true;
				charging_timer = GetTickCount();
			}
			else if (charging && GetTickCount() - charging_timer > CHARGING_TIME) {
				SetDefending();
				charging = false;
			}
		}
		else if (state == DEFENDING_STATE) {
			CGimmick* g = CGimmick::GetInstance();
			bool o = !((g->x + g->width < x - 15) || (x + width + 15 < g->x) || (g->y - g->height > y) || (y - height > g->y));
			if (x + width + 15 > g->x) {
				state = state;
			}
			if (o)
				state = state;
			if (!((g->x + g->width<x - 15) || (x + width + 15 < g->x) || (g->y - g->height > y) || (y - height>g->y))) {
				g->GetInjured(INJURED_BY_ENEMY, g->x > x);
			}
		}
		if (charging) {
			CGimmick* g = CGimmick::GetInstance();

			if (!((g->x + g->width < x) || (x + width < g->x) || (g->y - g->height > y+10) || (y > g->y))) {
				g->GetInjured(INJURED_BY_ENEMY, g->x > x);
			}
		}


		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(colliable_objects, coEvents);

		
		if (coEvents.size() == 0|| state == BBA_STATE_In_PIPE) {
			if (state != STOOD_ON_STATE) {
				
				if (state == BBA_STATE_In_PIPE) {
					Pipe* p, * pipe = NULL;
					bool stillInPipe = false;
					int minDistance = 99999, dumpDistance;
					for (UINT i = 0; i < colliable_objects->size(); i++) {
						if (dynamic_cast<Pipe*>(colliable_objects->at(i))) {
							p = dynamic_cast<Pipe*>(colliable_objects->at(i));
							if (!(p->x > x + width || x > p->x + p->width || y - height > p->y || p->y - p->height > y)) {
								stillInPipe = true;
								int d = pow(x - p->x, 2) + pow(y - p->y, 2);
								if (pow(x - p->x, 2) + pow(y - p->y, 2) < minDistance) {
									pipe = p;
									minDistance = pow(x - p->x, 2) + pow(y - p->y, 2);
								}
							}
						}
					}
					if (pipe != NULL) {
						if (pipe->GetType() == 24) {
							if (vx == 0) vx = BBA_X_SPEED;
							x += dx;
							if (vy != 0)vy = 0;
						}
						else if (pipe->GetType() == 34) {
							if (abs(x + width - (pipe->x + pipe->width)) < 0.1) {
								vx = 0;
								vy = -BBA_X_SPEED;
								y += dy;
							}
							else {
								x += dx;
								if (x + width > (pipe->x + pipe->width)) {
									x = pipe->x;
									vx = 0;
									vy = -BBA_X_SPEED;
								}
							}
						}
						else if (pipe->GetType() == 13) {
							if (vy == 0) vy = -BBA_X_SPEED;
							y += dy;
							if (vx != 0)vx = 0;
						}
					}
					
				}
				else {
					if (state != JUMPING_STATE && vyIncreasedOnce)
						state = JUMPING_STATE;
					x += dx;
					y += dy;
					if (!vyIncreasedOnce)vyIncreasedOnce = true;

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
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Ground*>(e->obj)) {
					if (e->ny != 0) {
						if (state != DEFENDING_STATE && state!=BBA_STATE_In_PIPE) {
							state = WALKING_STATE;
							CGimmick* g = CGimmick::GetInstance();
							if (!g->standingOnSBB && ((x + 5 > g->GetX() && vx >= 0) || (x + width < g->GetX() + 5 && vx <= 0)) && GetTickCount() - changingDirectionTimer > TIME_TO_CHANGE_DIRECTION && !noReturn) {
								vx = x > CGimmick::GetInstance()->GetX() ? -BBA_X_SPEED : BBA_X_SPEED;
								changingDirectionTimer = GetTickCount();
								
							}if (noReturn) {
								vx = BBA_X_SPEED;
							}
							//vx = x > CGimmick::GetInstance()->GetX() ? -BBA_X_SPEED : BBA_X_SPEED;
							direction = vx > 0;
							vy = 0;
							vyIncreasedOnce = false;
							//charging_timer = GetTickCount();

						}
						

					} 
				}else if(dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)) {
					if ((e->nx < 0 && !dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->direction) ||
						(e->nx > 0 && dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->direction)) {
						noReturn = dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->returnOrNot;
						vx = BBA_X_SPEED;
						if (!noReturn)
							changingDirectionTimer = GetTickCount();
					}
					if (e->ny != 0) {
						y = y - (min_ty * dy - ny * 0.1f) + dy;

					}
					x = x + dx - (min_tx * dx + nx * 0.1f);

				}
				else if (dynamic_cast<ReturningCommandBox*>(e->obj)) {
					
					vx = -vx;
					changingDirectionTimer = GetTickCount();
					direction = vx > 0;
					if (e->ny != 0) {
						y = y - (min_ty * dy - ny * 0.1f) + dy;

					}
					x = x + dx - (min_tx * dx + nx * 0.1f);

				}
				
				else if (dynamic_cast<Pipe*>(e->obj)) {
					Pipe* pipe = dynamic_cast<Pipe*>(e->obj);
					state = BBA_STATE_In_PIPE;
					if (e->nx != 0) {
						x = pipe->x + 1 - width;
						vy = 0;
					
					}
					else {
						y = pipe->y + height - 1;
						vx = 0;
						vy = 0;
					}
					
				}

				
				else if (dynamic_cast<CGimmick*>(e->obj)) {
					x = x;
				}
			}
		}
		if (GetTickCount() - defending_timer > DENFENDING_TIME && state==DEFENDING_STATE) {
			if (coEvents.size() == 0)
				state = JUMPING_STATE;
			else {
				state = WALKING_STATE;
				//vx = x > CGimmick::GetInstance()->GetX() ? -BBA_X_SPEED : BBA_X_SPEED;
				//direction = vx > 0;
				CGimmick* g = CGimmick::GetInstance();
				if (!g->standingOnSBB && ((x + 5 > g->GetX() && vx >= 0) || (x + width < g->GetX() + 5 && vx <= 0)) && GetTickCount() - changingDirectionTimer > TIME_TO_CHANGE_DIRECTION && !noReturn) {
					vx = x > CGimmick::GetInstance()->GetX() ? -BBA_X_SPEED : BBA_X_SPEED;
					changingDirectionTimer = GetTickCount();
					direction = vx > 0;
				}if (noReturn) {
					vx = BBA_X_SPEED;
				}
				charging_timer = GetTickCount();
			}
			
		}
	}
}
void SmallBlackBugWithAntena::SetDefending() { 
	if (state != DEFENDING_STATE && state!=BBA_STATE_In_PIPE) {
		state = DEFENDING_STATE;
		defending_timer = GetTickCount();
		vx = 0;
	}
	
}
void SmallBlackBugWithAntena::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == DEATH_STATE) {
		left = right = 0;
		top = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = left + WIDTH;
		bottom = top - HEIGHT;
	}
	
}
void SmallBlackBugWithAntena::GetStoodOn() {
	state = STOOD_ON_STATE;
	vx = 0;
	vy = 0;
}
void SmallBlackBugWithAntena::GetReleased() {
	state = WALKING_STATE;
	vx = x > CGimmick::GetInstance()->GetX() ? -BBA_X_SPEED : BBA_X_SPEED;
	charging=false;
	direction = vx > 0;
	charging_timer = GetTickCount();
}
void SmallBlackBugWithAntena::GetHit(bool dir) {
	state = DEATH_STATE;
	this->direction = true;
	vx = dir ? SBB_VX_WHEN_HIT : -SBB_VX_WHEN_HIT;
	vy = SBB_Y_SPEED;

}