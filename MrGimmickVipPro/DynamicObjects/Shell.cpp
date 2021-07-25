#include "Shell.h"
#include "../Gimmick.h"
#include "../StaticObjects/Ground.h"
OnBugHeadShell::OnBugHeadShell( BombCaryingBug* b)
{
	this->width =  SHELL_WIDTH;
	this->height = SHELL_HEIGHT;
	bug = b;
	//bug->GetShell(this);
	this->state = OBHS_INACTIVE_STATE;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(SHELL_ANIMATIO_SET_ID));

}
void OnBugHeadShell::Explode() {
	state = OBHS_EXPLODED_STATE;
	explodingTime = GetTickCount();
}
OnBugHeadShell::~OnBugHeadShell()
{
}
void OnBugHeadShell::Render()
{
	if(state!=OBHS_EXPLODED_STATE)
		animation_set->at(0)->Render(x, y-2);
	else if(GetTickCount()-explodingTime<EXPLODING_TIME)
		animation_set->at(2)->Render(x, y);

	RenderBoundingBox();
}
void OnBugHeadShell::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == OBHS_EXPLODED_STATE) {
		l = t = r = b = 0;
	}
	else {
		l = x;
		t = y;
		r = x + width;
		b = y - height;
	}

}
void OnBugHeadShell::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == OBHS_INACTIVE_STATE) {
		x = bug->x;
		y = bug->y + height;
		if (bug->IsDeath()) GetReleased();

	}
	else if(state!= OBHS_EXPLODED_STATE){
		CGameObject::Update(dt);
		vy -= dt * 0.00005;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		colliable_objects->push_back(CGimmick::GetInstance());
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
				if (dynamic_cast<Ground*>(e->obj)) {
					if (e->ny != 0)vy = 0;
					else vx = -vx;
				}
				
				
			}
		}
		if (state == OBHS_RELEASED_STATE && GetTickCount() - existingTime > EXISTING_TIME) {
			vx = vy = 0;
			state = OBHS_EXPLODED_STATE;
			explodingTime = GetTickCount();
		}
	}
}
void OnBugHeadShell::GetReleased() {
	vx = CGimmick::GetInstance()->x > x ? OBHS_X_SPEED : -OBHS_X_SPEED;
	state = OBHS_RELEASED_STATE;
	existingTime = GetTickCount();
}

Shell::Shell(int x, int y, int t, bool dir)
{
	this->x = x;
	this->y = y;
	this->type = t;
	this->direction = dir;
	this->width =  SHELL_WIDTH;
	this->height = SHELL_HEIGHT;
	this->state = 0;
	this->markedX = x;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(SHELL_ANIMATIO_SET_ID));
	existingTimer = GetTickCount();
	if (type == rolling) {
		vx = ROLLING_X_SPEED;
		
	}
}

Shell::~Shell()
{
}
void Shell::Render()
{
	if (state != SHELL_EXPLODING_STATE) {
		animation_set->at(state)->Render(x, y + 2);

	}
	else {
		if (GetTickCount() - explodingTimer < EXPLODING_TIME) {
			animation_set->at(state)->Render(x, y + 2);
		}
	}

	RenderBoundingBox();
}

void Shell::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != SHELL_EXPLODING_STATE) {
		l = x;
		t = y;
		r = x + width;
		b = y - height;
	}
	else {
		l = t = r = b;
	}
	
}
void Shell::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state != SHELL_EXPLODING_STATE) {


		if (onHill == OutOfArea) {
			if (vy > -ROLLING_Y_SPEED)
				vy -= SHELL_GRAVITY * dt;
		}
		CGameObject::Update(dt);


		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		colliable_objects->push_back(CGimmick::GetInstance());
		CalcPotentialCollisions(colliable_objects, coEvents);

		if (coEvents.size() == 0) {
			if (onHill == OutOfArea) {
				x += dx;
				y += dy;
			}
			if (onHill == Enter) {
				if (hill->isLeftUpToRight()) {
					if ((x + width - 4 - hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
						x = (abs(-hill->height + hill->y + height - y) * hill->GetXToYRatio() + hill->x + 4 - width);
						onHill = Touching;
						SetVX(ROLLING_X_SPEED * (1 - hill->GetSpeedReductionFactor()));
						SetVY(-vx / hill->GetXToYRatio());

					}
					else if (vy > 0.01 && (x + width - hill->x + 2) / abs(-hill->height + hill->y - y + height) < hill->GetXToYRatio()) {
						y = ((-(x + width - hill->x + 2) / hill->GetXToYRatio()) + hill->y - hill->height + height);
						onHill = Touching;
						SetVX(ROLLING_X_SPEED);
						SetVY(vx / hill->GetXToYRatio());
					}
					else if (vx > 0.01 && (x + width - hill->x - 5) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
						y = (-((x + width - hill->x - 5) / hill->GetXToYRatio()) + hill->y - hill->height + height);
						onHill = Touching;
						SetVX(ROLLING_X_SPEED);
						SetVY(vx / hill->GetXToYRatio());
					}
					else {
						x += dx;
						y += dy;
					}
				}
				else {
					if ((-x - 4 + hill->width + hill->x) / abs(-hill->height + hill->y - y + height) > hill->GetXToYRatio()) {
						y = (abs(-hill->height + hill->y + height + (-x + hill->width - 4 + hill->x) / hill->GetXToYRatio()));
						onHill = Touching;
						SetVX(ROLLING_X_SPEED * (1 + hill->GetSpeedReductionFactor()));
						SetVY(-vx / hill->GetXToYRatio());

					}
					else {
						x += dx;
						y += dy;
					}
				}
				if (x > hill->width + hill->x || (!hill->isLeftUpToRight() && x + 10 > hill->width + hill->x) || x + height < hill->x || (vy > 0 && y - height > hill->y) || (vy < 0 && y < hill->y - hill->height)) {
					onHill = OutOfArea;
					if (hill->isLeftUpToRight()) {
						SetVX(ROLLING_X_SPEED);
						SetVY(0);
						y = hill->y + height + 1;
					}
					else {
						SetVX(ROLLING_X_SPEED);
						y = -hill->y + hill->height + height + 1;
						SetVY(0);
					}
					/*SetX(x + dx);
					SetY(y - dt * vy);*/
					//SetVY(GIMMICK_GRAVITY * dt);
				}
			}
			else if (onHill == Touching) {
				//if (x + 4> hill->width + hill->x || x + width < hill->x || (vy < 0 && y + height < hill->y) || (vy > 0 && y > hill->y + hill->height)) {
				//	
				//	onHill = Enter;

				//	x = (x + dx);
				//	//SetY(y + dy);
				//}
				if (hill->isLeftUpToRight() && vx < 0 && x + width - 4 < hill->x) {
					onHill = OutOfArea;
					vx = -ROLLING_X_SPEED;
					vy = 0;
					y += 1;
				}
				else {
					if (hill->isLeftUpToRight()) {
						if (vx > 0) {
							SetVX(vx - 0.000005 * dt);
							SetVY(vx / hill->GetXToYRatio());

						}
						else {
							if (vx > -ROLLING_X_SPEED) {
								SetVX(vx - 0.000005 * dt);
								SetVY(vx / hill->GetXToYRatio());

							}
						}
					}
					x = (x + dx);
					y = (y + dy);
				}
			}
		}
		else {
			//if (onHill == Enter) {
			//	if (hill->isLeftUpToRight()) {
			//		if (vy <= 0.003 && (x + width - 4 - hill->x) / (hill->height + hill->y + 4 - y - height) > hill->GetXToYRatio()) {
			//			x = ((hill->height + hill->y + 4 - height - y) * hill->GetXToYRatio() + hill->x + 4 - width);
			//			onHill = Touching;
			//			vx = ROLLING_X_SPEED * (1 - hill->GetSpeedReductionFactor());
			//			vy = -vx / hill->GetXToYRatio();

			//		}
			//		else if (vy > 0.003 && (x + width - 4 - hill->x) / (hill->height + hill->y + 4 - y - height) < hill->GetXToYRatio()) {
			//			y = (((x + width - 4 - hill->x) / hill->GetXToYRatio()) + hill->y + hill->height + 4 - height);
			//			onHill = Touching;
			//			vx = ROLLING_X_SPEED * hill->GetSpeedReductionFactor();
			//			vy = -vx / hill->GetXToYRatio();
			//		}
			//		else {
			//			x += dx;
			//			y += dy;
			//		}
			//	}
			//	else {
			//		/*if ((-x - 4 + hill->width + hill->x) / (hill->height + hill->y - y - height) > hill->GetXToYRatio()) {
			//			y = ((hill->height + hill->y - height - (-x + hill->width - 4 + hill->x) / hill->GetXToYRatio()));
			//			onHill = Touching;
			//			vx = ROLLING_X_SPEED * hill->GetSpeedReductionFactor();
			//			vy = vx / hill->GetXToYRatio();

			//		}
			//		else {
			//			x += dx;
			//			y += dy;
			//		}*/
			//		if (vy <= 0.003 && (- x - 4 +hill->width+ hill->x) / (hill->height + hill->y + 4 - y - height) > hill->GetXToYRatio()) {
			//			x = ((hill->height + hill->y + 4 - height - y) * hill->GetXToYRatio() + hill->x + 4 - width);
			//			onHill = Touching;
			//			vx = -ROLLING_X_SPEED * (1 - hill->GetSpeedReductionFactor());
			//			vy = vx / hill->GetXToYRatio();

			//		}
			//		else if (vy > 0.003 && (-x - 4 +hill->width+ hill->x) / (hill->height + hill->y + 4 - y - height) < hill->GetXToYRatio()) {
			//			y = (((x + width - 4 - hill->x) / hill->GetXToYRatio()) + hill->y + hill->height + 4 - height);
			//			onHill = Touching;
			//			vx = ROLLING_X_SPEED * hill->GetSpeedReductionFactor();
			//			vy = -vx / hill->GetXToYRatio();
			//		}
			//		else {
			//			x += dx;
			//			y += dy;
			//		}
			//	}
			//	if (x > hill->width + hill->x || x + height < hill->x || (vy < 0 && y + height < hill->y) || (vy > 0 && y > hill->y + hill->height)) {
			//		onHill = OutOfArea;
			//		if (hill->isLeftUpToRight()) {
			//			vx = -ROLLING_X_SPEED;
			//			vy = 0;
			//			y += dy;
			//		}
			//		else {
			//			vx = ROLLING_X_SPEED;
			//			y += dy;
			//			vx = 0;
			//		}
			//		/*SetX(x + dx);
			//		SetY(y - dt * vy);*/
			//		//SetVY(GIMMICK_GRAVITY * dt);
			//	}
			//}
			//else if (onHill == Touching) {
			//	if (x > hill->width + hill->x || x + width < hill->x || (vy < 0 && y + height < hill->y) || (vy > 0 && y > hill->y + hill->height)) {
			//		if (x + width < 400)
			//			x = x;
			//		DebugOut(L"x %f\n", x);

			//		onHill = Enter;

			//		x = (x + dx);
			//		//SetY(y + dy);
			//	}
			//	else {
			//		DebugOut(L"x %f\n", x + width - hill->x);
			//		if (x + width - hill->x < 0.5)
			//			x = x;
			//		x = (x + dx);
			//		y = (y + dy);
			//	}
			//}
			float min_tx, min_ty, nx, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			bool collideWithGroundOnY = false, collideWithHillOnY = false;
			for (UINT i = 0; i < coEvents.size(); i++) {
				if (coEvents[i]->ny > 0) {
					if (dynamic_cast<Ground*>(coEvents[i]->obj))
						collideWithGroundOnY = true;
					if (dynamic_cast<Hill*>(coEvents[i]->obj))
						collideWithHillOnY = true;
				}
			}
			if (onHill != OutOfArea && collideWithGroundOnY && collideWithHillOnY) {
				for (UINT i = 0; i < coEvents.size(); i++) {
					if (coEvents[i]->ny > 0 && !dynamic_cast<Ground*>(coEvents[i]->obj))
						coEvents.erase(coEvents.begin() + i);
				}
			}

			x = x + min_tx * dx + nx * 0.1f;
			y = y + min_ty * dy - ny * 0.1f;
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Hill*>(e->obj)) {
					if (e->ny != 0) {
						onHill = Enter;
						hill = dynamic_cast<Hill*>(e->obj);
						y = y - (min_ty * dy - ny * 0.1f) + dy;

					}
					if (e->nx != 0) {
						onHill = Enter;
						hill = dynamic_cast<Hill*>(e->obj);
						x = x - (min_tx * dx + nx * 0.1f) + dx;
					}
				}
				else if (dynamic_cast<Ground*>(e->obj)) {
					if (e->nx != 0) {
						if (onHill != OutOfArea) {
							x = x - (min_tx * dx + nx * 0.1f) + dx;

						}
						else {
							SetVX(-vx);
						}
					}
					if (e->ny != 0) {
						if (onHill != OutOfArea) {
							if (onHill == Touching) {
								vy = 0;
								y = e->obj->y + height + 0.5;
							}
							else {
								y = y - (min_ty * dy - ny * 0.1f) + dy;

							}
						}
						else {
							/*vx = -vx;*/
							SetVY(0);
						}
					}
				}
				else if (dynamic_cast<CGimmick*>(e->obj)) {
					vx = vy = 0;
					state = SHELL_EXPLODING_STATE;
					explodingTimer = GetTickCount();
					dynamic_cast<CGimmick*>(e->obj)->GetInjured(INJURED_BY_OTHER, e->nx < 0);
				
				}
				else {
					y = y - (min_ty * dy - ny * 0.1f) + dy;
					x = x - min_tx * dx - nx * 0.1f + dx;

				}
			}
		}
		if (onHill != Touching) {
			if (abs(x - markedX) > SHELL_X_RANGE) {
				markedX = x;
				state = state == 0 ? 1 : 0;

			}
		}
		else {
			if (abs(x - markedX) > SHELL_X_RANGE / 2) {
				markedX = x;
				state = state == 0 ? 1 : 0;

			}

		}
		if (GetTickCount() - existingTimer > SHELL_EXISTING_TIME) {
			state = SHELL_EXPLODING_STATE;
			explodingTimer = GetTickCount();
		}
	}
}
void Shell::SetVX(float vx) {
	this->vx = vx;
}
void Shell::SetVY(float vy) {
	this->vy = vy;
}
void Shell::Explode() {
	state = SHELL_EXPLODING_STATE;
	explodingTimer = GetTickCount();
}