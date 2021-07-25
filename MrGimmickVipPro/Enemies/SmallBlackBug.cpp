#include "SmallBlackBug.h"
#include "../StaticObjects/Ground.h"
#include "../StaticObjects/Elevator.h"
#include "Utility/JumpingCommandBox.h"
#include "../Gimmick.h"
#include "../Items/Jar.h"
#include "../StaticObjects/Pipe.h"

BombCaryingBug::BombCaryingBug(int x, int y, int xf, int xt, Jar* j) {
	this->x = x;
	this->y = y;
	this->width = WIDTH;
	this->height = 16;
	xFrom = xf;
	xTo = xt;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = BCB_X_SPEED;
	jar = j;
	this->state = BCB_WALKING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(SSB_ANIMATION_SET_ID);
}

BombCaryingBug::~BombCaryingBug() {}
void BombCaryingBug::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == BCB_WALKING_STATE) {
		left = x;
		top = y;
		right = left + WIDTH;
		bottom = top - 16;
	}
	else {
		left = right = 0;
		top = bottom = 0;
	}
}

void BombCaryingBug::Render() {
	int aniFrom = direction ? 0 : animation_set->size() / 2;
	if (state == SSB_DEATH_STATE) aniFrom = 0;
	animation_set->at(aniFrom + state)->Render(x, y);
	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}
void BombCaryingBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vy -= dt * 0.00005;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);


	if (coEvents.size() == 0 || state== SSB_DEATH_STATE) {
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
			if (dynamic_cast<BombCaryingBug*>(e->obj)) {
				x = x - min_tx * dx - nx * 0.1f+dx;
				if(e->ny!=0)
					y = y - min_ty * dy + ny * 0.1f+dy;
				
			}
			if (e->ny != 0)vy = 0;
		}
	}
	if ((vx > 0 && x +width > xTo) 
		//|| (vx < 0 && x > CGimmick::GetInstance()->GetX() + BCB_MAX_DISTANCE_TO_GIMMICK)
		) {
		vx = -BCB_X_SPEED;
		direction = false;
	}
	else if ((vx < 0 && x < xFrom)
		//||(vx<0&&x< CGimmick::GetInstance()->GetX()-BCB_MAX_DISTANCE_TO_GIMMICK)
		) {
		vx = BCB_X_SPEED;
		direction = true;
	}
}

void BombCaryingBug::GetHit(bool dir) {
	this->state = BCB_DEATH_STATE;
	vx = dir ? BCB_X_SPEED : -BCB_X_SPEED;
	vy = BCB_Y_SPEED;
	if (jar != NULL)
		jar->Appear(x,y,dir);
	//shell->GetReleased();
}

StandingStillBug::StandingStillBug(int x, int y) {
	this->x = x;
	this->y = y;
	this->width = WIDTH;
	this->height = 16;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = 0;
	this->state = SSB_STANDING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(SSB_ANIMATION_SET_ID);
}

StandingStillBug::~StandingStillBug() {}
void StandingStillBug::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == SSB_STANDING_STATE) {
		left = x;
		top = y;
		right = left + WIDTH;
		bottom = top - 16;
	}
	else {
		left = right = 0;
		top = bottom = 0;
	}
}

void StandingStillBug::Render() {
	int aniFrom = direction ? 0 : animation_set->size() / 2;
	if (state == SSB_DEATH_STATE) aniFrom = 0;
	animation_set->at(aniFrom + state)->Render(x, y);
	RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}
void StandingStillBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	CGameObject::Update(dt);
	vy -= dt * 0.00005;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);


	if (coEvents.size() == 0 || state == SSB_DEATH_STATE) {
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
			if (e->ny != 0) {
				vy = 0;
			}
		}
	}
	if (direction && CGimmick::GetInstance()->GetX() < x - 10)direction = false;
	else if (!direction && CGimmick::GetInstance()->GetX() > x + 10)	direction = true;
}

void StandingStillBug::GetHit(bool dir) {
	this->state = SSB_DEATH_STATE;
	vx = dir ? SSB_X_SPEED : -SSB_X_SPEED;
	vy = SSB_Y_SPEED;
}

SmallBlackBug::SmallBlackBug(int aniId, int x, int y,bool mv, Jar* jar, bool t) {
	this->x = x;
	this->y = y;
	this->jar = jar;
	this->moving = mv;
	this->width = WIDTH;
	this->height = 16;
	this->toX = t;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = 0;
	this->state = INACTIVE_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(aniId);
}

SmallBlackBug::~SmallBlackBug(){}

void SmallBlackBug::GoDead(bool sd) {
	state = DEATH_STATE;
	width = height = 0;
	if (sd)vx = SBB_X_SPEED;
	else vx = -SBB_X_SPEED;
	vy = SBB_Y_SPEED;
}
void SmallBlackBug::Render() {
	if (state != INACTIVE_STATE) {
		if (state != SBB_IN_PIPE_STATE) {
			int aniFrom = vx >= 0 ? 0 : animation_set->size() / 2;
			if (state == JUMPING_STATE)
				state = state;
			animation_set->at(aniFrom + state)->Render(x, y);
			RenderBoundingBox();
		}
		else {
			int aniFrom = vx >= 0 ? 0 : animation_set->size() / 2;
			
			animation_set->at(aniFrom+3)->Render(x, y);
			RenderBoundingBox();
		}
		
	}
	
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}
void SmallBlackBug::SetVX(float vx) { 

	if (vx * this->vx < 0)
		vx = vx;
	this->vx = vx; 
}
void SmallBlackBug::SetVY(float _vy) { 
	/*if (onHill == Touching && abs(this->vx / _vy) != hill->GetXToYRatio())
		x = x;*/
	this->vy = _vy; 
}
void SmallBlackBug::SetX(float x) { this->x = x; }
void SmallBlackBug::SetY(float y) { this->y = y; }
void SmallBlackBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == INACTIVE_STATE && abs(x - CGimmick::GetInstance()->GetX()) < 150) {
		state = WALKING_STATE;
		/*if (!carryingGimmick) {
			vx = x > CGimmick::GetInstance()->GetX() ? -X_SPEED : X_SPEED;
			SetVY(0);
		}*/
		SetVX(x > CGimmick::GetInstance()->GetX() ? -SBB_X_SPEED : SBB_X_SPEED);
		//SetVX(0);
		SetVY(0);
	}
	else if(abs(x - CGimmick::GetInstance()->GetX()) > 250) state = INACTIVE_STATE;
	if (state != INACTIVE_STATE && state != DEATH_STATE) {
		if (onHill == 0)
			SetVY(vy - 0.00005 * dt);
		CGimmick* g = CGimmick::GetInstance();
		if (vy < -0.01f && onHill == OutOfArea) {
			state = JUMPING_STATE;

		}
		if (!g->standingOnSBB && ((x +5 > g->GetX() && vx >= 0) || (x +width< g->GetX()+5 && vx <= 0)) 
			&& GetTickCount() - changingDirectionTimer > TIME_TO_CHANGE_DIRECTION &&!noReturn && state!=JUMPING_STATE) {
			SetVX(-vx);
			changingDirectionTimer = GetTickCount();
			if (onHill == Touching) {
				SetVY(-vy);
			}
		}
		else if (g->standingOnSBB && GetTickCount() - changingDirectionTimer > TIME_TO_CHANGE_DIRECTION && vx > 0 && !noReturn && state != JUMPING_STATE) {
			SetVX(-vx);
			changingDirectionTimer = GetTickCount();
			if (onHill == Touching) {
				if (hill->isLeftUpToRight()) {
					if (vx > 0) {
						SetVX(SBB_X_SPEED * (1 - hill->GetSpeedReductionFactor()));
						SetVY(vx / hill->GetXToYRatio());
					}
					else {
						SetVX(-SBB_X_SPEED * (1 + hill->GetSpeedReductionFactor()));
						SetVY(vx / hill->GetXToYRatio());
					}
				}else {
					if (vx > 0) {
						SetVX(SBB_X_SPEED * (1 + hill->GetSpeedReductionFactor()));
						SetVY(-vx / hill->GetXToYRatio());
					}
					else {
						SetVX(-SBB_X_SPEED * (1 - hill->GetSpeedReductionFactor()));
						SetVY(-vx / hill->GetXToYRatio());
					}
				}
			}
		}

		CGameObject::Update(dt);


		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);

		if (onHill == Enter) {
			if (hill->isLeftUpToRight()) {
				if (vx < 0
					&& (x + 11 - hill->x) / abs(-hill->height + hill->y - y + height) < hill->GetXToYRatio()
					&& y - height > hill->y - 2) {
					x = (abs(-hill->height + hill->y + height - y) * hill->GetXToYRatio() + hill->x - 11);
					onHill = Touching;
					SetVX(-SBB_X_SPEED * (1 + hill->GetSpeedReductionFactor()));
					SetVY(vx / hill->GetXToYRatio());

				}
				else if (
					vx > 0
					&& (x + 11 - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()
					&& y - height < hill->y - hill->height / 3) {
					y = (((x + 11 - hill->x) / hill->GetXToYRatio()) + hill->y - hill->height + height);
					onHill = Touching;
					SetVX(SBB_X_SPEED * (1 - hill->GetSpeedReductionFactor()));
					SetVY(vx / hill->GetXToYRatio());
				}
				else {
					if (vx < 0)
						vx = vx;
					x += dx;
					//y += dy;
				}
			}
			else {
				if (vx<0
					&& (-x - (-5) + hill->width + hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()
					&& y - height < hill->y - hill->height / 3) {
					y = ((-(-x - (-5) + hill->width + hill->x) / hill->GetXToYRatio()) + hill->y - hill->height + height);
					//y = (((-x - 3 + hill->width + hill->x) / hill->GetXToYRatio()) + hill->y + hill->height - height);
					onHill = Touching;
					SetVX(-SBB_X_SPEED * (1 - hill->GetSpeedReductionFactor()));
					SetVY(-vx / hill->GetXToYRatio());

				}
				else if (vx > 0
					&& ((-x - 5 + hill->width + hill->x) / abs(-hill->height + hill->y - y + height) < hill->GetXToYRatio())
					&& (y - height > hill->y - hill->height / 3)) {


					x = ((-hill->height + hill->y + height - y) * hill->GetXToYRatio() + hill->x + hill->width - 5);
					onHill = Touching;
					SetVX(SBB_X_SPEED * (1 + hill->GetSpeedReductionFactor()));
					SetVY(-vx / hill->GetXToYRatio());
				}
				/*if ((-x  + hill->width + hill->x) / (hill->height + hill->y - y - height) > hill->GetXToYRatio()) {
					y = ((hill->height + hill->y - height - (-x + hill->width + hill->x) / hill->GetXToYRatio()));
					onHill = Touching;
					vx = SBB_X_SPEED * hill->GetSpeedReductionFactor();
					vy = vx / hill->GetXToYRatio();

				}*/
				else {
					if (vx < 0)
						vx = vx;
					x += dx;
					//y += dy;
				}
			}
			if (x > hill->width + hill->x || (x > hill->width + hill->x) || x + height < hill->x || (vy > 0 && y - height > hill->y) || (vy < 0 && y - height < hill->y - hill->height)) {
				onHill = OutOfArea;
				if (hill->isLeftUpToRight()) {
					if (vx > 0) {
						SetVX(SBB_X_SPEED);
						SetVY(0);
						if (y - height < hill->y - hill->height / 3)
							y = hill->y - hill->height + 1 + height;
						else
							y = hill->y + height + 1;

						x += dx;
					}
					else {
						SetVX(-SBB_X_SPEED);
						SetVY(0);
						if (y - height < hill->y - hill->height / 3)
							y = hill->y - hill->height + 1 + height;
						else
							y = hill->y + height + 1;

						x += dx;
					}
					/*SetVX(SBB_X_SPEED);
					SetVY(0);
					if (y - height < hill->y - hill->height / 3)
						y = hill->y - hill->height + 1 + height;
					else
						y = hill->y + height + 1;

					x += dx;*/
				}
				else {
					SetVX(SBB_X_SPEED);
					//y = hill->y - height - 1;
					if (y - height < hill->y - hill->height / 3)
						y = hill->y - hill->height + height + 1;
					else
						y = hill->y + height + 1;
					x += dx;
					SetVY(0);
				}
				/*SetX(x + dx);
				SetY(y - dt * vy);*/
				//SetVY(GIMMICK_GRAVITY * dt);
			}
		}
		else if (onHill == Touching) {
			if (x > hill->width + hill->x ||
				(vx < 0 && !hill->isLeftUpToRight() && x + 3 < hill->x) ||
				(vx < 0 && hill->isLeftUpToRight() && x + 13 < hill->x) ||
				(vy > 0 && y - height > hill->y) || (vy < 0 && y - height < hill->y - hill->height)) {
				if (x + width < 400)
					x = x;
				DebugOut(L"x %f\n", x);

				onHill = Enter;

				x = (x + dx);
				SetVY(0);
				//SetY(y + dy);
			}
			else {
				DebugOut(L"x %f\n", x + width - hill->x);
				if (x + width - hill->x < 0.5)
					x = x;
				x = (x + dx);
				y = (y + dy);
			}
		}
		else if (coEvents.size() == 0) {
			x = x + dx;
			y = y + dy;

			if (onHill == OutOfArea && state!=JUMPING_STATE)
				state = WALKING_STATE;
			Pipe* p, * pipe=NULL;
			bool stillInPipe = false;
			if (state == SBB_IN_PIPE_STATE) {
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
			}

			if (stillInPipe) {
				if (pipe->GetType() == top_bottom) {
					vy = -0.005;
					x = pipe->x;
				}
				/*if (dx > 0) {
					if (pipe->GetType() == top_left || pipe->GetType() == top_bottom || pipe->GetType() == bottom_left) {
						if (x > pipe->x) {
							SetX(pipe->x);
						}
					}

				}
				else if (pipe->GetType() == top_right || pipe->GetType() == top_bottom || pipe->GetType() == right_bottom) {
					if (x < pipe->x) {
						SetX(pipe->x);
					}

				}
				if (dy > 0) {

					if (pipe->GetType() == bottom_left || pipe->GetType() == right_left || pipe->GetType() == right_bottom) {
						if (y > pipe->y) {
							SetY(pipe->y);
						}
					}

				}
				else if (dy < 0) {
					if (pipe->GetType() == top_left || pipe->GetType() == top_right || pipe->GetType() == right_left) {
						if (y < pipe->y) {
							SetY(pipe->y);
						}
					}

				}*/
			}
			
			
		}
		else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		bool collideWithGroundOnY = false, collideWithHillOnY = false, ereaseGround = false;
		for (UINT i = 0; i < coEvents.size(); i++) {
			if (coEvents[i]->ny < 0) {
				if (dynamic_cast<Ground*>(coEvents[i]->obj))
					collideWithGroundOnY = true;
				if (dynamic_cast<Hill*>(coEvents[i]->obj)) {
					if ((vx < 0 && !dynamic_cast<Hill*>(coEvents[i]->obj)->isLeftUpToRight()) ||
						(vx > 0 && dynamic_cast<Hill*>(coEvents[i]->obj)->isLeftUpToRight())) {
						collideWithHillOnY = true;
					}
					else {
						ereaseGround = true;
					}
				}
			}
		}
		if (onHill != OutOfArea && collideWithGroundOnY && collideWithHillOnY) {
			for (UINT i = 0; i < coEvents.size(); i++) {
				if (coEvents[i]->ny < 0 && !dynamic_cast<Ground*>(coEvents[i]->obj))
					coEvents.erase(coEvents.begin() + i);
			}
		}
		else if (ereaseGround) {
			for (UINT i = 0; i < coEvents.size(); i++) {
				if (coEvents[i]->ny < 0 && dynamic_cast<Ground*>(coEvents[i]->obj))
					coEvents.erase(coEvents.begin() + i);
			}
		}
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + min_tx * dx + nx * 0.1f;
		y = y + min_ty * dy - ny * 0.1f;

		bool collideWithJumpingCommand = false;
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<JumpingCommandBox*>(e->obj)) {
				if ((e->nx > 0 && !dynamic_cast<JumpingCommandBox*>(e->obj)->direction) ||
					(e->nx < 0 && dynamic_cast<JumpingCommandBox*>(e->obj)->direction)) {
					SetVY(SBB_Y_SPEED);
					state = JUMPING_STATE;
					collideWithJumpingCommand = true;
					x = x + dx - (min_tx * dx + nx * 0.1f);

				}
				if (e->ny != 0) {
					y = y - (min_ty * dy - ny * 0.1f) + dy;
				}

			}
			else if (dynamic_cast<ReturningCommandBox*>(e->obj)) {
				if ((e->nx < 0 && !dynamic_cast<ReturningCommandBox*>(e->obj)->direction) ||
					(e->nx > 0 && dynamic_cast<ReturningCommandBox*>(e->obj)->direction)) {
					//collideWithJumpingCommand = true;
					if (!noReturn) {
						SetVX(-vx);
						changingDirectionTimer = GetTickCount();
						if (onHill == Touching) {
							SetVY(-vy);
						}
					}
					
				}
				else {
					x = x + dx - (min_tx * dx + nx * 0.1f);
				}
				if (e->ny != 0) {
					y = y - (min_ty * dy - ny * 0.1f) + dy;
				}

			}
			else if (dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)) {
				if ((e->nx < 0 && !dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->direction) ||
					(e->nx > 0 && dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->direction)) {
					noReturn = dynamic_cast<ReturnOrNotCommandbBox*>(e->obj)->returnOrNot;
					if (!noReturn)
						changingDirectionTimer = GetTickCount();
				}
				if (e->ny != 0) {
					y = y - (min_ty * dy - ny * 0.1f) + dy;

				}
				x = x + dx - (min_tx * dx + nx * 0.1f);

			}
			else if (dynamic_cast<SelectiveJumpingCommandBox*>(e->obj)) {
				if ((e->nx < 0 && !dynamic_cast<SelectiveJumpingCommandBox*>(e->obj)->direction) ||
					(e->nx > 0 && dynamic_cast<SelectiveJumpingCommandBox*>(e->obj)->direction)) {
					SetVY(SBB_Y_SPEED*1.5);
					state = JUMPING_STATE;
					collideWithJumpingCommand = true;
					//x = x + dx - (min_tx * dx + nx * 0.1f);
				}
				if (e->ny != 0) {
					y = y - (min_ty * dy - ny * 0.1f) + dy;

				}
				x = x + dx - (min_tx * dx + nx * 0.1f);

			}
			else if (dynamic_cast<Pipe*>(e->obj)) {
				Pipe* pipe = dynamic_cast<Pipe*>(e->obj);
				state = SBB_IN_PIPE_STATE;
				if (e->nx != 0) {
					x = pipe->x - width + 1;

				}
				else {
					y = pipe->y + height - 1;

				}
				CGimmick::GetInstance()->SetStandingOnSBB(false);
			}
			else if (dynamic_cast<SmallBlackBug*>(e->obj)) {
				
				x = x + dx - (min_tx * dx + nx * 0.1f);

				if(e->ny<0) {
					y = y - (min_ty * dy - ny * 0.1f) + dy;

				}
				CGimmick::GetInstance()->SetStandingOnSBB(false);
			}
			else if (dynamic_cast<StaticObject*>(e->obj)) {
				if (e->nx != 0) {
					if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
						if (carryingGimmick) {
							CGimmick::GetInstance()->x = x;
							CGimmick::GetInstance()->deltaSBB_x = 0;
						}
					}
					else if (dynamic_cast<Hill*>(e->obj)) {
						state = WALKING_STATE;
						onHill = Enter;
						hill = dynamic_cast<Hill*>(e->obj);
						if (e->nx != 0) {
							x = x + dx - (min_tx * dx + nx * 0.1f);

						}
						else
							y = y + dy - (min_ty * dy - ny * 0.1f);
					}
					else if (dynamic_cast<Elevator*>(e->obj)) {
						x = x + dx - (min_tx * dx + nx * 0.1f);
						if (vx > 0) {
							SetVX(dynamic_cast<Elevator*>(e->obj)->GetDirection() == true ?
								vx * (1 + 0.5) : vx * (1 - 0.5));
						}
						else {
							SetVX(dynamic_cast<Elevator*>(e->obj)->GetDirection() == false && vx > 0 ?
								-vx * (1 + 0.5) : -vx * (1 - 0.5));
						}

					}

				}
				if (e->ny != 0) {
					if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
						if (!collideWithJumpingCommand) {
							//SetVY(vy+dt*GIMMICK_GRAVITY);
							SetVY(0);
							state = WALKING_STATE;
						}

					}
					else if (dynamic_cast<Hill*>(e->obj)) {
						onHill = Enter;
						hill = dynamic_cast<Hill*>(e->obj);
						//x = x + dx - (min_tx * dx + nx * 0.1f);
						y = y + dy - (min_ty * dy - ny * 0.1f);
						state = WALKING_STATE;
						SetVY(0);

					}
					else if (dynamic_cast<Elevator*>(e->obj)) {
						y = y + dy - (min_ty * dy - ny * 0.1f);
						if (vx > 0) {
							SetVX(dynamic_cast<Elevator*>(e->obj)->GetDirection() == true ?
								vx * (1 + 0.5) : vx * (1 - 0.5));
						}
						else {
							SetVX(dynamic_cast<Elevator*>(e->obj)->GetDirection() == false && vx > 0 ?
								-vx * (1 + 0.5) : -vx * (1 - 0.5));
						}
					}

				}

			}

		}
			}
	}
	else if (state == DEATH_STATE) {

		CGameObject::Update(dt);
		SetVY(vy - GIMMICK_GRAVITY* dt);

		x = x + dx;
		y = y + dy;
	}
	if (state == JUMPING_STATE)
		state = state;
}
void SmallBlackBug::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state != DEATH_STATE) {
		left = x;
		top = y;
		right = left + WIDTH;
		bottom = top - 16;
	}
	else {
		left = top = right = bottom = 0;
	}
}
void SmallBlackBug::GetHit(bool dir) {
	state = DEATH_STATE;
	this->direction = true;
	vx = dir ? SBB_VX_WHEN_HIT : -SBB_VX_WHEN_HIT;
	vy = SBB_Y_SPEED;
	if (jar != NULL) {
		jar->Appear(x,y, dir);
	}

}
