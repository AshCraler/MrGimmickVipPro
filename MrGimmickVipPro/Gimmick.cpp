#include "Gimmick.h"
#include "./StaticObjects/Ground.h"
#include "Utils.h"
#include "Items/Item.h"
#include "StaticObjects/Elevator.h"
#include "StaticObjects/MStair.h"
#include "Enemies/Enemy.h"
#include "Enemies/SmallBlackBug.h"
#include "StaticObjects/Pipe.h"
#include "StaticObjects/Portal.h"

#include "./SceneManager.h"
#include "DynamicObjects/GunMachine.h"
#include "DynamicObjects/HangingElevator.h"
#include "Enemies/Utility/JumpingCommandBox.h"
#include "StaticObjects/Window.h"
#include "Enemies/Bosses/GiantBlackBug.h"
#include "Enemies/Turtle.h"
#include "DynamicObjects/Boat.h"
#include "Enemies/LongNeckedBird.h"
#include "DynamicObjects/WheelGunMachine.h"
#include "Enemies/Bosses/BrownStuff.h"
#include "Enemies/Bosses/BlackMan.h"
#include "Items/BigPrize.h"
#include "Enemies/PinkWorm.h"
#include "TileMap.h"
#include "PlayScence.h"
void CFire::Render()
{
	animation_set->at(state)->Render(x, y);

	RenderBoundingBox();
}
void CFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
	l = x;
	t = y;
	r = x + width;
	b = y - height;
}
CFire::CFire(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
	this->height = this->width = STAR_HEIGHT;
	this->state = FIRE_BIG_BALL;
	this->animation_set = CAnimationSets::GetInstance()->Get(7);
}

CFire::~CFire()
{
}
void CFire::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects, bool updateXY) {
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0) {
		x = x + dx;
		y = y + dy;


	}
	else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + dx;
		y = y + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<StaticObject*>(e->obj)) {
				if (e->nx != 0) {
					if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {

					}
				}
				if (e->ny != 0) {
					if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
					}
				}
			}
			else if (dynamic_cast<GiantBlackBug*>(e->obj)) {
				dynamic_cast<GiantBlackBug*>(e->obj)->SentFlying();
			}
			else if (dynamic_cast<Spark*>(e->obj)) {
				x = x;
			}
			else if (dynamic_cast<Enemy*>(e->obj)) {
				if (dynamic_cast<SmallBlackBug*>(e->obj)) {
					dynamic_cast<SmallBlackBug*>(e->obj)->GoDead(vx > 0);
				}
				else if (dynamic_cast<Turtle*>(e->obj)) {
					dynamic_cast<Turtle*>(e->obj)->GetTurnedUp();
				

				}
				else if (dynamic_cast<StandingStillBug*>(e->obj)) {
					dynamic_cast<StandingStillBug*>(e->obj)->GetHit();

				}
				else if (dynamic_cast<BombCaryingBug*>(e->obj)) {
					dynamic_cast<BombCaryingBug*>(e->obj)->GetHit();

				}
				else if (dynamic_cast<GreenFatStuff*>(e->obj)) {
					dynamic_cast<GreenFatStuff*>(e->obj)->GetHit();


				}
				else if (dynamic_cast<BrownStuff*>(e->obj)) {
					CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);
				}
				else if (dynamic_cast<BlackMan*>(e->obj)) {
					dynamic_cast<BlackMan*>(e->obj)->GetHit();
				}
				else if (dynamic_cast<SmallBug*>(e->obj)) {
					dynamic_cast<SmallBug*>(e->obj)->GetHit();
				}
			}
			
		}
	}
	if (abs(x - CGimmick::GetInstance()->x) > 260) {
		CGimmick::GetInstance()->whatToForm = CGimmick::whatToForm::Stars;
		gone = true;

	}
}
void CBomb::Render()
{
	if (!destroyed) {
		animation_set->at(state)->Render(x, y);
	}
	else {
		if (destroyed_phase == SpreadingStars) {
			for (int i = 0; i < BOMB_NUMBER_OF_STARS; i++) {
				animation_set->at(BOMB_STAR)->Render(Xs[i], Ys[i]);
			}
		}
		else if (/*GetTickCount()-smallerStarTimer<BOMB_SMALLER_STAR_TIME*/true) {
			for (int i = 0; i < BOMB_NUMBER_OF_STARS; i++) {
				animation_set->at(BOMB_SMALLER_STAR)->Render(Xs[i], Ys[i]);
			}
		}
	}

	RenderBoundingBox();
}
void CBomb::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!destroyed) {
		l = x;
		t = y;
		r = x + width;
		b = y - height;
	}
	else {
		l = r = t = b = 0;
	}

}
CBomb::CBomb(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
	this->height = this->width = STAR_HEIGHT;
	this->state = BOMB_BOMB_STATE;
	this->animation_set = CAnimationSets::GetInstance()->Get(BOMB_ANIMATION_SET_ID);
}

CBomb::~CBomb()
{
}
void CBomb::GetDestroyed() {
	destroyed = true;
	destroyed_phase = SpreadingStars;
	times = 0;
}
void CBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects, bool updateXY) {
	if (!destroyed) {
		CGameObject::Update(updateXY ? dt : 0);
		vy -= GIMMICK_GRAVITY * dt/2;


		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(colliable_objects, coEvents);

		if (coEvents.size() == 0) {
			x = x + dx;
			y = y + dy;


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
				if (dynamic_cast<StaticObject*>(e->obj)) {
					if (e->nx != 0) {
						if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
							vx = -vx * STAR_SPEED_REDUCTION_FACTOR;
						}
					}
					if (e->ny != 0) {
						if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
							vy = -vy * STAR_SPEED_REDUCTION_FACTOR;
						}
					}
				}
				else if (dynamic_cast<GiantBlackBug*>(e->obj)) {
					dynamic_cast<GiantBlackBug*>(e->obj)->SentFlying();
					GetDestroyed();
				}
				else if (dynamic_cast<Spark*>(e->obj)) {
					x = x;
				}
				else if (dynamic_cast<Enemy*>(e->obj)) {
					if (dynamic_cast<SmallBlackBug*>(e->obj)) {
						dynamic_cast<SmallBlackBug*>(e->obj)->GoDead(vx > 0);
						GetDestroyed();

					}
					else if (dynamic_cast<Turtle*>(e->obj)) {
						dynamic_cast<Turtle*>(e->obj)->GetTurnedUp();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						GetDestroyed();

					}
					else if (dynamic_cast<StandingStillBug*>(e->obj)) {
						dynamic_cast<StandingStillBug*>(e->obj)->GetHit();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						GetDestroyed();

					}
					else if (dynamic_cast<BombCaryingBug*>(e->obj)) {
						dynamic_cast<BombCaryingBug*>(e->obj)->GetHit();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						GetDestroyed();

					}
					else if (dynamic_cast<GreenFatStuff*>(e->obj)) {
						dynamic_cast<GreenFatStuff*>(e->obj)->GetHit();
						GetDestroyed();


					}
					else if (dynamic_cast<BrownStuff*>(e->obj)) {

					}
					else if (dynamic_cast<BlackMan*>(e->obj)) {
						dynamic_cast<BlackMan*>(e->obj)->GetHit();
						GetDestroyed();

					}
					else if (dynamic_cast<SmallBug*>(e->obj)) {
						dynamic_cast<SmallBug*>(e->obj)->GetHit();
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						GetDestroyed();

					}
				}
				else {
					vy = -vy * STAR_SPEED_REDUCTION_FACTOR;
				}
			}
		}
	}
	else {
		if (destroyed_phase == SpreadingStars) {
			for (int i = 0; i < BOMB_NUMBER_OF_STARS; i++) {
				if (Xs[i] == 0 && Ys[i] == 0) {
					Xs[i] = x;
					Ys[i] = y;
				}
				else {
					Xs[i] = Xs[i] + BOMB_SPREADING_SPEED * VXs[i] * dt;
					Ys[i] = Ys[i] - BOMB_SPREADING_SPEED * VYs[i] * dt;
				}

				if (abs(Xs[i] - x) * abs(Xs[i] - x) + abs(Ys[i] - y) * abs(Ys[i] - y) > BOMB_STARS_BOUNDARY * BOMB_STARS_BOUNDARY) {

					times++;
					if (times >= 10) {
						destroyed_phase = SmallerStars;
						smallerStarTimer = GetTickCount();
					}
					else {
						Xs[i] = x;
						Ys[i] = y;
					}
				}
			}
		}
		else if (GetTickCount() - smallerStarTimer > BOMB_SMALLER_STAR_TIME) {
			/*for (int i = 0; i < BOMB_NUMBER_OF_STARS; i++) {
				Xs[i] = 0;
				Ys[i] = 0;
			}*/
			gone = true;
			CGimmick::GetInstance()->whatToForm = CGimmick::whatToForm::Stars;
		}
	}

}
void Star::Render()
{
	CAnimationSets::GetInstance()->Get(2)->at(destroyed? 3:2)->Render(x, y);
	RenderBoundingBox();
}
void Star::SetXY(int _x, int _y) {
	
	this->x = _x;
	this->y = _y;
}
void Star::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width ;
	b = y - height;
}
void Star::SetVX(float _vx) { 
	
	this->vx = _vx; 
}
void Star::SetVY(float _vy) { this->vy = _vy; }
Star::Star(int _x, int _y, Hill* h)
{
	this->x = _x;
	this->y = _y;
	this->height = this->width = STAR_HEIGHT;
	this->hill = h;
	inHillArea = true;
}

Star::~Star()
{
}
void Star::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects, bool updateXY) {
	
	CGameObject::Update(updateXY?dt:0);
	SetVY(vy - GIMMICK_GRAVITY * dt/3);
	if (vy != 0) {
		vx = vx;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);

	if (coEvents.size() == 0) {
		x = x + dx;
		y = y + dy;

		if (inHillArea && hill!=NULL) {
			if (hill->x>x+width||hill->x+hill->width<x||hill->y<y-height) {
				inHillArea = false;
			}
			else {
				if (hill->isLeftUpToRight()) {
 					if ((x + width - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
						vy = -vy;
						y = (((-hill->height + hill->y + height) + ((x + width - hill->x) / hill->GetXToYRatio())));
					}
				}
				else {
					if ((-x + hill->width + hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
						y = ((-hill->height + hill->y + height) + ((-x + hill->width + hill->x) / hill->GetXToYRatio()));
						vy = -vy;
					}
				}
			}
			
		}
		
	}
	else {
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x= x + min_tx * dx + nx * 0.1f;
		y= y + min_ty * dy - ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<JumpingCommandBox*>(e->obj) || dynamic_cast<ReturningCommandBox*>(e->obj)
				|| dynamic_cast<ReturnOrNotCommandbBox*>(e->obj) || dynamic_cast<SelectiveJumpingCommandBox*>(e->obj)) {
				x = x + dx - (min_tx * dx + nx * 0.1f);
				y = y + dy - (min_ty * dy - ny * 0.1f);
			}
			else {
				if (dynamic_cast<StaticObject*>(e->obj)) {
					if (e->nx != 0) {
						if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
							vx = -vx * STAR_SPEED_REDUCTION_FACTOR;
						}
						if (dynamic_cast<Hill*>(e->obj)) {
							inHillArea = true;
							hill = dynamic_cast<Hill*>(e->obj);
							x = x + dx - (min_tx * dx + nx * 0.1f);
							y = y + dy - (min_ty * dy - ny * 0.1f);
						}
					}
					if (e->ny != 0) {
						if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<MStair*>(e->obj))) {
							vy = -vy * STAR_SPEED_REDUCTION_FACTOR;
						}
						if (dynamic_cast<Hill*>(e->obj)) {
							inHillArea = true;
							hill = dynamic_cast<Hill*>(e->obj);
							x = x + dx - (min_tx * dx + nx * 0.1f);
							y = y + dy - (min_ty * dy - ny * 0.1f);
						}
					}
				}
				else if (dynamic_cast<GiantBlackBug*>(e->obj)) {
					dynamic_cast<GiantBlackBug*>(e->obj)->SentFlying();
					if (updateXY) {
						SignalGimmickToDestroyStar();
					}
				}
				else if (dynamic_cast<Spark*>(e->obj)) {
					SignalGimmickToDestroyStar();
					x = x;
				}
				else if (dynamic_cast<LockingViewPoint*>(e->obj)) {
					LockingViewPoint* p = dynamic_cast<LockingViewPoint*>(e->obj);


					x = (x - min_tx * dx - nx * 0.3f + dx);
					if (e->ny != 0)
						y= (y - min_ty * dy + ny * 0.3f + dy);
				}
				else if (dynamic_cast<LockingViewToPoint*>(e->obj)) {
					LockingViewToPoint* p = dynamic_cast<LockingViewToPoint*>(e->obj);


					x = (x - min_tx * dx - nx * 0.3f + dx);
					if (e->ny != 0)
						y = (y - min_ty * dy + ny * 0.3f + dy);
				}
				else if (dynamic_cast<Enemy*>(e->obj)) {
					if (dynamic_cast<SmallBlackBug*>(e->obj)) {
						dynamic_cast<SmallBlackBug*>(e->obj)->GetHit(e->nx > 0);
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);
					}
					else if (dynamic_cast<PinkWorm*>(e->obj)) {
						dynamic_cast<PinkWorm*>(e->obj)->GetHit(e->nx > 0);
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);
					}
					else if (dynamic_cast<Turtle*>(e->obj)) {
						dynamic_cast<Turtle*>(e->obj)->GetTurnedUp();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);

					}
					else if (dynamic_cast<StandingStillBug*>(e->obj)) {
						dynamic_cast<StandingStillBug*>(e->obj)->GetHit();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);

					}
					else if (dynamic_cast<BombCaryingBug*>(e->obj)) {
						dynamic_cast<BombCaryingBug*>(e->obj)->GetHit();
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);

						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);

					}
					else if (dynamic_cast<GreenFatStuff*>(e->obj)) {
						dynamic_cast<GreenFatStuff*>(e->obj)->GetHit();


					}
					else if (dynamic_cast<BrownStuff*>(e->obj)) {
						if (dynamic_cast<BrownStuff*>(e->obj)->ProcessStars(this, e->nx, e->ny))
							SignalGimmickToDestroyStar();

					}
					else if (dynamic_cast<BlackMan*>(e->obj)) {
						dynamic_cast<BlackMan*>(e->obj)->GetHit();
						SignalGimmickToDestroyStar();

					}
					else if (dynamic_cast<SmallBug*>(e->obj)) {
						dynamic_cast<SmallBug*>(e->obj)->GetHit();
						y = y + dy - (min_ty * dy - ny * 0.1f);
						x = x + dx - (min_tx * dx + nx * 0.1f);
						CGimmick::GetInstance()->IncreaseScore(KILL_ENEMY);

					}
				}
				else {
					vy = -vy * STAR_SPEED_REDUCTION_FACTOR;
				}
			}
			
		}
	}
}
void Star::SignalGimmickToDestroyStar() {
	CGimmick::GetInstance()->aboutToDestroyStar = true;
}
CGimmick* CGimmick::instance = NULL;

CGimmick::CGimmick(float x, float y)
{
	
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);
	landedWhen = GetTickCount();

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	width = 16;
	height = 16;
	vx = vy = 0;
	items.push_back(1);
	items.push_back(3);
	items.push_back(2);
	pickedItem = 0;
}
void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (score < finalScore) {
		score += SCORE_INSCREASED_BY;
		if (score > finalScore)
			score = finalScore;
	}
	// Calculate dx, dy 
	
	CGameObject::Update(dt);
	if (state == GIMMICK_STATE_INJURED)
		x = x;
	if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME)
		x = x;

	// Simple fall down
	//vy -= GIMMICK_GRAVITY*dt;
	Pipe* p;
	bool stillInPipe = false;
	int minDistance = 99999, dumpDistance;
	for (UINT i = 0; i < colliable_objects->size(); i++) {
		if (dynamic_cast<Pipe*>(colliable_objects->at(i))) {
			p = dynamic_cast<Pipe*>(colliable_objects->at(i));
			if (!p->CanWalk() && !(p->x > x + width || x > p->x + p->width || y - height > p->y || p->y - p->height > y)) {
				stillInPipe = true;
				int d = pow(x - p->x, 2) + pow(y - p->y, 2);
				if (pow(x - p->x, 2) + pow(y - p->y, 2) < minDistance) {
					pipe = p;
					minDistance = pow(x - p->x, 2) + pow(y - p->y, 2);
				}
			}
		}
	}
	if (stillInPipe) {
		CGameObject::SetState(GIMMICK_STATE_IN_PIPE);
	}
	if (state != GIMMICK_STATE_IN_PIPE) {
		SetVY(vy - GIMMICK_GRAVITY * dt);
		if (state == GIMMICK_STATE_INJURED) {
			if (vx < finalVX && finalVX >= 0) {
				vx += GIMMICK_FRICTION_FORCE;
				if (vx > finalVX)vx = finalVX;
			}
			else if (vx > finalVX && finalVX <= 0) {
				vx -= GIMMICK_FRICTION_FORCE;
				if (vx < finalVX)vx = finalVX;
			}
		}
		else {
			if (vx < finalVX && finalVX >= 0) {
				vx += GIMMICK_FRICTION_FORCE;
				if (vx > finalVX)vx = finalVX;
			}
			else if (vx > finalVX && finalVX <= 0) {
				vx -= GIMMICK_FRICTION_FORCE;
				if (vx < finalVX)vx = finalVX;
			}
		}
		
	}
	else {
		Pipe* p;
		bool stillInPipe = false;
		int minDistance=99999, dumpDistance;
		for (UINT i = 0; i < colliable_objects->size(); i++) {
			if (dynamic_cast<Pipe*>(colliable_objects->at(i))) {
				p = dynamic_cast<Pipe*>(colliable_objects->at(i));
				if (!p->CanWalk() && !(p->x > x + width || x > p->x + p->width || y - height > p->y || p->y - p->height > y)) {
					stillInPipe = true;
					int d = pow(x - p->x,2) + pow(y - p->y,2);
					if (pow(x - p->x,2) + pow(y - p->y,2) < minDistance) {
						pipe = p;
						minDistance = pow(x - p->x, 2) + pow(y - p->y, 2);
					}
				}
			}
		}
		if (!stillInPipe) {
			CGameObject::SetState(GIMMICK_STATE_IDLE);
			landedWhen = GetTickCount();
		}
		else {
			CGameObject::SetState(GIMMICK_STATE_IN_PIPE);

		}
	}
	/*if (y > 365)
		y = y;*/
	if (state == GIMMICK_STATE_INJURED)
		state = state;
	if (state == GIMMICK_STATE_INJURED && GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
		//CGameObject::SetState(GIMMICK_STATE_IDLE);
		if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT) {
			CGameObject::SetState(GIMMICK_STATE_IDLE);
			landedWhen = GetTickCount();
		}
		
	}
	bool collideWithGround__ = false;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (int i = 0; i < colliable_objects->size() ; i++) {
		if (dynamic_cast<WheelGunMachine*>(colliable_objects->at(i))) {
			vector<FlyingShell*> s = dynamic_cast<WheelGunMachine*>(colliable_objects->at(i))->GetShells();
			colliable_objects->insert(colliable_objects->end(), s.begin(), s.end());
		}
		if (dynamic_cast<BlackMan*>(colliable_objects->at(i))) {
			vector<Spark*> s = dynamic_cast<BlackMan*>(colliable_objects->at(i))->GetSparks();
			colliable_objects->insert(colliable_objects->end(), s.begin(), s.end());
		}
		if (dynamic_cast<GiantBlackBug*>(colliable_objects->at(i))) {
			vector<SmallBug*> s = dynamic_cast<GiantBlackBug*>(colliable_objects->at(i))->GetBugs();
			colliable_objects->insert(colliable_objects->end(), s.begin(), s.end());
		}
	}
	
	if (y - 15< 34)
		y = y;
	if (standingOnSBB)
		SetVX(sbb->vx);
	// turn off collision when die 
	if (stars.size() != 0 && star_phase==4)
		colliable_objects->push_back(&stars[0]);
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(colliable_objects, coEvents);

	// reset untouchable timer if untouchable time has passed
	
	if (standingOnElevator!=0) {
		if (dy > 0 || (x + width < eFrom) || x > eTo)
			SetStandingOnElevator(0);
		else SetStandingOnElevator(standingOnElevator, eFrom, eTo);
	} 
	if (vy<-0.03f &&climbingHill==OutOfArea&& state==GIMMICK_STATE_IDLE && !climbingHill 
		&&!standingOnHangingElevator &&!standingOnBBA&&!standingOnBBA && !standingOnBird
		&&!standingOnGFS&&!standingOnWGM &&!standingOnShell
		&&!standingOnBoat&&!standingOnSB
		&& !standingOnMovingStair && state!=GIMMICK_STATE_IN_PIPE) {
		CGameObject::SetState(GIMMICK_STATE_JUMP);
	}
	/*if (standingOnMovingStair != 0) {
		if ((x + width < sFrom) || x > sTo)
			SetStandingMovingStair(false);
		else SetStandingMovingStair(standingOnMovingStair, eFrom, eTo);
	}*/
	// No collision occured, proceed normally
	//if (standingOnHill) {
	//	if ((x + width < hill->x || hill->x + hill->width < x) || (hill->y > y + height)) {
	//		//SetVY(vy + GIMMICK_GRAVITY * dt);
	//		SetStangdingOnHill(false);
	//	}
	//}
	if (coEvents.size() == 0)
	{
		if (gmachine != NULL && GetTickCount()-gm_timer_small>100) { gmachine->StopBeingPushed(); gmachine = NULL; }
		if (standingOnMovingStair) {
			landedWhen = GetTickCount();
			SetX(stair->x + deltaXToMStair + dx);
			deltaXToMStair += dx;
			SetY(stair->y + deltaYToMStair);
			if (x + width < stair->x || stair->x + stair->width < x) {
				SetVY(vy + GIMMICK_GRAVITY * dt);
				SetStandingMovingStair(false);
			}

		}
		else if (standingOnHangingElevator) {
			landedWhen = GetTickCount();
			SetX(he->GetX() - deltaHE_x + dx);
			deltaHE_x -= dx;
			SetY(he->GetY() - deltaHE_y);
			if (x + width < he->GetStart() || he->GetEnd() < x) {
				SetVY(vy + GIMMICK_GRAVITY * dt);
				SetStandingHangingElevator(false);
			}
		}
		else if(standingOnSBB){
			if (sbb->state == 0)
				SetStandingOnSBB(false);
			if (standingOnSBB) {
				landedWhen = GetTickCount();

				SetX(sbb->x - deltaSBB_x + dx);
				deltaSBB_x -= dx;
				SetY(sbb->y + 16);
				if (x + width < sbb->x || sbb->x + sbb->width < x) {
					SetVY(vy - GIMMICK_GRAVITY * dt);
					SetStandingOnSBB(false);
				}
			}
			
		}
		else if (standingOnSB) {
			if (sb->state == 0)
				SetStandingOnSB(false);
			if (standingOnSB) {
				landedWhen = GetTickCount();

				SetX(sb->x - deltaSB_x + dx);
				deltaSB_x -= dx;
				SetY(sb->y + 16);
				if (x + width < sb->x || sb->x + sb->width < x) {
					SetVY(vy - GIMMICK_GRAVITY * dt);
					SetStandingOnSB(false);
				}
			}

		}
		else if (standingOnBoat) {
			landedWhen = GetTickCount();
			SetX(boat->x + deltaBoat_x + dx);
			deltaBoat_x += dx;
			SetY(boat->y +deltaBoat_y);
			if (x + width < boat->x || boat->x + boat->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnBoat(false);
			}
		}
		else if (standingOnBird) {
			landedWhen = GetTickCount();
			SetX(bird->x + deltaBird_x + dx);
			deltaBird_x += dx;
			SetY(bird->y + deltaBird_y);
			if (x + width < bird->x || bird->x + bird->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnBird(false);
			}
		}
		else if (standingOnShell) {
			landedWhen = GetTickCount();
			SetX(shell->x + deltaShell_x + dx);
			deltaShell_x += dx;
			SetY(shell->y + deltaShell_y);
			if (x + width < shell->x || shell->x + shell->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnShell(false);
			}
		}
		else if (standingOnWGM) {
			landedWhen = GetTickCount();
			SetX(wgm->x + deltaWGM_x + dx);
			deltaWGM_x += dx;
			SetY(wgm->y + deltaWGM_y);
			if (x + width < wgm->x || wgm->x + wgm->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnWGM(false);
			}
		}
		else if (standingOnGFS) {
			landedWhen = GetTickCount();
			SetX(gfs->x + deltaGFS_x + dx);
			deltaGFS_x += dx;
			SetY(gfs->y + deltaGFS_y);
			if (x + width < gfs->x || gfs->x + gfs->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnGFS(false);
			}
		}
		else if (standingOnStar) {
			landedWhen = GetTickCount();
			SetX(str->x + deltaSTAR_x + dx);
			deltaSTAR_x += dx;
			SetY(str->y + deltaSTAR_y);
			if (x + width < str->x || str->x + str->width < x) {
				SetVY(-GIMMICK_GRAVITY);
				CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
				SetStandingOnStar(false);
			}
		}
		else if (standingOnBBA) {
			if (x + width < bba->x || bba->x + bba->width < x) {
				standingOnBBA = false;
				bba->GetReleased();
			}
		}
		//else if (standingOnHill) {
		//	//if ((x + width < hill->x || hill->x + hill->width < x) && (hill->y<y+height)) {
		//	//	//SetVY(vy + GIMMICK_GRAVITY * dt);
		//	//	SetStangdingOnHill(false);
		//	//}
		//}
		else if (state == GIMMICK_STATE_IN_PIPE) {
			SetX(x + dx);
			SetY(y + dy);
			if (dx < 0)
				dy = dy;
			if (dx > 0) {
				if (pipe->GetType() == top_left || pipe->GetType() == top_bottom || pipe->GetType() == bottom_left || (pipe->GetType() == right_bottom && abs(y - pipe->y) > 0.1)) {
					if (x > pipe->x) {
						SetX(pipe->x);
					}
				}

			}
			else if (pipe->GetType() == top_right || pipe->GetType() == top_bottom || pipe->GetType() == right_bottom || (pipe->GetType() == top_left && abs(y-pipe->y)>0.1)|| (pipe->GetType() == bottom_left && abs(y - pipe->y) > 0.1)) {
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

			}
		}
		else  {
			//if (inHillArea && hill!=NULL) {
			//	/*if (hill->isLeftUpToRight()) {
			//		float ratio = abs((x + 10 - hill->x)) / abs((y + height - hill->y - hill->height));
			//		if (ratio<=hill->GetXToYRatio()+0.12&& ratio >= hill->GetXToYRatio() - 0.12) SetStangdingOnHill(true, hill);
			//	}	
			//	else {
			//		float ratio = abs((x + 10 - hill->x)) / abs((y + height - hill->y - hill->height));
			//		if (ratio <= hill->GetXToYRatio() + 0.12 && ratio >= hill->GetXToYRatio() - 0.12) SetStangdingOnHill(true, hill);
			//	}*/
			//	if (hill->isLeftUpToRight()) {
			//		float ratio = (x + width - 4 - hill->x) / abs((y + height - hill->y - hill->height));
			//		if (ratio <= hill->GetXToYRatio() && ratio >=0 ) {
			//			SetStangdingOnHill(true, hill);
			//			SetY(hill->height + hill->y - (x + width - 4 - hill->x) / hill->GetXToYRatio() - height);
			//		}
			//	}
			//	else {
			//		float ratio = (x + 10 - hill->x - hill->width) / abs((y + height - hill->y - hill->height));
			//		if (ratio >= -hill->GetXToYRatio()&&ratio <=0) {
			//			SetStangdingOnHill(true, hill);
			//			SetY(hill->height + hill->y + (x + 10 - hill->x - hill->width) / hill->GetXToYRatio() - height);
			//		}
			//	}/*
			//	if ((ratio <= hill->GetXToYRatio() + 0.12 && ratio >= hill->GetXToYRatio() - 0.12 && hill->isLeftUpToRight() )||
			//		(ratio <= -hill->GetXToYRatio() + 0.12 && ratio >= -hill->GetXToYRatio() - 0.12 && !hill->isLeftUpToRight())) SetStangdingOnHill(true, hill);*/
			//	if ((x + width < hill->x || hill->x + hill->width < x) || (hill->y > y + height)) inHillArea = false;
			//}
			//
			//if (standingOnHill) {
			//	
			//	if (inHillArea) {
			//		inHillArea = false;
			//	}
			//	else {
			//		if ((x + width < hill->x || hill->x + hill->width < x) || (hill->y > y + height)) {
			//			//SetVY(vy + GIMMICK_GRAVITY * dt);
			//			SetStangdingOnHill(false);

			//		}
			//		//SetStangdingOnHill(false);
			//		else {
			//			SetX(x + dx);
			//			SetY(y + dy);
			//		}
			//	}

			//	
			//}
			if (climbingHill != OutOfArea) {
				if (hill == NULL)climbingHill = OutOfArea;
			}
			if (climbingHill == Enter && hill!=NULL) {
				if (hill->isLeftUpToRight()) {
					if (vy<0&& 
						((x + width - 4 < hill->x + hill->width && y  - height >=hill->y-2)||
							(x + width - 4 > hill->x && y - height< hill->y-2))) {
						
						if (state != GIMMICK_STATE_JUMP && state!=GIMMICK_STATE_INJURED) {
							if ((x + width - 4 < hill->x + hill->width && y - height >= hill->y - 2)) {
								if (((x + width - 4 - hill->x) / abs(-hill->height + hill->y - y + height) < hill->GetXToYRatio())) {
									SetY((-hill->height + hill->y + height + ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
									SetVY(0);
									climbingHill = Climbing;
								}
							}
							if ((x + width - 4 > hill->x && y - height < hill->y - 2)) {
								if (((x + width - 4 - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
									SetX(abs(-hill->height + hill->y - y + height) * hill->GetXToYRatio() + 4 - width + hill->x);
									
									SetVY(0);
									climbingHill = Climbing;
								}
							}
							/*if (((x + width - 4 - hill->x) / (hill->height + hill->y - y - height) < hill->GetXToYRatio())) {
								SetY((hill->height + hill->y - height - ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
								SetVY(0);
								climbingHill = Climbing;
							}*/
							x = x + dx;
							landedWhen = GetTickCount();

							
						}
						else if(state==GIMMICK_STATE_JUMP){
							if (((x + width - 4 - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
								SetY((-hill->height + hill->y + height + ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
								SetVY(0);
								state = GIMMICK_STATE_IDLE;
								climbingHill = Climbing;
							}
							else {
								SetY(y + dy);
							}
							x = x + dx;
						}
						else if (state == GIMMICK_STATE_INJURED) {
							//if (((x + width - 4 - hill->x) / (hill->height + hill->y - y - height) > hill->GetXToYRatio())) {
							//	SetY((hill->height + hill->y - height - ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
							//	SetVY(0);
							//	//state = GIMMICK_STATE_IDLE;
							//	climbingHill = Climbing;
							//}
							//else 
							landedWhen = GetTickCount();

							if (((x + width - 4 - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
								SetY((-hill->height + hill->y + height + ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
								//SetVX();
								//SetVY(0);
								//SetVX(-GIMMICK_WALKING_SPEED * (hill->GetSpeedReductionFactor()));
								//SetVY(-abs(vx) / hill->GetXToYRatio());								//state = GIMMICK_STATE_IDLE;
								//climbingHill = Climbing;
								SetVX(0);
								vx = 0;
								SetVY(0);
								
							}
							//else if(((x + width - 4 - hill->x) / (hill->height + hill->y - y - height) < hill->GetXToYRatio()))
							else {
								SetY(y + dy);
							}
							x = x + dx;
						}

						//if (y + height > hill->y + hill->height / 2) {
						//	//vx = X_SPEED * (1 - hill->GetSpeedReductionFactor());
						//	//SetVY(-abs(vx) / hill->GetXToYRatio());

						//}
						//else {
						//	//vx = -X_SPEED * (1 + hill->GetSpeedReductionFactor());
						//	//SetVY(abs(vx) / hill->GetXToYRatio());

						//}
						//SetY((hill->height + hill->y - height - ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
						//SetVY(0);
						//climbingHill = Climbing;
					}
					else {
						x = x + dx;
						if (state == GIMMICK_STATE_JUMP || state==GIMMICK_STATE_INJURED)
							SetY(y + dy);
						//y = y + dy;
					}
				}
				else {
					//if ((-x - 4 + hill->width + hill->x) / (hill->height + hill->y - y - height) > hill->GetXToYRatio()) {
					//	if (y + height > hill->y + hill->height / 2) {
					//		/*vx = -X_SPEED * (1 - hill->GetSpeedReductionFactor());
					//		SetVY(-abs(vx) / hill->GetXToYRatio());*/

					//	}
					//	else {
					//		// vx = X_SPEED * (1 + hill->GetSpeedReductionFactor());
					//		// SetVY(abs(vx) / hill->GetXToYRatio());

					//	}
					//	//SetY((hill->height + hill->y - height - (-x + hill->width - 4 + hill->x) / hill->GetXToYRatio())-0.5);

					//	climbingHill = Climbing;

					//}
					//else {
					//	x = x + dx;
					//	//y = y + dy;
					//	if (state == GIMMICK_STATE_JUMP)
					//		SetY(y + dy);
					//}
					if (vy < 0 &&
						((x + 4 > hill->x && y - height >= hill->y - 2) ||
							(x + 4 < hill->x + hill->width && y - height < hill->y - 2))) {

						if (state != GIMMICK_STATE_JUMP && state!=GIMMICK_STATE_INJURED) {
							if ((x + 4 > hill->x && y - height >= hill->y - 2)) {
								if ((-(x + 4 - hill->x - hill->width) / abs(-hill->height + hill->y - y + height) < hill->GetXToYRatio())) {
									//SetY((hill->height + hill->y - height - ((-x - 4 + +hill->width+ hill->x) / hill->GetXToYRatio())));
									SetX((-hill->height + hill->y - y + height) * hill->GetXToYRatio() + hill->width + hill->x - 4);
									SetVY(0);
									climbingHill = Climbing;
								}
							}
							if ((x + 4 < hill->x + hill->width && y - height < hill->y - 2)) {
								if ((-(x + 4 - hill->x - hill->width) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
									SetY((-hill->height + hill->y + height + ((-x - 4 + +hill->width+ hill->x) / hill->GetXToYRatio())));
									//SetX((hill->height + hill->y - y - height) * hill->GetXToYRatio() - width + hill->width + hill->x + 4);
									SetVY(0);
									climbingHill = Climbing;
								}
							}
							/*if (((x + width - 4 - hill->x) / (hill->height + hill->y - y - height) < hill->GetXToYRatio())) {
								SetY((hill->height + hill->y - height - ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
								SetVY(0);
								climbingHill = Climbing;
							}*/
							//DebugOut(L"state in Gimmick: %d\n", state);
							landedWhen = GetTickCount();


						}
						else if(state==GIMMICK_STATE_JUMP){
							if ((-(x + 4 - hill->x - hill->width) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
								SetY((-hill->height + hill->y + height + (-(x + 4 - hill->width - hill->x) / hill->GetXToYRatio())));
								SetVY(0);
								state = GIMMICK_STATE_IDLE;
								climbingHill = Climbing;
							}
							else {
								SetY(y + dy);
							}
						}
						else {
							landedWhen = GetTickCount();

							if ((-(x + 4 - hill->x - hill->width) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio())) {
								SetY((-hill->height + hill->y + height + (-(x + 4 - hill->width - hill->x) / hill->GetXToYRatio())));
								//SetVX(GIMMICK_WALKING_SPEED * (hill->GetSpeedReductionFactor()));
								//SetVY(abs(vx) / hill->GetXToYRatio());								
								////state = GIMMICK_STATE_IDLE;
								//climbingHill = Climbing;
								SetVX(0);
								vx = 0;
								SetVY(0);
							}
							else {
								SetY(y + dy);
							}
						}
						x = x + dx;

						//if (y + height > hill->y + hill->height / 2) {
						//	//vx = X_SPEED * (1 - hill->GetSpeedReductionFactor());
						//	//SetVY(-abs(vx) / hill->GetXToYRatio());

						//}
						//else {
						//	//vx = -X_SPEED * (1 + hill->GetSpeedReductionFactor());
						//	//SetVY(abs(vx) / hill->GetXToYRatio());

						//}
						//SetY((hill->height + hill->y - height - ((x + width - 4 - hill->x) / hill->GetXToYRatio())));
						//SetVY(0);
						//climbingHill = Climbing;
					}
					else {
						x = x + dx;
						if (state == GIMMICK_STATE_JUMP)
							SetY(y + dy);
						//y = y + dy;
					}
				}
				
				if (x > hill->width + hill->x || x + height < hill->x || (vy > 0 && y - height > hill->y) || (vy < 0 && y < hill->y - hill->height)) {
					climbingHill = OutOfArea;
					SetX(x + dx);
					SetY(y - dt*vy);
					//SetVY(GIMMICK_GRAVITY * dt);
				}
			}else if(climbingHill == Climbing) {
				landedWhen = GetTickCount();

				if (x > hill->width + hill->x || x + height < hill->x || (vy > 0 && y - height > hill->y) || (vy < 0 && y < hill->y - hill->height)) {
					climbingHill = Enter;
					SetVY(-GIMMICK_GRAVITY * dt);
					SetX(x + dx);
					//SetY(y + dy);
				}
				else {
					if (state != GIMMICK_STATE_INJURED) {
						
					}
					SetX(x + dx);
					SetY(y + dy);
				}
			}
			else {
				/*if(abs(dy) >= 0.001) 
					CGameObject::SetState(GIMMICK_STATE_JUMP);
				else 
					CGameObject::SetState(GIMMICK_STATE_IDLE);*/

				SetX(x + dx);
				SetY(y + dy);
			}
			
		}
		
		// SetVY(vy + GIMMICK_GRAVITY * dt);
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;

		bool collideWithGroundOnY = false, collideWithHillOnY = false, gm=false;
		for (UINT i = 0; i < coEvents.size(); i++) {
			if (coEvents[i]->ny < 0) {
				if (dynamic_cast<Ground*>(coEvents[i]->obj))
					collideWithGroundOnY = true;
				if (dynamic_cast<Hill*>(coEvents[i]->obj)) {
					if ((vx < 0 && !dynamic_cast<Hill*>(coEvents[i]->obj)->isLeftUpToRight()) ||
						(vx > 0 && dynamic_cast<Hill*>(coEvents[i]->obj)->isLeftUpToRight())) {
						collideWithHillOnY = true;

					}
				}
				if (dynamic_cast<GunMachine*>(coEvents[i]->obj)) gm = true;
				
			}
			if (dynamic_cast<LockingViewPoint*>(coEvents[i]->obj)) {
				LockingViewPoint* p = dynamic_cast<LockingViewPoint*>(coEvents[i]->obj);
				if ((p->direction && x > p->x) || (!p->direction && x < p->x)) {
					GetLocked(true, p->GetFrom());
				}

			}
			else if (dynamic_cast<LockingViewToPoint*>(coEvents[i]->obj)) {
				LockingViewToPoint* p = dynamic_cast<LockingViewToPoint*>(coEvents[i]->obj);
				if ((p->direction && x > p->x) || (!p->direction && x < p->x)) {
					GetLockedIn(true, p->GetTo());
				}
			}
		}
		if (climbingHill!=OutOfArea && collideWithGroundOnY && collideWithHillOnY) {
			for (UINT i = 0; i < coEvents.size(); i++) {
				if (coEvents[i]->ny < 0 && !dynamic_cast<Ground*>(coEvents[i]->obj))
					coEvents.erase(coEvents.begin() + i);
			}
		}
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		/*x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;*/
		/*if (!standingOnMovingStair) {
			SetX(x + min_tx * dx + nx * 0.4f);
			SetY(y + min_ty * dy + ny * 0.4f);
		}*/
		/*if (standingOnMovingStair && dx != 0)
			dx = dx;*/
		if (gm)
			x = x;
		SetX(x + min_tx * dx + nx * 0.3f);
		SetY(y + min_ty * dy - ny * 0.3f);
		//CGameObject::SetState(GIMMICK_STATE_IDLE);

		/*if (nx != 0) SetVX(0);
		if (ny != 0) SetVY(0);*/
		if (standingOnSBB)
			state = state;
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Portal*>(e->obj)) {
				CSceneManager::GetInstance()->SwitchScene(dynamic_cast<Portal*>(e->obj)->scene);
			}
			if (dynamic_cast<JumpingCommandBox*>(e->obj) || dynamic_cast<ReturningCommandBox*>(e->obj) 
				|| dynamic_cast<ReturnOrNotCommandbBox*>(e->obj) || dynamic_cast<SelectiveJumpingCommandBox*>(e->obj)) {
				if(e->nx!=0)
					SetX(x - min_tx * dx - nx * 0.3f+dx);
				else SetY(y - min_ty * dy + ny * 0.3f+dy);
			}
			else if (dynamic_cast<Window*>(e->obj)) {
				dynamic_cast<Window*>(e->obj)->OpenWindow();
				SetX(x +dx- min_tx * dx - nx * 0.3f);
				SetY(y- min_ty * dy + ny * 0.3f);
			}
			else if (dynamic_cast<Star*>(e->obj)) {
				/*if (standingOnStar) {
					SetX(str->x + deltaSTAR_x + dx);
					deltaSTAR_x += dx;
					SetY(str->y + deltaSTAR_y);
					if (x + width < str->x || str->x + str->width < x) {
						SetVY(-GIMMICK_GRAVITY);
						CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
						SetStandingOnStar(false);
					}
				}*/
				if (e->ny < 0) {
					if (standingOnStar) {
						SetX(str->x + deltaSTAR_x + dx);
						deltaSTAR_x += dx;
						SetY(str->y + deltaSTAR_y);
						if (x + width < str->x || str->x + str->width < x) {
							SetVY(-GIMMICK_GRAVITY);
							CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
							SetStandingOnStar(false);
						}
					}
					else {
						SetStandingOnStar(true, dynamic_cast<Star*>(e->obj));
					}
				}
				else {
					SetX(x - min_tx * dx - nx * 0.3f + dx);
					SetY(y - min_ty * dy + ny * 0.3f );
				}
			}
			else if (dynamic_cast<SmallBlackBugWithAntena*>(e->obj)) {
				SmallBlackBugWithAntena* b = dynamic_cast<SmallBlackBugWithAntena*>(e->obj);
				if (e->ny < 0) {
					if (b->IsWalking()) {
						standingOnBBA = true;
						b->GetStoodOn();
						bba = b;
						CGameObject::SetState(GIMMICK_STATE_IDLE);
						landedWhen = GetTickCount();
					}
				}
				else {
					GetInjured(INJURED_BY_ENEMY,e->nx > 0||vx>0);
					if (e->ny > 0) {
						if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(y - min_ty * dy - ny * 0.3f );
						}
						
					}
				}
				if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
					SetX(x - min_tx * dx - nx * 0.3f + dx);
					SetY(y - min_ty * dy - ny * 0.3f);
				}
				//b->SetDefending();

			}
			else if (dynamic_cast<LockingViewPoint*>(e->obj)) {
				LockingViewPoint* p=dynamic_cast<LockingViewPoint*>(e->obj);
				if ((p->direction && x > p->x) || (!p->direction && x < p->x)) {
					GetLocked(true, p->GetFrom());
				}
				
				SetX(x - min_tx * dx - nx * 0.3f + dx);
				if(e->ny!=0)
					SetY(y - min_ty * dy + ny * 0.3f +dy);
			}
			else if (dynamic_cast<LockingViewToPoint*>(e->obj)) {
				LockingViewToPoint* p = dynamic_cast<LockingViewToPoint*>(e->obj);
				if ((p->direction && x > p->x) || (!p->direction && x < p->x)) {
					GetLockedIn(true, p->GetTo());
				}				
				SetX(x - min_tx * dx - nx * 0.3f + dx);
				if (e->ny != 0)
					SetY(y - min_ty * dy + ny * 0.3f + dy);
			}
			else if (dynamic_cast<Spark*>(e->obj) || dynamic_cast<BFire*>(e->obj)) {
				GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
				if (e->ny > 0) {
					if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
						SetX(x - min_tx * dx - nx * 0.3f + dx);
						SetY(y - min_ty * dy - ny * 0.3f);
					}
				}
				if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
					SetX(x - min_tx * dx - nx * 0.3f + dx);
					SetY(y - min_ty * dy - ny * 0.3f);
				}
			}
			else if (dynamic_cast<Enemy*>(e->obj)) {
				if (dynamic_cast<BlackMan*>(e->obj) || 
					dynamic_cast<GiantBlackBug*>(e->obj)|| dynamic_cast<BrownStuff*>(e->obj)) {
					GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
					if (e->ny > 0) {
						if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(y - min_ty * dy - ny * 0.3f);
						}
					}
					if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
						SetX(x - min_tx * dx - nx * 0.3f + dx);
						SetY(y - min_ty * dy - ny * 0.3f);
					}
				}
				
				else if (dynamic_cast<PinkWorm*>(e->obj)) {
					GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
					if (e->ny > 0) {
						if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(y - min_ty * dy - ny * 0.3f);
						}
					}
					if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
						SetX(x - min_tx * dx - nx * 0.3f + dx);
						SetY(y - min_ty * dy - ny * 0.3f);
					}
				}
				else if (dynamic_cast<SmallBug*>(e->obj)) {
					if (!standingOnSB) {
						if (e->ny < 0) {
							SetStandingOnSB(true, dynamic_cast<SmallBug*>(e->obj));
							SetY(sb->y + height);
							SetVY(0);
						}
						else {
							GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
							if (e->ny > 0) {
								if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(y - min_ty * dy + ny * 0.3f);
								}
							}

						}
						if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(y - min_ty * dy + ny * 0.3f);
						}
					}
					else {
						SetX(sb->x - deltaSB_x + dx);
						//DebugOut(L"x, sX, deltaX when standing on MS: %f, %f, %f\n", x, dynamic_cast<MStair*>(e->obj)->x,deltaXToMStair);
						SetY(sb->y - deltaSB_y);
						deltaSB_x -= dx;
						SetVY(0);
					}
					
					
				}
				else if (dynamic_cast<SmallBlackBug*>(e->obj)) {
					if (!standingOnSBB) {
						if (e->ny < 0) {
							SetStandingOnSBB(true, dynamic_cast<SmallBlackBug*>(e->obj));
							SetY(sbb->y + height);
							SetVY(0);
						}
						else {
							GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
							if (e->ny > 0) {
								if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(y - min_ty * dy + ny * 0.3f);
								}
							}
						}
						
						if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(y - min_ty * dy + ny * 0.3f);
						}
						//SetX(x + nx * 1);
					}
					else {
						SetX(sbb->x - deltaSBB_x + dx);
						//DebugOut(L"x, sX, deltaX when standing on MS: %f, %f, %f\n", x, dynamic_cast<MStair*>(e->obj)->x,deltaXToMStair);
						SetY(sbb->y-deltaSBB_y);
						deltaSBB_x -= dx;
						SetVY(0);

					}
					
					
				}
				else if (dynamic_cast<GreenFatStuff*>(e->obj)) {
 					if (e->ny < 0) {
						if (!standingOnGFS) {
							SetStandingOnGFS(true, dynamic_cast<GreenFatStuff*>(e->obj));
						}
						else {
							SetX(gfs->x + deltaGFS_x + dx);
							deltaGFS_x += dx;
							SetY(gfs->y + deltaGFS_y);
						}
					}
					else {
						GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
						if (e->ny > 0) {
							if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
								SetX(x - min_tx * dx - nx * 0.3f + dx);
								SetY(y - min_ty * dy - ny * 0.3f);
							}
						}
					}
					if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
						SetX(x - min_tx * dx - nx * 0.3f + dx);
						SetY(y - min_ty * dy - ny * 0.3f);
					}
					//dynamic_cast<GreenFatStuff*>(e->obj)->GetHit();

				}
				else if (dynamic_cast<LongNeckedBird*>(e->obj)) {
					LongNeckedBird* b = dynamic_cast<LongNeckedBird*>(e->obj);
					if (b->IsWaiting()) {
						if (e->nx != 0) {
							b->GetPushed();
							SetX(b->x-width);
						}
						else {
							if (state != GIMMICK_STATE_WALKING_RIGHT || state != GIMMICK_STATE_WALKING_LEFT) {
								landedWhen = GetTickCount();
								CGameObject::SetState(GIMMICK_STATE_IDLE);
							}
							SetVY(-GIMMICK_GRAVITY);
						}
					}
					else {
						if (e->ny != 0) {
							if (!standingOnBird) {
								SetStandingOnBird(true, b);
							}
							else {
								SetX(bird->x + deltaBird_x + dx);
								SetY(bird->y + deltaBird_y);
								deltaBird_x += dx;
							}
						}
						
					}
				}
				else if (dynamic_cast<Turtle*>(e->obj)) {
					if (e->ny!=0) {
						if (dynamic_cast<Turtle*>(e->obj)->IsTurnedUp()) {
							if (state != GIMMICK_STATE_WALKING_RIGHT || state != GIMMICK_STATE_WALKING_LEFT) {
								landedWhen = GetTickCount();
								CGameObject::SetState(GIMMICK_STATE_IDLE);
							}
							SetVY(-GIMMICK_GRAVITY);
						}
						else {
							GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
							if (e->ny > 0) {
								if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(y - min_ty * dy - ny * 0.3f);
								}
							}
						}
						
					}
					else {
						if (!dynamic_cast<Turtle*>(e->obj)->IsTurnedUp()) {
							GetInjured(INJURED_BY_ENEMY, e->nx > 0 || vx < 0);
							if (e->ny > 0) {
								if (GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(y - min_ty * dy - ny * 0.3f);
								}
							}
						}
						else {
							x = x;
						}
					}
					if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME) {
						SetX(x - min_tx * dx - nx * 0.3f + dx);
						SetY(y - min_ty * dy - ny * 0.3f);
					}
				}
				
			}
			else if (dynamic_cast<DynamicObject*>(e->obj)) {
				if (dynamic_cast<HangingMachine*>(e->obj)) {
					HangingMachine* m = dynamic_cast<HangingMachine*>(e->obj);
					if (m->isWaiting()) m->Go();
					else if (m->isFalling()) {
						m->GetDestroyed();
						SetVY(GIMMICK_CRASH_Y_SPEED);
						SetVX(GIMMICK_CRASH_X_SPEED);
						GetInjured(INJURED_BY_OTHER);
						SetX(x - min_tx * dx - nx * 0.3f );
						SetY(y - min_ty * dy + ny * 0.3f);
					}
					
				}
				else if (dynamic_cast<WheelGunMachine*>(e->obj)) {
					WheelGunMachine* m = dynamic_cast<WheelGunMachine*>(e->obj);
					if (e->nx > 0) {
						m->GetPushed(false);
						SetX(m->x + m->width);
					}
					else if (e->nx < 0) {
						m->GetPushed(true);
						SetX(m->x -width);
					}
					else {
						if (!standingOnWGM) {
							SetStandingOnWGM(true, m);

						}
						else {
							SetX(wgm->x + deltaWGM_x + dx);
							deltaWGM_x += dx;
							SetY(wgm->y + height);
						}
						/*if (state != GIMMICK_STATE_WALKING_RIGHT || state != GIMMICK_STATE_WALKING_LEFT)
							CGameObject::SetState(GIMMICK_STATE_IDLE);
						SetVY(0);
						SetY(m->y - height + ny*0.1f);*/
					}
				}
				else if (dynamic_cast<FlyingShell*>(e->obj)) {
					FlyingShell* m = dynamic_cast<FlyingShell*>(e->obj);
					if (e->ny < 0) {
						if (!standingOnShell) {
							SetStandingOnShell(true, m);
						}
						else {
							SetX(shell->x + deltaShell_x + dx);
							deltaShell_x += dx;
							SetY(shell->y + deltaShell_y);
							/*if (x + width < shell->x || shell->x + shell->width < x) {
								SetVY(GIMMICK_GRAVITY);
								CGameObject::SetState(direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_LEFT);
								SetStandingOnShell(false);
							}*/
						}
					}
					
				}
				else if (dynamic_cast<Shell*>(e->obj)) {
					Shell* m = dynamic_cast<Shell*>(e->obj);
					m->Explode();
					GetInjured(INJURED_BY_OTHER, e->nx > 0 || vx > 0);

				}
				else if (dynamic_cast<OnBugHeadShell*>(e->obj)) {
					OnBugHeadShell* m = dynamic_cast<OnBugHeadShell*>(e->obj);
					m->Explode();
					GetInjured(INJURED_BY_OTHER, e->nx > 0 || vx > 0);

				}
				else if (dynamic_cast<WindowThrowingShell*>(e->obj)) {
					WindowThrowingShell* w = dynamic_cast<WindowThrowingShell*>(e->obj);
					if (w->IsWaiting()) {
						w->ThrowShell();
					}
					else if (w->IsFalling()) {
						w->Explode();
						GetInjured(INJURED_BY_OTHER, e->nx > 0 || vx > 0);
					}
				}
				else if (dynamic_cast<Boat*>(e->obj)) {
					if (!standingOnBoat) {
						Boat* b = dynamic_cast<Boat*>(e->obj);
						if (b->IsWaiting())
							b->Go();
						SetStandingOnBoat(true, b);
					}
					else {
						SetX(boat->x + deltaBoat_x + dx);
						deltaBoat_x += dx;
						SetY(boat->y + deltaBoat_y);
					}
				}
				else if (dynamic_cast<HangingElevator*>(e->obj)) {
					if (!standingOnHangingElevator) {
						SetStandingHangingElevator(true, he = dynamic_cast<HangingElevator*>(e->obj));
						he->Go();
						landedWhen = GetTickCount();
						CGameObject::SetState(GIMMICK_STATE_IDLE);
						if (e->nx != 0) {
							SetX(x - min_tx * dx - nx * 0.1f + dx);

						}
						else {
							
						}

					}
					else {
						SetX(dynamic_cast<HangingElevator*>(e->obj)->GetX() - deltaHE_x +dx);
						//DebugOut(L"x, sX, deltaX when standing on MS: %f, %f, %f\n", x, dynamic_cast<MStair*>(e->obj)->x,deltaXToMStair);
						SetY(dynamic_cast<HangingElevator*>(e->obj)->GetY() - deltaHE_y );
						deltaHE_x -= dx;

					}
					
					
				}
				else if (dynamic_cast<GunMachine*>(e->obj)) {
					if (e->nx != 0) {
						GunMachine* m = dynamic_cast<GunMachine*>(e->obj);
						if (gmachine == NULL) {
							if (climbingHill == OutOfArea) {
								gmachine = m;
								gm_timer = GetTickCount();
							}
							else {
								SetX(x - min_tx * dx - nx * 0.1f);
								SetY(y - min_ty * dy + ny * 0.1f);
							}
							
						}
						else if (GetTickCount() - gm_timer > TIME_TO_PUSH_GM) {
							if (m->isBeingPushed() == 0)
								m->GetPushed(e->nx > 0 ? -1 : 1);
							else if (m->isBeingPushed() == 1) {
								gm_timer_small = GetTickCount();
								SetX(m->GetX() -width-0.1);
							}
							else{
								gm_timer_small = GetTickCount();

								SetX(m->GetX() +m->GetWidth()+0.1);

							}
						}
					}
					if (e->ny != 0) {
						CGameObject::SetState(GIMMICK_STATE_IDLE);
						landedWhen = GetTickCount();

						SetVY(0);
						//SetY(e->obj->y - height - 0.1);
							
						if (vx == 0) {
							landedWhen = GetTickCount();
							CGameObject::SetState(GIMMICK_STATE_IDLE);
						}
						else
							CGameObject::SetState(direction ? GIMMICK_STATE_WALKING_RIGHT : GIMMICK_STATE_WALKING_LEFT);
					}

				}
			}
			if (dynamic_cast<Item*>(e->obj)) {
				if (dynamic_cast<Jar*>(e->obj)) {
					Jar* j = dynamic_cast<Jar*>(e->obj);
					if (e->nx != 0) {
						//j->SetTaken(true);

						if (j->GetType() == 1) {
							IncreaseLife(GET_JAR);
							j->SetTaken(true);

						}
						else if (j->GetType()==0){
							if (AddItem(GET_LIQUID))
								j->SetTaken(true);
							else
								j->GetMoved(e->nx < 0);
						}
						else if (j->GetType() == 3) {
							if (AddItem(GET_BOMB))
								j->SetTaken(true);
							else
								j->GetMoved(e->nx < 0);
						}
						continue;
					}
					if (e->ny < 0) {
						if (CGame::GetInstance()->IsKeyDown(208)) {
							//j->SetTaken(true);
							SetVY(-GIMMICK_GRAVITY);
							if (j->GetType() == 1) {
								IncreaseLife(GET_JAR);
								j->SetTaken(true);
								SetVY(0);
							}
							else if (j->GetType() == 0) {
								if (AddItem(GET_LIQUID))
									j->SetTaken(true);
								SetVY(0);
							}
							else if (j->GetType() == 3) {
								if (AddItem(GET_BOMB))
									j->SetTaken(true);
								SetVY(0);
							}
							continue;
						}
						else SetVY(0);
						if (vx == 0) {
							landedWhen = GetTickCount();
							CGameObject::SetState(GIMMICK_STATE_IDLE);
						}
						else
							CGameObject::SetState(direction ? GIMMICK_STATE_WALKING_RIGHT : GIMMICK_STATE_WALKING_LEFT);
					}
				}
				else if (dynamic_cast<BigPrize*>(e->obj)) {
					BigPrize* j = dynamic_cast<BigPrize*>(e->obj);
					if (e->nx != 0) {
						j->SetTaken(true);
						IncreaseScore(GET_BIG_PRIZE);
						continue;
					}
					if (e->ny < 0) {
						if (CGame::GetInstance()->IsKeyDown(208)) {
							j->SetTaken(true);
							SetVY(-GIMMICK_GRAVITY);
							IncreaseScore(GET_BIG_PRIZE);
							continue;
						}
						else SetVY(0);
						if (vx == 0) {
							landedWhen = GetTickCount();
							CGameObject::SetState(GIMMICK_STATE_IDLE);
						}
						else
							CGameObject::SetState(direction ? GIMMICK_STATE_WALKING_RIGHT : GIMMICK_STATE_WALKING_LEFT);
					}
				}
				/*if (e->nx != 0) {
					dynamic_cast<Item*>(e->obj)->SetTaken(true);
					continue;
				}
				if (e->ny < 0) {
					if (CGame::GetInstance()->IsKeyDown(208)) {
						dynamic_cast<Item*>(e->obj)->SetTaken(true);
						SetVY(-GIMMICK_GRAVITY);
						continue;
					}
					else SetVY(0);
					if (vx == 0)
						CGameObject::SetState(GIMMICK_STATE_IDLE);
					else
						CGameObject::SetState(direction ? GIMMICK_STATE_WALKING_RIGHT : GIMMICK_STATE_WALKING_LEFT);
				}*/

			}
			else if (dynamic_cast<StaticObject*>(e->obj)) {
				if (e->nx != 0) {
					if (dynamic_cast<Elevator*>(e->obj)) {
 						Elevator* el = dynamic_cast<Elevator*>(e->obj);
						/*if (CGame::GetInstance()->IsKeyDown(203)) {
							el->GetDirection() ? SetVX(GIMMICK_WALKING_SPEED * (-0.6)) : SetVX(GIMMICK_WALKING_SPEED * (-2));
							SetX(x + vx*dt);
						}
						else if (CGame::GetInstance()->IsKeyDown(205)) {
							el->GetDirection() ? SetVX(GIMMICK_WALKING_SPEED * (2)) : SetVX(GIMMICK_WALKING_SPEED * (0.6));
							SetX(x + vx * dt);
						}
						else {
							el->GetDirection() ? SetVX(GIMMICK_WALKING_SPEED * 0.5) : SetVX(GIMMICK_WALKING_SPEED * (-0.5));
							SetX(x + vx * dt);
						}*/
						SetX(x + vx * dt);
						CGameObject::SetState(GIMMICK_STATE_IDLE);
						landedWhen = GetTickCount();

						SetStandingOnElevator(el->GetDirection()?1:-1, el->x, el->x + el->width);
						
						continue;
					}
					else if (dynamic_cast<Hill*>(e->obj)) {
						if (!standingOnSBB) {
							Hill* hi = dynamic_cast<Hill*>(e->obj);
							//SetStangdingOnHill(true, hi);
							//inHillArea = true;
							climbingHill = Enter;
							hill = hi;
						}
						
						SetX(x - min_tx * dx - nx * 0.1f + dx);
						//SetY(y - min_ty * dy - ny * 0.1f + dy);
					}
					else if (dynamic_cast<Pipe*>(e->obj)) {
						if (!dynamic_cast<Pipe*>(e->obj)->CanWalk()) {
							CGameObject::SetState(GIMMICK_STATE_IN_PIPE);
							if (e->nx != 0) {
								if (dynamic_cast<Pipe*>(e->obj)->GetType() == top_left) {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(dynamic_cast<Pipe*>(e->obj)->y);
								}
								else {
									SetX(x - min_tx * dx - nx * 0.3f + dx);
									SetY(dynamic_cast<Pipe*>(e->obj)->y);
								}
								

							}
						}
						/*if (e->nx != 0) {
							SetX(x - min_tx * dx - nx * 0.3f + dx);
							SetY(dynamic_cast<Pipe*>(e->obj)->y);
							
						}*/
						else {
							SetY(y - min_ty * dy + ny * 0.3f );
							if(e->nx<0)
								SetX(dynamic_cast<Pipe*>(e->obj)->x-width+1);
							else
								SetX(dynamic_cast<Pipe*>(e->obj)->x+ dynamic_cast<Pipe*>(e->obj)->width);

						}
						pipe = dynamic_cast<Pipe*>(e->obj);
						SetVY(0);
						SetVX(0);
					}
					else if (dynamic_cast<Ground*>(e->obj)) {
						if (state == GIMMICK_STATE_IN_PIPE) {
							SetVX(0);
							//SetVX(pipe->x);
							SetY(y - min_ty * dy + ny * 0.3f+dy);

							/*SetX(x - nx * 0.1f );
							if (nx < 0)
								SetX(dynamic_cast<Ground*>(e->obj)->x - width);
							else SetX(dynamic_cast<Ground*>(e->obj)->x + dynamic_cast<Ground*>(e->obj)->width);*/

						} 
						if (standingOnSBB) {
							collideWithGround__ = true;
							if (e->nx > 0) {
								x = dynamic_cast<Ground*>(e->obj)->x + dynamic_cast<Ground*>(e->obj)->width + 1;
							}else {
								x = dynamic_cast<Ground*>(e->obj)->x -width - 1;
							}
							deltaSBB_x = sbb->x - x;
						}
						else if (standingOnSB) {
							collideWithGround__ = true;
							if (e->nx > 0) {
								x = dynamic_cast<Ground*>(e->obj)->x + dynamic_cast<Ground*>(e->obj)->width + 1;
							}
							else {
								x = dynamic_cast<Ground*>(e->obj)->x - width - 1;
							}
							deltaSB_x = sb->x - x;
						}
						if (climbingHill != 0) {
							SetX(x - min_tx * dx - nx * 0.1f + dx);
						}
						if (state == GIMMICK_STATE_WALKING_RIGHT || state == GIMMICK_STATE_WALKING_LEFT || state==GIMMICK_STATE_IDLE)
							SetX(x + min_tx * dx + nx * 0.1f - dx);

					}
					else {
						SetVX(0);
						continue;
					}
					
				}
 				else if (e->ny < 0 ) {
					if (dynamic_cast<Elevator*>(e->obj)) {
						// SetVY(vy + GIMMICK_GRAVITY * dt);
						SetY(dynamic_cast<Elevator*>(e->obj)->y+13);
						CGameObject::SetState(GIMMICK_STATE_IDLE);
						landedWhen = GetTickCount();

						SetVY(0);
						SetStandingOnElevator(dynamic_cast<Elevator*>(e->obj)->GetDirection() ? 1 : -1, 
							dynamic_cast<Elevator*>(e->obj)->x, 
							dynamic_cast<Elevator*>(e->obj)->x + dynamic_cast<Elevator*>(e->obj)->width);
					}else if (dynamic_cast<Hill*>(e->obj)) {
						// SetVY(vy + GIMMICK_GRAVITY * dt);
						//SetY(y + vy * dt);
						// 
						if (!standingOnSBB) {
							if (collideWithGroundOnY) {
								CGameObject::SetState(GIMMICK_STATE_IDLE);
								landedWhen = GetTickCount();

								SetVY(0);
							}
							/*CGameObject::SetState(GIMMICK_STATE_IDLE);
						SetVY(0);*/

						// SetStangdingOnHill(true, dynamic_cast<Hill*>(e->obj));

						//inHillArea = true;
							if (climbingHill == OutOfArea) {

							}
							climbingHill = Enter;
							hill = dynamic_cast<Hill*>(e->obj);
							
						}
						//SetY(y + (min_ty + 0.1) * dy);
						SetY(y - min_ty * dy + ny * 0.1f + dy);


						//SetY(y + (min_ty + 0.1) * dy);
						//if (collideWithGroundOnY) {
						//	CGameObject::SetState(GIMMICK_STATE_IDLE);
						//	SetVY(0);
						//}
						//	/*CGameObject::SetState(GIMMICK_STATE_IDLE);
						//SetVY(0);*/

						//// SetStangdingOnHill(true, dynamic_cast<Hill*>(e->obj));

						////inHillArea = true;
						//climbingHill = Enter;
						//hill = dynamic_cast<Hill*>(e->obj);
					}
					else if (dynamic_cast<MStair*>(e->obj)) {
						// SetVY(0);
						if (standingOnMovingStair) {
							landedWhen = GetTickCount();
							SetX(dynamic_cast<MStair*>(e->obj)->x + deltaXToMStair);
							deltaXToMStair += dx;
							//DebugOut(L"x, sX, deltaX when standing on MS: %f, %f, %f\n", x, dynamic_cast<MStair*>(e->obj)->x,deltaXToMStair);
							SetY(dynamic_cast<MStair*>(e->obj)->y + deltaYToMStair);

						}/*else SetStandingMovingStair(true,
							ceil(dynamic_cast<MStair*>(e->obj)->x),
							ceil(dynamic_cast<MStair*>(e->obj)->x) + dynamic_cast<MStair*>(e->obj)->width,
							dynamic_cast<MStair*>(e->obj)->y,
							dynamic_cast<MStair*>(e->obj)) ;*/
						else SetStandingMovingStair(
							true,
							dynamic_cast<MStair*>(e->obj)->x,
							dynamic_cast<MStair*>(e->obj)->y,
							dynamic_cast<MStair*>(e->obj)
						);
					}
					else if (dynamic_cast<Ground*>(e->obj)) {
						if (state == GIMMICK_STATE_IN_PIPE) {
							SetVX(0);
							//SetY (pipe->y);
;						}
						else {

							if ((vx == 0 || standingOnElevator == true) && GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME) {
								landedWhen = GetTickCount();
								CGameObject::SetState(GIMMICK_STATE_IDLE);
							}
							else if(climbingHill!=Climbing && GetTickCount() - faintedTimer > GIMMICK_FAINTED_TIME)
								CGameObject::SetState(direction ? GIMMICK_STATE_WALKING_RIGHT : GIMMICK_STATE_WALKING_LEFT);

							SetVY(-GIMMICK_GRAVITY);
							if (state == GIMMICK_STATE_INJURED)
							{
								SetVX(0);
							}

						}
						
					}
					else if (dynamic_cast<Pipe*>(e->obj)) {
						CGameObject::SetState(GIMMICK_STATE_IN_PIPE);
						SetStandingOnSBB(false);
						pipe = dynamic_cast<Pipe*>(e->obj);
						SetY(pipe->y+height-1);

					}
				}
				else {
					if (dynamic_cast<Pipe*>(e->obj)) {
						CGameObject::SetState(GIMMICK_STATE_IN_PIPE);
						pipe = dynamic_cast<Pipe*>(e->obj);
						SetY(pipe->y - height + 1);
						SetStandingOnSBB(false);


					}
					else if (dynamic_cast<Ground*>(e->obj)) {
						if (state == GIMMICK_STATE_IN_PIPE) {
							SetVY(0);
							SetY(pipe->y);
						}
					}
					else {
						SetVY(-vy);
					}

						SetVY(0);
				}
			}
		}

		//
		// Collision logic with other objects
		//
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];

		//	if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
		//	{
		//		CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

		//		// jump on top >> kill Goomba and deflect a bit 
		//		if (e->ny < 0)
		//		{
		//			if (goomba->GetState() != GOOMBA_STATE_DIE)
		//			{
		//				goomba->SetState(GOOMBA_STATE_DIE);
		//				vy = -MARIO_JUMP_DEFLECT_SPEED;
		//			}
		//		}
		//		else if (e->nx != 0)
		//		{
		//			if (untouchable == 0)
		//			{
		//				if (goomba->GetState() != GOOMBA_STATE_DIE)
		//				{
		//					if (level > MARIO_LEVEL_SMALL)
		//					{
		//						level = MARIO_LEVEL_SMALL;
		//						StartUntouchable();
		//					}
		//					else
		//						SetState(MARIO_STATE_DIE);
		//				}
		//			}
		//		}
		//	} // if Goomba
		//	else if (dynamic_cast<CPortal *>(e->obj))
		//	{
		//		CPortal *p = dynamic_cast<CPortal *>(e->obj);
		//		CGame::GetInstance()->SwitchScene(p->GetSceneId());
		//	}
		//}
	}

	//if(gmachine!=NULL&&)
	if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (standingOnSBB) {
		if(sbb->state==0)
			SetStandingOnSBB(false);
		landedWhen = GetTickCount();
		if (standingOnSBB) {
			if (!collideWithGround__) {
				SetX(sbb->x - deltaSBB_x + dx);
				deltaSBB_x -= dx;
			}
			else {

			}

			SetY(sbb->y + 16);
			SetVY(0);
			if (x + width < sbb->x || sbb->x + sbb->width < x) {
				SetVY(vy - GIMMICK_GRAVITY * dt);
				SetStandingOnSBB(false);
			}
		}
		
	}
	else if (standingOnBoat) {
		SetVY(0);
		SetVX(0);
	}
	else if (standingOnGFS) {
		SetVY(gfs->vy);
		SetVX(gfs->vx);
	}
	else if (standingOnBird) {
		SetVY(0);
		SetVX(bird->vx);
	}
	else if (standingOnSB) {
		if (sb->state == 0)
			SetStandingOnSB(false);
		landedWhen = GetTickCount();
		if (standingOnSB) {
			if (!collideWithGround__) {
				SetX(sb->x - deltaSB_x + dx);
				deltaSB_x -= dx;
			}
			else {

			}

			SetY(sb->y + 16);
			SetVY(0);
			if (x + width < sb->x || sb->x + sb->width < x) {
				SetVY(vy - GIMMICK_GRAVITY * dt);
				SetStandingOnSB(false);
			}
		}

	}
	if (lockedInView) {
		if (x +width > viewFrom + 250) {
			/*x -= GIMMICK_WALKING_SPEED * 2*dt;
			if (standingOnSB) {
				SetStandingOnSB(false);
				SetVY(vy - GIMMICK_GRAVITY * dt);

			}*/
		}
		else if (x < viewFrom) {
			x += GIMMICK_WALKING_SPEED * 2 * dt;

		}
	}
	if (LockedToView) {
		if (x + width > viewTo) {
			x -= GIMMICK_WALKING_SPEED * 2 * dt;
			if (standingOnSB) {
				SetStandingOnSB(false);
				SetVY(vy - GIMMICK_GRAVITY * dt);

			}
		}
		/*else if (x < viewFrom) {
			x += GIMMICK_WALKING_SPEED * 2 * dt;

		}*/
	}
	// clean up collision events
	// for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	// _checkSweptAABB(colliable_objects);

	// SetStandingOnElevator(collidingWithElevator);

	if (star_phase >= 3) {
		if (stars.size()==0) {
			stars.push_back(Star(x + STAR_X_MARGIN, y+STAR_Y_MARGIN, this->hill));
			stars.push_back(Star(x + STAR_X_MARGIN, y+STAR_Y_MARGIN, this->hill));
			stars.push_back(Star(x + STAR_X_MARGIN, y+STAR_Y_MARGIN, this->hill));
			star_1 = GetTickCount();
			star_2 = GetTickCount();

			stars[0].prev_x = x + STAR_X_MARGIN; stars[0].prev_y = y+ STAR_Y_MARGIN;
			stars[1].prev_x = x + STAR_X_MARGIN; stars[1].prev_y = y + STAR_Y_MARGIN;
		}
		else {
			if (star_phase == 4) {
				if (GetTickCount() - stars_timer > STAR_EXISTING_TIME) {
					stars[0].GetDestroyed();
					stars[1].GetDestroyed();
					stars[2].GetDestroyed();
					SetStandingOnStar(false);
					star_phase = 0;
					stars_timer = GetTickCount();
				}
				else {
					stars[0].Update(dt, colliable_objects);
					stars[1].Update(dt, colliable_objects, false);
					stars[2].Update(dt, colliable_objects, false);
					if (!aboutToDestroyStar) {
						if (GetTickCount() - star_1 > STAR_TIME_DELAY) {
							star_1 = GetTickCount();
							if (abs(stars[0].prev_x - stars[0].x) < STAR_DISTANCE)
								stars[1].x = stars[0].prev_x;
							else
								stars[1].x = stars[0].x > stars[0].prev_x ? stars[0].x - STAR_DISTANCE : stars[0].x + STAR_DISTANCE;
							if (abs(stars[0].prev_y - stars[0].y) < STAR_DISTANCE)
								stars[1].y = stars[0].prev_y;
							else
								stars[1].y = stars[0].y > stars[0].prev_y ? stars[0].y - STAR_DISTANCE : stars[0].y + STAR_DISTANCE;
							//stars[1].SetXY(stars[0].prev_x, stars[0].prev_y);
							stars[0].prev_x = stars[0].x; stars[0].prev_y = stars[0].y;
						}

						if (GetTickCount() - star_2 > STAR_TIME_DELAY * 2) {
							star_2 = GetTickCount();
							if (abs(stars[1].prev_x - stars[1].x) < STAR_DISTANCE)
								stars[2].x = stars[1].prev_x;
							else
								stars[2].x = stars[1].x > stars[1].prev_x ? stars[1].x - STAR_DISTANCE : stars[1].x + STAR_DISTANCE;
							if (abs(stars[1].prev_y - stars[1].y) < STAR_DISTANCE)
								stars[2].y = stars[1].prev_y;
							else
								stars[2].y = stars[1].y > stars[1].prev_y ? stars[1].y - STAR_DISTANCE : stars[1].y + STAR_DISTANCE;

							//stars[2].SetXY(stars[1].prev_x, stars[1].prev_y);
							stars[1].prev_x = stars[1].x; stars[1].prev_y = stars[1].y;
						}
					}
					else {
						ExplodeStars();
						
					}
					
				}
				

				
			}
			else {
				stars[0].Update(dt, colliable_objects);
				stars[1].Update(dt, colliable_objects, false);
				stars[2].Update(dt, colliable_objects, false);
				stars[0].SetXY(x + STAR_X_MARGIN, y + STAR_Y_MARGIN);
				stars[0].vy = 0;

				if (GetTickCount() - star_1 > STAR_TIME_DELAY) {
					star_1 = GetTickCount();
					if (abs(stars[0].prev_x - stars[0].x) < STAR_DISTANCE)
						stars[1].x = stars[0].prev_x;
					else
						stars[1].x = stars[0].x > stars[0].prev_x ? stars[0].x - STAR_DISTANCE : stars[0].x + STAR_DISTANCE;
					if (abs(stars[0].prev_y - stars[0].y) < STAR_DISTANCE)
						stars[1].y = stars[0].prev_y;
					else
						stars[1].y = stars[0].y > stars[0].prev_y ? stars[0].y - STAR_DISTANCE : stars[0].y + STAR_DISTANCE;
					//stars[1].SetXY(stars[0].prev_x, stars[0].prev_y);
					stars[0].prev_x = x + STAR_X_MARGIN; stars[0].prev_y = y + STAR_Y_MARGIN;
				}

				if (GetTickCount() - star_2 > STAR_TIME_DELAY) {
					star_2 = GetTickCount();
					if (abs(stars[1].prev_x - stars[1].x) < STAR_DISTANCE)
						stars[2].x = stars[1].prev_x;
					else
						stars[2].x = stars[1].x > stars[1].prev_x ? stars[1].x - STAR_DISTANCE : stars[1].x + STAR_DISTANCE;
					if (abs(stars[1].prev_y - stars[1].y) < STAR_DISTANCE)
						stars[2].y = stars[1].prev_y;
					else
						stars[2].y = stars[1].y > stars[1].prev_y ? stars[1].y - STAR_DISTANCE : stars[1].y + STAR_DISTANCE;

					//stars[2].SetXY(stars[1].prev_x, stars[1].prev_y);
					stars[1].prev_x = stars[1].x; stars[1].prev_y = stars[1].y;
				}
			}
			
		}
	}
	if (bomb_phase >= Thrown) {
		bomb->Update(dt, colliable_objects);
		if (bomb->gone) {
			bomb_phase = NotExist;
			bomb = NULL;
		}
		/*if (!bomb->destroyed)
			bomb->Update(dt, colliable_objects);
		else
			bomb_phase = SpreadingStars;*/
	}
	if (fire_phase >= FireOnHead) {
		fire->Update(dt, colliable_objects);
		if (fire_phase == FireOnHead) {
			fire->x = x;
			fire->y = y + 20;
		}
		if (fire->gone) {
			fire_phase = NotExist;
			fire = NULL;
		}
		/*if (!bomb->destroyed)
			bomb->Update(dt, colliable_objects);
		else
			bomb_phase = SpreadingStars;*/
	}
	if (x < 0) {
		x = 0;
	}
	else if (x > dynamic_cast<CPlayScene*>(CSceneManager::GetInstance()->GetCurrentScene())->mapWidth) {
		x = dynamic_cast<CPlayScene*>(CSceneManager::GetInstance()->GetCurrentScene())->mapWidth;
	}
	if (y > dynamic_cast<CPlayScene*>(CSceneManager::GetInstance()->GetCurrentScene())->mapHeight) {
		y = dynamic_cast<CPlayScene*>(CSceneManager::GetInstance()->GetCurrentScene())->mapHeight;
	}
	else if (y < 0 && state!=GIMMICK_STATE_JUMP) {
		y = 1;
	}
	instance = this;
	// DebugOut(L"VX, dx in Gimmick: %f, %f\n", vx,dx);

}

void CGimmick::ExplodeStars() {
	if (stars.size() != 0) {
		stars[0].GetDestroyed();
		stars[1].GetDestroyed();
		stars[2].GetDestroyed();

		stars[1].x = stars[2].x = stars[0].x;
		stars[1].y = stars[2].y = stars[0].y;

		star_phase = 0;
		stars_timer = GetTickCount();
		aboutToDestroyStar = false;
	}
}
void CGimmick::Render()
{
	
	/*if (vx == 0 || abs(vx)<0.55*GIMMICK_WALKING_SPEED)
	{
		if (nx>0) ani = GIMMICK_ANI_IDLE_RIGHT;
		else ani = GIMMICK_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = GIMMICK_ANI_WALKING_RIGHT;
	else ani = GIMMICK_ANI_WALKING_LEFT;*/
	int ani = -1;
	if (state == GIMMICK_STATE_DIE) {
		ani = GIMMICK_ANI_DIE;
	}
	if (state == GIMMICK_STATE_JUMP)
		ani = direction ? GIMMICK_ANI_JUMPING_RIGHT : GIMMICK_ANI_JUMPING_RIGHT + CAnimationSets::GetInstance()->Get(1)->size() / 2;
	if (state == GIMMICK_STATE_IDLE)
		ani = direction ? GIMMICK_ANI_IDLE_RIGHT : GIMMICK_ANI_IDLE_RIGHT+ CAnimationSets::GetInstance()->Get(1)->size()/2;
	else if (state == GIMMICK_STATE_WALKING_LEFT) 
		ani = GIMMICK_ANI_WALKING_RIGHT+ CAnimationSets::GetInstance()->Get(1)->size() / 2;
	else if (state == GIMMICK_STATE_WALKING_RIGHT) 
		ani = GIMMICK_ANI_WALKING_RIGHT;
	else if(state == GIMMICK_STATE_IN_PIPE)
		ani = direction ? GIMMICK_ANI_IN_PIPE_RIGHT : GIMMICK_ANI_IN_PIPE_RIGHT + CAnimationSets::GetInstance()->Get(1)->size() / 2;
	else if(state == GIMMICK_STATE_INJURED)
		ani = direction ? GIMMICK_ANI_FAINTED_RIGHT : GIMMICK_ANI_FAINTED_RIGHT + CAnimationSets::GetInstance()->Get(1)->size() / 2;

	int alpha = 255;
	if (untouchable) alpha = 128;
	float rx, ry;
	if (state == GIMMICK_STATE_IN_PIPE) {
		if (GetTickCount() - injuringTimer < GIMMICK_INJURED_TIME && GetTickCount() - faintedTimer>GIMMICK_FAINTED_TIME) {
			CAnimationSets::GetInstance()->Get(1)->at(ani + 6)->Render(x - 4, y + 6, alpha);
		}
		else {
			CAnimationSets::GetInstance()->Get(1)->at(ani)->Render(x - 4, y + 6, alpha);
		}
	}
	else {
		if (GetTickCount() - injuringTimer < GIMMICK_INJURED_TIME && GetTickCount() - faintedTimer>GIMMICK_FAINTED_TIME) {
			CAnimationSets::GetInstance()->Get(1)->at(ani + 6)->Render(x - 4, y + 9, alpha);
		}
		else {
			CAnimationSets::GetInstance()->Get(1)->at(ani)->Render(x - 4, y + 9, alpha);
		}
	}
	
	/*if (x > 100) rx = 100; 
	else rx = x;*/
	//if (y > )
//	DebugOut(L"location in Render: %f, %f\n", rx, y); 00;
	if (star_phase != 0) {
		if (star_phase==1) {
			/*if (count < 10) {
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x1[count],	y + 17 - y1[count], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x2[9-count], y + 17 - y2[9-count], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x3[9-count], y + 17 - y3[9-count], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x4[9-count], y + 17 - y4[9-count], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x5[9-count], y + 17 - y5[9-count], alpha);
			}
			if (count < 11) {
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x1[count-1],   y + 17 - y1[count-1], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x2[9-count+1], y + 17 - y2[9-count+1], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x3[9-count+1], y + 17 - y3[9-count+1], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x4[9-count+1], y + 17 - y4[9-count+1], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x5[9-count+1], y + 17 - y5[9-count+1], alpha);
			}
			if (count < 12) {
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x1[count - 2],   y + 17 - y1[count - 2], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x2[9-count + 2], y + 17 - y2[9-count + 2], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x3[9-count + 2], y + 17 - y3[9-count + 2], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x4[9-count + 2], y + 17 - y4[9-count + 2], alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3+x5[9-count + 2], y + 17 - y5[9-count + 2], alpha);
			}*/
			
			/*if (count >= 11) {
				star_phase = 2;
				stars_timer = GetTickCount();
				count = 0;
			}*/
			radius += RADIUS_SPEED * (1);
			distance -= DISTANCE_SPEED * (30);
			if (distance <= 0) {
				count += 1000;
			}

			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0)), y + 17 - distance0 * sin(D3DXToRadian(radius0)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 2)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 3)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 3)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 4)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 4)), alpha);


			if (count >= 1) {
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1)), Y1 - distance1 * sin(D3DXToRadian(radius1)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 2)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 2)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 3)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 3)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 4)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 4)), alpha);

			}
			if (count >= 2) {
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2)), Y2 - distance2 * sin(D3DXToRadian(radius2)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 2)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 2)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 3)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 3)), alpha);
				CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 4)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 4)), alpha);

			}
			if (delay % 10 == 0) {


				distance2 = distance1;
				distance1 = distance0;
				distance0 = distance;
				radius2 = radius1;
				radius1 = radius0;
				radius0 = radius;
				X2 = X1; Y2 = Y1;
				X1 = x + 3; Y1 = y + 17;
			}

			delay++;

			if (delay % 10 == 0) {
				count++;
			}
			if (count >= 3000) {
				star_phase = 2;
				stars_timer = GetTickCount();
				count = 0;
			}
		}
		else if (star_phase == 2) {
			
			CAnimationSets::GetInstance()->Get(2)->at(1)->Render(x - 5, y + 7 + STAR_Y_MARGIN, alpha);
			if (GetTickCount() - stars_timer > GIMMICK_FORMING_STAR_PHASE_2) {
				star_phase = 3;
				if (stars.size() != 0) stars.clear();
				stars.push_back(Star(x + STAR_X_MARGIN, y + STAR_Y_MARGIN, this->hill));
				stars.push_back(Star(x + STAR_X_MARGIN, y + STAR_Y_MARGIN, this->hill));
				stars.push_back(Star(x + STAR_X_MARGIN, y + STAR_Y_MARGIN, this->hill));
				star_1 = GetTickCount();
				star_2 = GetTickCount();

				stars[0].prev_x = x + STAR_X_MARGIN; stars[0].prev_y = y + STAR_Y_MARGIN;
				stars[1].prev_x = x + STAR_X_MARGIN; stars[1].prev_y = y + STAR_Y_MARGIN;

			}
		} else if (star_phase >= 3) {
			stars[0].Render();
			stars[1].Render();
			stars[2].Render();
		}
		
	}
	else if (GetTickCount() - stars_timer > STAR_SMOKE_TIME) {
		if (stars.size()!=0) stars.clear();
		
	}
	else {
		stars[0].Render();
		stars[1].Render();
		stars[2].Render();
	}
	if (bomb_phase == Forming) {
		radius += RADIUS_SPEED * (1);
		distance -= DISTANCE_SPEED * (30 + delay);
		if (distance <= 0) {
			count += 1000;
		}

		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0)), y + 17 - distance0 * sin(D3DXToRadian(radius0)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 2)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 2)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 3)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 3)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 4)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 4)), alpha);


		if (count >= 1) {
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1)), Y1 - distance1 * sin(D3DXToRadian(radius1)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 2)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 3)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 3)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 4)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 4)), alpha);

		}
		if (count >= 2) {
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2)), Y2 - distance2 * sin(D3DXToRadian(radius2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 2)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 3)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 3)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 4)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 4)), alpha);

		}
		if (delay % 10 == 0) {


			distance2 = distance1;
			distance1 = distance0;
			distance0 = distance;
			radius2 = radius1;
			radius1 = radius0;
			radius0 = radius;
			X2 = X1; Y2 = Y1;
			X1 = x + 3; Y1 = y + 17;
		}

		delay++;

		if (delay % 10 == 0) {
			count++;
		}
		
		if (count >= 3000) {
			bomb_phase = Smoke;
			bomb_timer = GetTickCount();
			count = 0;
		}
	}
	else if (bomb_phase == Smoke) {
		CAnimationSets::GetInstance()->Get(BOMB_ANIMATION_SET_ID)->at(BOMB_SMOKE_STATE)->Render(x, y + 16);
		if (GetTickCount() - bomb_timer > BOMB_SMOKE_TIME) {
			bomb_phase = OnHead;
		}
	}
	else if (bomb_phase == OnHead) {
		CAnimationSets::GetInstance()->Get(BOMB_ANIMATION_SET_ID)->at(BOMB_BOMB_STATE)->Render(x, y + 16);
	}
	else if (bomb_phase >= Thrown) {
		bomb->Render();
	}

	if (fire_phase == Forming) {
		radius += RADIUS_SPEED * (1);
		distance -= DISTANCE_SPEED * (30+delay);
		if (distance <= 0) {
			count += 1000;
		}

		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0)), y + 17 - distance0 * sin(D3DXToRadian(radius0)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 2)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 2)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 3)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 3)), alpha);
		CAnimationSets::GetInstance()->Get(2)->at(0)->Render(x + 3 + distance0 * cos(D3DXToRadian(radius0 + 72 * 4)), y + 17 - distance0 * sin(D3DXToRadian(radius0 + 72 * 4)), alpha);


		if (count >= 1) {
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1)), Y1 - distance1 * sin(D3DXToRadian(radius1)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 2)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 3)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 3)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X1 + distance1 * cos(D3DXToRadian(radius1 + 72 * 4)), Y1 - distance1 * sin(D3DXToRadian(radius1 + 72 * 4)), alpha);

		}
		if (count >= 2) {
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2)), Y2 - distance2 * sin(D3DXToRadian(radius2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 2)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 2)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 3)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 3)), alpha);
			CAnimationSets::GetInstance()->Get(2)->at(0)->Render(X2 + distance2 * cos(D3DXToRadian(radius2 + 72 * 4)), Y2 - distance2 * sin(D3DXToRadian(radius2 + 72 * 4)), alpha);

		}
		if (delay%10 == 0) {


			distance2 = distance1;
			distance1 = distance0;
			distance0 = distance;
			radius2 = radius1;
			radius1 = radius0;
			radius0 = radius;
			X2 = X1; Y2 = Y1;
			X1 = x + 3; Y1 = y + 17;
		}

		delay++;

		if (delay % 10 == 0) {
			count++;
		}
		if (count >= 3000) {
			fire_phase = FireSmallFlame;
			fire = new CFire(x, y + 16+4);
			fire_timer = GetTickCount();
			count = 0;
		}
	}
	else if (fire_phase == FireSmallFlame) {
		CAnimationSets::GetInstance()->Get(7)->at(FIRE_SMALL_FLAME)->Render(x-1, y	 + 16 + 16 + 0);
		CAnimationSets::GetInstance()->Get(7)->at(FIRE_SMALL_FLAME)->Render(x + 8, y + 16 + 16 + 4);
		CAnimationSets::GetInstance()->Get(7)->at(FIRE_SMALL_FLAME)->Render(x+16, y	 + 16 + 16 + 2);

		fire->Render();
		if (GetTickCount() - fire_timer > FIRE_SMALL_FLAME_TIME) {
			fire_phase = FireOnHead;
		}
	}
	else if (fire_phase == FireOnHead) {
		CAnimationSets::GetInstance()->Get(7)->at(FIRE_BIG_BALL)->Render(x, y + 16 + 4);
	}
	else if (fire_phase >= FireThrown) {
		fire->Render();
	}

	// DebugOut(L"location in Render: %f, %f\n", x, y);
	//animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}
void CGimmick::SetStandingMovingStair(bool s, float sX, float sY, MStair* st) {
	standingOnMovingStair = s;
	SetVY(0);
	if (standingOnMovingStair != 0) {
		/*sFrom = from;
		sTo = to;*/
		deltaXToMStair = ceil(x - sX);
		deltaYToMStair = ceil(y - sY);
		SetY(st->y + height);

		
		SetStandingOnSBB(false);
		SetStandingOnSB(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnBird(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnWGM(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);

	}
	stair = st;

}
void CGimmick::SetStandingOnBoat(bool s, Boat* b) {
	standingOnBoat = s;
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	SetVY(-GIMMICK_GRAVITY);
	boat = b;
	if (standingOnBoat) {
		deltaBoat_x = x - boat->x;
		deltaBoat_y = y - boat->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSBB(false);
		SetStandingOnSB(false);
		SetStandingOnBird(false);
		SetStandingOnShell(false);
		SetStandingOnWGM(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetVX(boat->vx);

	}
}
void CGimmick::SetStandingOnBird(bool s, LongNeckedBird* b) {
	standingOnBird = s;
	
	/*if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT)
		CGameObject::SetState(GIMMICK_STATE_IDLE);*/
	SetVY(-GIMMICK_GRAVITY);
	bird = b;
	if (standingOnBird) {
		deltaBird_x = x - bird->x;
		deltaBird_y = y - bird->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnWGM(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
	}
}
void CGimmick::SetStandingOnStar(bool s, Star* b) {
	standingOnStar = s;

	/*if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT)
		CGameObject::SetState(GIMMICK_STATE_IDLE);*/
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	SetVY(-GIMMICK_GRAVITY);
	str = b;
	if (standingOnStar) {
		deltaSTAR_x = x - str->x;
		deltaSTAR_y = y - str->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnWGM(false);
		SetStandingOnGFS(false);
		SetStandingOnBird(false);

	}
	else {
		//CGameObject::SetState(GIMMICK_STATE_JUMP);
	}
}
void CGimmick::SetStandingOnGFS(bool s, GreenFatStuff* b) {
	standingOnGFS = s;
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	SetVY(-GIMMICK_GRAVITY);
	gfs = b;
	if (standingOnGFS) {
		deltaGFS_x = x - gfs->x;
		deltaGFS_y = y - gfs->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnWGM(false);
		SetStandingOnBird(false);
		SetStandingOnStar(false);

	}
}
void CGimmick::SetStandingOnShell(bool s, FlyingShell* b) {
	standingOnShell = s;
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	SetVY(-GIMMICK_GRAVITY);
	shell = b;
	if (standingOnShell) {
		deltaShell_x = x - shell->x;
		deltaShell_y = y - shell->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnBird(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetStandingOnWGM(false);
	}
}
void CGimmick::SetStandingOnWGM(bool s, WheelGunMachine* b) {
	standingOnWGM = s;
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	SetVY(-GIMMICK_GRAVITY);
	wgm = b;
	if (standingOnWGM) {
		deltaWGM_x = x - wgm->x;
		deltaWGM_y = y - wgm->y;
		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnBird(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetStandingOnShell(false);
	}
}
void CGimmick::SetStandingOnSBB(bool s, SmallBlackBug* b) {
	standingOnSBB = s;
	SetVX(0);
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED && state!=GIMMICK_STATE_IN_PIPE) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}
	

	if (standingOnSBB) {
		/*sFrom = from;
		sTo = to;*/
		sbb = b;
		deltaSBB_x = b->x - x;
		deltaSBB_y = b->y - y;

		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnBird(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetStandingOnWGM(false);
		sbb->SetCarringGimmick();
	}
	else {

		if(sbb!=NULL)
			sbb->StopCarringGimmick();
	}
	sbb = b;
}
void CGimmick::SetStandingOnSB(bool s, SmallBug* b) {
	standingOnSB = s;
	SetVX(0);
	if (state != GIMMICK_STATE_WALKING_RIGHT && state != GIMMICK_STATE_WALKING_LEFT && state != GIMMICK_STATE_INJURED && state != GIMMICK_STATE_IN_PIPE) {
		landedWhen = GetTickCount();
		CGameObject::SetState(GIMMICK_STATE_IDLE);
	}


	if (standingOnSB) {
		/*sFrom = from;
		sTo = to;*/
		sb = b;
		deltaSB_x = b->x - x;
		deltaSB_y = b->y - y;

		SetStandingMovingStair(false);
		SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnElevator(0);
		SetStandingOnBird(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnSBB(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetStandingOnWGM(false);
		//sbb->SetCarringGimmick();
	}
	else {

		/*if (sb != NULL)
			sb->StopCarringGimmick();*/
	}
	sb = b;
}
void CGimmick::SetStandingHangingElevator(bool s, HangingElevator* h) {
	standingOnHangingElevator = s;
	he = h;

	if (standingOnHangingElevator) {
		/*sFrom = from;
		sTo = to;*/
		deltaHE_x = he->GetX() - x;
		deltaHE_y = he->GetY() - y;

		SetStangdingOnHill(false);
		SetStandingMovingStair(false);
		SetStandingOnElevator(0);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnBird(false);
		SetStandingOnShell(false);
		SetStandingOnGFS(false);
		SetStandingOnWGM(false);
		SetStandingOnStar(false);

	}
	else {
		x = x;
	}

}
void CGimmick::SetStandingOnElevator(int s, float from, float to) {
	standingOnElevator = s;
	SetVY(0);
	if (standingOnElevator != 0) {
		eFrom = from; eTo = to;
		SetStandingMovingStair(false);
		//SetStangdingOnHill(false);
		SetStandingHangingElevator(false);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBird(false);
		SetStandingOnWGM(false);
		SetStandingOnShell(false);
		SetStandingOnGFS(false);
		SetStandingOnStar(false);
		SetStandingOnBoat(false);
	}
	
}
void CGimmick::SetStangdingOnHill(bool o, Hill* h) {
	//standingOnHill = o;
	hill = h;
	if (/*standingOnHill*/true) {
		//SetStandingOnElevator(0);
	}
	else {
		SetVX(0);
	}
	if (o) {
		SetStandingHangingElevator(false);
		SetStandingOnSB(false);
		SetStandingOnSBB(false);
		SetStandingOnBoat(false);
		SetStandingOnShell(false);
		SetStandingOnBird(false);
		SetStandingOnGFS(false);
		SetStandingOnWGM(false);
		SetStandingMovingStair(false);
		SetStandingOnStar(false);

	}
	
}

void CGimmick::SetVX(float _vx) {
	if (GetTickCount() - faintedTimer < GIMMICK_FAINTED_TIME)
		x = x;
	if (state==GIMMICK_STATE_INJURED)
		vx = vx;
	finalVX = _vx;
	if (vx < finalVX && finalVX >= 0) {
		vx += GIMMICK_FRICTION_FORCE;
		if (vx > finalVX)vx = finalVX;
	}
	else if (vx > finalVX && finalVX <= 0) {
		vx -= GIMMICK_FRICTION_FORCE;
		if (vx < finalVX)vx = finalVX;
	}
	//this->vx = _vx;
	// DebugOut(L"VX at time in Gimmick: %f\n", vx);

}
void CGimmick::SetVY(float _vy) {
	/*if (abs(vx)!= abs(_vy) && climbingHill==Climbing )
		vy = vy;*/
	/*if (climbingHill == Climbing) {
		if (hill == NULL)
			climbingHill == OutOfArea;
		else if (abs(vx) / abs(vy) != hill->GetXToYRatio()) {
			if(hill->isLeftUpToRight())
				vy = _vy;
			else {
				vy = vx / hill->GetXToYRatio();
			}
		}
	}
	else {
		if (_vy < -GIMMICK_JUMP_SPEED_Y) _vy = -GIMMICK_JUMP_SPEED_Y;
		if (_vy > GIMMICK_JUMP_SPEED_Y) _vy = GIMMICK_JUMP_SPEED_Y;
	}
	
	this->vy = _vy;*/

	//DebugOut(L"VY in Gimmick: %f\n", vy);
	if (climbingHill == Climbing) {
		if (hill == NULL)
			climbingHill == OutOfArea;
		else if (abs(vx) / abs(vy) != hill->GetXToYRatio()) {
			if (hill->isLeftUpToRight())
				vy = -_vy;
			else {
				vy = -vx / hill->GetXToYRatio();
			}
		}
	}
	else {
		if (_vy < -GIMMICK_JUMP_SPEED_Y) _vy = -GIMMICK_JUMP_SPEED_Y;
		if (_vy > GIMMICK_JUMP_SPEED_Y) _vy = GIMMICK_JUMP_SPEED_Y;
	}
	
	this->vy = _vy;
}
void CGimmick::SetX(float _x) {
	// DebugOut(L"VX in Gimmick: %f, %f\n", x, x-_x);
	if (climbingHill == Climbing && x > _x)
		x = x;
	if (abs(x - _x) > 5)
	  		y = y;
	if (x < _x)
		x = x;
	this->x = _x;
}
void CGimmick::SetY(float _y) { 
	if (abs(y-_y)>5) 
		y = y;
	this->y = _y; 
}
void CGimmick::SetDirection(bool dir) {
	this->direction = dir;
}

void CGimmick::SetState(int state)
{
	if (state == DIK_U) {
		PickItem(1);
		return;
	}
	else if (state == DIK_I) {
		PickItem(2);
		return;
	}
	else if (state == DIK_O) {
		PickItem(3);
		return;
	}
	if (this->state != GIMMICK_STATE_INJURED) {
		int prev_state = this->state;
		if (prev_state == GIMMICK_STATE_WALKING_RIGHT && state == GIMMICK_STATE_IDLE)
			state = state;
		if (state != GIMMICK_STATE_FORMING_STAR)
		{
			//if(!(this->state==GIMMICK_STATE_JUMP&& state==GIMMICK_STATE_IDLE&&(vy>0||abs(vy)>0.01f)))
			CGameObject::SetState(state);
		}
		else {
			if (whatToForm == Stars) {
				if (bomb_phase == 0 && fire_phase == 0) {

					if (star_phase == 0 && stars.empty()) {
						//forming_star_start = GetTickCount();
						star_phase = 1;
						count = 0;
						delay = 0;
						radius = 0;
						distance = STARS_START_AT;

					}
					else {
						if (star_phase == 3) {
							stars[0].SetVX(direction ? STAR_VX : -STAR_VX);
							/*stars[1].SetVX(direction ? STAR_VX : -STAR_VX);
							stars[2].SetVX(direction ? STAR_VX : -STAR_VX);*/

							/*stars[1].SetVY(STAR_VY);
							
							stars[2].SetVY(STAR_VY);*/
							stars[1].SetVY(0);
							star_1 = GetTickCount();
							star_2 = GetTickCount();

							star_phase++;
							stars_timer = GetTickCount();

						}
					}
				}
			}
			else if (whatToForm == Bomb) {
				if (star_phase == 0 && fire_phase == 0) {
					

					if (bomb_phase == NotExist) {
						bomb_phase = Forming;
						count = 0;
						delay = 0;

						radius = 0;
						distance = STARS_START_AT;
						items[0] = items[1];
						items[1] = items[2];
						items[2] = 0;
						pickedItem = 0;
					}
					else {
						if (bomb_phase == OnHead) {
							bomb = new CBomb(x, y + 16);
							this->bomb->vx = direction ? BOMB_VX : -BOMB_VX;
							this->bomb->vy = -BOMB_VY;
							bomb_phase = Thrown;
						}

					}
				}
				
			}
			else if (whatToForm == Fire) {
				if (bomb_phase == 0 && star_phase == 0) {

					

					if (fire_phase == FireNotExist) {
						fire_phase = FireForming;
						count = 0;
						delay = 0;

						radius = 0;
						distance = STARS_START_AT;
						items[0] = items[1];
						items[1] = items[2];
						items[2] = 0;
						pickedItem = 0;
					}
					else {
						if (fire_phase == FireOnHead) {
							this->fire->vx = direction ? FIRE_VX : -FIRE_VX;
							fire_phase = FireThrown;
						}

					}
				}
			}
			

		}
		if (true) {
			if (state == GIMMICK_STATE_JUMP)
				DebugOut(L"state in Gimmick: JUMP \n");
			if (state == GIMMICK_STATE_WALKING_RIGHT)
				DebugOut(L"state in Gimmick: WALKING RIGHT %d\n", GetTickCount()-jumpAt);

			switch (state)
			{
			case GIMMICK_STATE_WALKING_RIGHT:
				//vx = vx == GIMMICK_WALKING_SPEED? vx:vx+GIMMICK_FRICTION_FORCE;
				if (prev_state == GIMMICK_STATE_JUMP) {
					CGameObject::SetState(prev_state);
					if (CGame::GetInstance()->IsKeyDown(DIK_SPACE) && GetTickCount() - landedWhen < 700)
						SetVY(GIMMICK_JUMP_SPEED_Y);

					SetVX(GIMMICK_WALKING_SPEED);
					SetDirection(true);
				}
				else if (prev_state == GIMMICK_STATE_IN_PIPE) {
					CGameObject::SetState(prev_state);
					SetVX(GIMMICK_WALKING_SPEED);
					direction = true;
				}
				else {
					landedWhen = GetTickCount();

					if (standingOnElevator == 0)
						SetVX(GIMMICK_WALKING_SPEED);
					else if (standingOnElevator > 0) {
						landedWhen = GetTickCount();
						SetVX(GIMMICK_WALKING_SPEED * (2));
					}

					else {
						landedWhen = GetTickCount();
						SetVX(GIMMICK_WALKING_SPEED * (0.6));
					}
					SetDirection(true);

					if (climbingHill == Climbing) {
						if (hill != NULL) {
							if (hill->isLeftUpToRight()) {
								SetVX(GIMMICK_WALKING_SPEED * (1 - hill->GetSpeedReductionFactor()));
								SetVY(vx / hill->GetXToYRatio());
							}
							else {
								SetVX(GIMMICK_WALKING_SPEED * (1 + hill->GetSpeedReductionFactor()));
								SetVY(-vx / hill->GetXToYRatio());

							}
						}
						else {
							climbingHill = OutOfArea;
						}
					}
					/*else if (prev_state==GIMMICK_STATE_JUMP && climbingHill==Enter){
						CGameObject::SetState(GIMMICK_STATE_JUMP)
					}*/
					SetDirection(true);
					nx = 1;
				}


				//vx = GIMMICK_WALKING_SPEED;vx = 
				break;
			case GIMMICK_STATE_WALKING_LEFT:
				//vx = vx == -GIMMICK_WALKING_SPEED ? vx : vx - GIMMICK_FRICTION_FORCE;
				//vx = -GIMMICK_WALKING_SPEED;
				if (prev_state == GIMMICK_STATE_JUMP) {
					CGameObject::SetState(prev_state);
					if (CGame::GetInstance()->IsKeyDown(DIK_SPACE) && GetTickCount() - landedWhen < 700)
						SetVY(GIMMICK_JUMP_SPEED_Y);

					SetVX(-GIMMICK_WALKING_SPEED);
					SetDirection(false);
				}
				else if (prev_state == GIMMICK_STATE_IN_PIPE) {
					CGameObject::SetState(prev_state);
					SetVX(-GIMMICK_WALKING_SPEED);
					direction = false;
				}
				else {
					landedWhen = GetTickCount();
					if (standingOnElevator == 0)
						SetVX(-GIMMICK_WALKING_SPEED);
					else if (standingOnElevator < 0)
						SetVX(-GIMMICK_WALKING_SPEED * (2));
					else
						SetVX(-GIMMICK_WALKING_SPEED * (0.6));
					SetDirection(false);
					if (climbingHill == Climbing) {
						if (hill != NULL) {
							if (hill->isLeftUpToRight()) {
								SetVX(-GIMMICK_WALKING_SPEED * (1 + hill->GetSpeedReductionFactor()));
								SetVY(vx / hill->GetXToYRatio());
							}
							else {
								SetVX(-GIMMICK_WALKING_SPEED * (1 - hill->GetSpeedReductionFactor()));
								SetVY(-vx / hill->GetXToYRatio());

							}
						}
						else climbingHill = OutOfArea;
					}
					SetDirection(false);
					nx = -1;
				}

				break;
			case GIMMICK_STATE_JUMP:
				// TODO: need to check if Mario is *current* on a platform before allowing to jump again
 				if (prev_state == GIMMICK_STATE_IN_PIPE) {
					CGameObject::SetState(prev_state);
				}
				if (climbingHill != OutOfArea && GetTickCount() - landedWhen > 250) {
					x = x;
				}
				else if (GetTickCount() - landedWhen < 700 || prev_state==GIMMICK_STATE_WALKING_LEFT||prev_state==GIMMICK_ANI_WALKING_RIGHT) {
					if (CGame::GetInstance()->IsKeyDown(DIK_LEFTARROW))
						SetVX(-GIMMICK_WALKING_SPEED);
					if (CGame::GetInstance()->IsKeyDown(DIK_RIGHTARROW))
						SetVX(GIMMICK_WALKING_SPEED);

					if (!standingOnMovingStair)
 						if (climbingHill == Climbing) {
							climbingHill = Enter;
							SetY(y - GIMMICK_GRAVITY);
							SetVY(GIMMICK_JUMP_SPEED_Y);
							SetVX(0);
						}
						else if (standingOnSBB) {
							vy = GIMMICK_JUMP_SPEED_Y;
							SetStandingOnSBB(false);
						}
						else if (standingOnSB) {
							vy = GIMMICK_JUMP_SPEED_Y;
							SetStandingOnSB(false);
						}
						else if (standingOnHangingElevator) {
							vy = GIMMICK_JUMP_SPEED_Y;
							SetStandingHangingElevator(false);
						}
						else if (standingOnBBA) {
							standingOnBBA = false;
							bba->GetReleased();
						}
						else if (standingOnBoat) {
							SetStandingOnBoat(false);
							vy = GIMMICK_JUMP_SPEED_Y;
						}
						else if (standingOnBird) {
							SetStandingOnBird(false);
							vy = GIMMICK_JUMP_SPEED_Y;
						}
						else if (standingOnShell) {
							SetStandingOnShell(false);
							vy = GIMMICK_JUMP_SPEED_Y;
						}
						else if (standingOnWGM) {
							SetStandingOnWGM(false);
							vy = GIMMICK_JUMP_SPEED_Y;
						}
						else if (standingOnGFS) {
							SetStandingOnGFS(false);
							vy = GIMMICK_JUMP_SPEED_Y;

						}
						else if (standingOnStar) {
							SetStandingOnStar(false);
							vy = GIMMICK_JUMP_SPEED_Y;

						}
       						else vy = GIMMICK_JUMP_SPEED_Y;
					else {
						SetStandingMovingStair(false);
						vy = GIMMICK_JUMP_SPEED_Y;
					}
				}

				break;
			case GIMMICK_STATE_IDLE:
				if (prev_state == GIMMICK_STATE_JUMP) {
					CGameObject::SetState(prev_state);
				}
				else if (prev_state == GIMMICK_STATE_IN_PIPE) {
					CGameObject::SetState(prev_state);
					SetVX(0);
					SetVY(0);
				}
				else {
					if (standingOnElevator == 0)
						SetVX(0);
					else if (standingOnElevator > 0)
						SetVX(0.5 * GIMMICK_WALKING_SPEED);
					else SetVX(-0.5 * GIMMICK_WALKING_SPEED);

					if (climbingHill == Climbing) {
						if (hill->isLeftUpToRight()) {
							SetVX(-GIMMICK_WALKING_SPEED * (hill->GetSpeedReductionFactor()));
							SetVY(vx / hill->GetXToYRatio());
						}
						else {
							SetVX(GIMMICK_WALKING_SPEED * (hill->GetSpeedReductionFactor()));
							SetVY(-vx / hill->GetXToYRatio());

						}
					}

					else {

					}
				}

				break;
			case GIMMICK_STATE_DOWN:
				CGameObject::SetState(prev_state);
				if (prev_state == GIMMICK_STATE_IN_PIPE)
					vy = -0.01;
				break;
			case GIMMICK_STATE_UP:
				CGameObject::SetState(prev_state);
				if (prev_state == GIMMICK_STATE_IN_PIPE)
					vy = 0.01;
				break;
			case GIMMICK_STATE_DIE:
				//vy = -GIMMICK_DIE_DEFLECT_SPEED;
				break;
			}
		}
	}
	else {
		if (state == GIMMICK_STATE_WALKING_RIGHT) {
			SetDirection(true);
		}
		else if (state == GIMMICK_STATE_WALKING_LEFT) {
			SetDirection(false);
		}
	}
}
void CGimmick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top - 16 + 1;
}
CGimmick* CGimmick::GetInstance() {
	if (instance == NULL) instance = new CGimmick();
	return instance;
}
void CGimmick::Reset() {

}
void CGimmick::Load(LPCWSTR filepath) {

}
void CGimmick::GetInjured(float life, bool direction) {
	if (GetTickCount() - injuringTimer > GIMMICK_INJURED_TIME) {
		if (standingOnBoat)
			SetStandingOnBoat(false);

		if (direction)
			SetVX(GIMMICK_VX_WHEN_INJURED);
		else
			SetVX(-GIMMICK_VX_WHEN_INJURED);

		vy = GIMMICK_VY_WHEN_INJURED*2;
		injuringTimer = GetTickCount();
		faintedTimer = GetTickCount();
		state = GIMMICK_STATE_INJURED;
		if (hill != NULL && climbingHill != OutOfArea)
			climbingHill = Enter;
		IncreaseLife(life);
	}
	
}
void CGimmick::IncreaseLife(float l) {
	if (lives < 4) {
		lives=ceil(lives+l);
		if (lives> 4)lives = 4;
	}
}
bool CGimmick::AddItem(int i) {
	if (items[0] != 0 && items[1] != 0 && items[2] != 0)return false;
	if (items[0] == 0)items[0] = i;
	else if (items[1] == 0)items[1] = i;
	else if (items[2] == 0)items[2] = i;
	return true;
}
void CGimmick::PickItem(int i) {
	if (GetTickCount() - timeToPickItem > 1000) {
		if (i == 1 && pickedItem != 0)
			pickedItem = 0;
		else if (items[i - 1] != pickedItem && items[i - 1] != 0) {
			pickedItem = items[i - 1];
			swap(items[0], items[i - 1]);
		}

		if (pickedItem == GET_BOMB) whatToForm = Bomb;
		else if (pickedItem == GET_FIRE) whatToForm = Fire;
		else whatToForm = Stars;
		timeToPickItem = GetTickCount();
	}
	
}
void CGimmick::GetLocked(bool l, int f) {
	lockedInView = l;
	viewFrom = f;
	if(l)
		GetLockedIn(false);

}
void CGimmick::GetLockedIn(bool l, int f) {
	LockedToView = l;
	viewTo = f;
	if (l)
		GetLocked(false);
	else
		x = x;
}