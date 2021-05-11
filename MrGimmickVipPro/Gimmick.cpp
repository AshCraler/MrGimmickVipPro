#include "Gimmick.h"
#include "Ground.h"
#include "Utils.h"

CGimmick* CGimmick::instance = NULL;

CGimmick::CGimmick(float x, float y)
{
	
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	vx = vy = 0;
}
void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(colliable_objects, coEvents);

	// reset untouchable timer if untouchable time has passed
	

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
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
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


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
	_checkSweptAABB(colliable_objects);


	instance = this;
	DebugOut(L"location in Gimmick: %f, %f\n", x, y);

}
void CGimmick::_checkSweptAABB(vector<LPGAMEOBJECT>* coObjects) {
	DWORD now = GetTickCount();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		bool isColisionGround = false;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj)) {

			}
		}
		if (!isColisionGround) {
			x += dx;
			y += dy;
		}
		else {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGimmick::Render()
{
	int ani = -1;
	if (state == GIMMICK_STATE_DIE)
		ani = GIMMICK_ANI_DIE;
	else
		if (vx == 0)
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
	if (x > 100) rx = 100; 
	else rx = x;
	DebugOut(L"location in Cammera: %f, %f\n", rx, y); 00;
	CAnimationSets::GetInstance()->Get(1)->at(ani)->Render(rx, y, alpha);
	DebugOut(L"location in Render: %f, %f\n", x, y);
	//animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -GIMMICK_JUMP_SPEED_Y;
		break;
	case GIMMICK_STATE_IDLE:
		vx = 0;
		break;
	case GIMMICK_STATE_DIE:
		vy = -GIMMICK_DIE_DEFLECT_SPEED;
		break;
	}
}


void CGimmick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{

}
CGimmick* CGimmick::GetInstance() {
	if (instance == NULL) instance = new CGimmick();
	return instance;
}
void CGimmick::Reset() {

}
void CGimmick::Load(LPCWSTR filepath) {

}