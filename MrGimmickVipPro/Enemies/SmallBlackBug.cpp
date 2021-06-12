#include "SmallBlackBug.h"
SmallBlackBug::SmallBlackBug(int x, int y, int jar) {
	this->x = x;
	this->y = y;
	vx = 0;
	vy = 0;

	this->carryJar = jar;
	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = direction ? X_SPEED : -X_SPEED;
	this->state = WALKING_STATE;
	animation_set = CAnimationSets::GetInstance()->Get(ANIMATION_SET_ID);
}

SmallBlackBug::~SmallBlackBug(){}

void SmallBlackBug::Jump()
{
	vy -= sqrt(GRAVITY * 60);
}

void SmallBlackBug::Render() {
	

	int aniFrom = direction ? 0 : animation_set->size() / 2;
	animation_set->at(aniFrom + state)->Render(x, y);
	//RenderBoundingBox();
	/*switch (state) {
	case WALKING_STATE:
		animation_set->at(case)
	}*/
}

void SmallBlackBug::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	//calculate dx, dy
	CGameObject::Update(dt);

	CGimmick *player = CGimmick::GetInstance();

	direction = CGimmick::GetInstance()->GetX() < x ? false : true;
	vx = direction ? X_SPEED : -X_SPEED;

	// Simple fall down
	vy = vy + GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != DEATH_STATE)
		CalcPotentialCollisions(colliable_objects, coEvents);
	
	if (standingOnElevator != 0) {
		if (dy < 0 || (x + width < eFrom) || x > eTo)
			standingOnElevator = 0;
	}

	if (coEvents.size() == 0)	//no collision
	{
		x += dx;
		y += dy;
		return;
	}
	else						//there are collisions
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x = x + min_tx * dx + nx * 0.4f;
		y = y + min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++) 
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<StaticObject*>(e->obj)) //collide with static objects
			{
				if (e->nx != 0) //collide to left or right
				{
					if (dynamic_cast<Elevator*>(e->obj)) //elevator
					{
						Elevator* el = dynamic_cast<Elevator*>(e->obj);
						if (this->x < player->GetX()) //at the left of gimmick
						{
							el->GetDirection() ? vx = X_SPEED * (-0.6) : vx = X_SPEED * (-2);
							x = x + vx*dt;
							state = WALKING_STATE;
							direction = 1;
						}
						else if (this->x > player->GetX()) //at the right of gimmick
							{
								el->GetDirection() ? vx = X_SPEED * (2) : vx = X_SPEED * (0.6);
								x = x + vx * dt;
								state = WALKING_STATE;
								direction = 0;
							}
							else //touched gimmick
							{
								el->GetDirection() ? vx = X_SPEED * 0.5 : vx = X_SPEED * (-0.5);
								x = x + vx * dt;
							}

						this->standingOnElevator = el->GetDirection() ? 1 : -1;
						eFrom = el->x;
						eTo = el->x + el->width;
						continue;
					}
					else //other static objects
					{
						if (vy == 0)
							Jump();
						else
							vx = 0;
						continue;
					}

				}
				else if (e->ny < 0) //standing on ground
					{
						if (dynamic_cast<Elevator*>(e->obj)) 
						{
							
							SetStandingOnElevator(dynamic_cast<Elevator*>(e->obj)->GetDirection() ? 1 : -1,
								dynamic_cast<Elevator*>(e->obj)->x,
								dynamic_cast<Elevator*>(e->obj)->x + dynamic_cast<Elevator*>(e->obj)->width);
						}
						vy = 0;
					}
					else //collide at the bottom
					{
						// Simple fall down
						vy = vy + GRAVITY * dt;
					}
			}
		}
	}

}

void SmallBlackBug::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + WIDTH;
	bottom = y + HEIGHT;
}
