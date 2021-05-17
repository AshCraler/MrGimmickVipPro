#include "Gimmick.h"
#include "./StaticObjects/Ground.h"
#include "Utils.h"
#include "Items/Item.h"
#include "StaticObjects/Elevator.h"
#include "StaticObjects/MStair.h"

CGimmick* CGimmick::instance = NULL;

CGimmick::CGimmick(float x, float y)
{
	
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	width = 16;
	vx = vy = 0;
}
void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	

	// Simple fall down
	//vy += GIMMICK_GRAVITY*dt;
	SetVY(vy + GIMMICK_GRAVITY * dt);
	/*if (y > 365)
		y = y;*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(colliable_objects, coEvents);

	// reset untouchable timer if untouchable time has passed
	
	if (standingOnElevator!=0) {
		if (dy < 0 || (x + width < eFrom) || x > eTo)
			SetStandingOnElevator(0);
		else SetStandingOnElevator(standingOnElevator, eFrom, eTo);
	} 
	/*if (standingOnMovingStair != 0) {
		if ((x + width < sFrom) || x > sTo)
			SetStandingMovingStair(false);
		else SetStandingMovingStair(standingOnMovingStair, eFrom, eTo);
	}*/
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (standingOnMovingStair) {
			SetX(stair->x + deltaXToMStair+ dx);
			SetY(stair->y + deltaYToMStair);
		}
		else {
			SetX(x + dx);
			SetY(y + dy);
		}
		
		// SetVY(vy + GIMMICK_GRAVITY * dt);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		/*x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;*/
		if (!standingOnMovingStair) {
			SetX(x + min_tx * dx + nx * 0.4f);
			SetY(y + min_ty * dy + ny * 0.4f);
		}
		

		/*if (nx != 0) SetVX(0);
		if (ny != 0) SetVY(0);*/
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Item*>(e->obj)) {
				if (e->nx != 0) {
					dynamic_cast<Item*>(e->obj)->SetTaken(true);
					continue;
				}
				if (e->ny < 0) {
					if (CGame::GetInstance()->IsKeyDown(208)) {
						dynamic_cast<Item*>(e->obj)->SetTaken(true);
						SetVY(GIMMICK_GRAVITY);
						continue;
					}
					else SetVY(0);
				}

			}
			else if (dynamic_cast<StaticObject*>(e->obj)) {
				if (e->nx != 0) {
					if (dynamic_cast<Elevator*>(e->obj)) {
 						Elevator* el = dynamic_cast<Elevator*>(e->obj);
						if (CGame::GetInstance()->IsKeyDown(203)) {
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
						}
						SetStandingOnElevator(el->GetDirection()?1:-1, el->x, el->x + el->width);
						continue;
					}
					else {
						SetVX(0);
						continue;
					}
					
				}
 				else if (e->ny < 0 ) {
					if (dynamic_cast<Elevator*>(e->obj)) {
						// SetVY(vy + GIMMICK_GRAVITY * dt);
						SetY(y + vy * dt);
						SetStandingOnElevator(dynamic_cast<Elevator*>(e->obj)->GetDirection() ? 1 : -1, 
							dynamic_cast<Elevator*>(e->obj)->x, 
							dynamic_cast<Elevator*>(e->obj)->x + dynamic_cast<Elevator*>(e->obj)->width);
					}
					else if (dynamic_cast<MStair*>(e->obj)) {
						// SetVY(0);
						if (standingOnMovingStair) {
							SetX(dynamic_cast<MStair*>(e->obj)->x + deltaXToMStair);
							SetY(dynamic_cast<MStair*>(e->obj)->y + deltaYToMStair);
						}else SetStandingMovingStair(true,
							ceil(dynamic_cast<MStair*>(e->obj)->x),
							ceil(dynamic_cast<MStair*>(e->obj)->x) + dynamic_cast<MStair*>(e->obj)->width,
							dynamic_cast<MStair*>(e->obj)->y,
							dynamic_cast<MStair*>(e->obj)) ;
					}
					else SetVY(GIMMICK_GRAVITY);
				}
				else {
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

	if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	// clean up collision events
	// for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	// _checkSweptAABB(colliable_objects);

	// SetStandingOnElevator(collidingWithElevator);

	instance = this;
	// DebugOut(L"VX, dx in Gimmick: %f, %f\n", vx,dx);

}


void CGimmick::Render()
{
	int ani = -1;
	if (state == GIMMICK_STATE_DIE)
		ani = GIMMICK_ANI_DIE;
	else
		if (vx == 0 || abs(vx)<0.55*GIMMICK_WALKING_SPEED)
		{
			if (nx>0) ani = GIMMICK_ANI_IDLE_RIGHT;
			else ani = GIMMICK_ANI_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = GIMMICK_ANI_WALKING_RIGHT;
		else ani = GIMMICK_ANI_WALKING_LEFT;

	int alpha = 255;
	if (untouchable) alpha = 128;
	float rx, ry;
	/*if (x > 100) rx = 100; 
	else rx = x;*/
	//if (y > )
//	DebugOut(L"location in Render: %f, %f\n", rx, y); 00;
	CAnimationSets::GetInstance()->Get(1)->at(ani)->Render(x, y, alpha);
	// DebugOut(L"location in Render: %f, %f\n", x, y);
	//animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		//vx = vx == GIMMICK_WALKING_SPEED? vx:vx+GIMMICK_FRICTION_FORCE;
		if (standingOnElevator==0) {
			SetVX(GIMMICK_WALKING_SPEED);

		}
		else if (standingOnElevator > -1) SetVX(GIMMICK_WALKING_SPEED * (2));
		else SetVX(GIMMICK_WALKING_SPEED * (0.6));
		nx = 1;

		//vx = GIMMICK_WALKING_SPEED;vx = 
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		//vx = vx == -GIMMICK_WALKING_SPEED ? vx : vx - GIMMICK_FRICTION_FORCE;
		//vx = -GIMMICK_WALKING_SPEED;
		if (standingOnElevator==0) {
			SetVX(-GIMMICK_WALKING_SPEED);
			
		}
		else if (standingOnElevator < -1) SetVX(-GIMMICK_WALKING_SPEED * (2));
		else SetVX(-GIMMICK_WALKING_SPEED * (0.6));
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if(!standingOnMovingStair)	vy = -GIMMICK_JUMP_SPEED_Y;
		else {
			SetStandingMovingStair(false);
			vy = -GIMMICK_JUMP_SPEED_Y;
		}
		break;
	case GIMMICK_STATE_IDLE:
		if (standingOnElevator==0) SetVX(0);
		else if(standingOnElevator>0){
			SetVX(0.5 * GIMMICK_WALKING_SPEED);
		}
		else {
			SetVX(-0.5 * GIMMICK_WALKING_SPEED);

		}
		break;
	case GIMMICK_STATE_DIE:
		vy = -GIMMICK_DIE_DEFLECT_SPEED;
		break;
	}
}
void CGimmick::SetVX(float _vx) {
	
	this->vx = _vx;
	//DebugOut(L"VX in Gimmick: %f\n", vx);

}
void CGimmick::SetVY(float _vy) { 
	this->vy = _vy; 
	//DebugOut(L"VY in Gimmick: %f\n", vy);
}
void CGimmick::SetX(float _x) {	
	DebugOut(L"VX in Gimmick: %f, %f\n", x, x-_x);

	this->x = _x;
}
void CGimmick::SetY(float _y) { this->y = _y; }


void CGimmick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top + 16 - 1;
}
CGimmick* CGimmick::GetInstance() {
	if (instance == NULL) instance = new CGimmick();
	return instance;
}
void CGimmick::Reset() {

}
void CGimmick::Load(LPCWSTR filepath) {

}