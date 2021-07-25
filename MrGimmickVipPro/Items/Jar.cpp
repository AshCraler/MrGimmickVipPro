#include "Jar.h"
#include "../StaticObjects/Elevator.h"
#include "../StaticObjects/Ground.h"
#include "../Enemies/Utility/JumpingCommandBox.h"

Jar::~Jar()
{
}

void Jar::Render()
{
	if (!taken && !hidden) {
		CSprites::GetInstance()->Get(JAR_SPRITE_ID_STARTS_AT + type)->Draw(x, y);
		RenderBoundingBox();
	}
}
void Jar::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (!hidden && !taken)
	{
		CGameObject::Update(dt);

		if (type == 2)
			type = type;
		if (!collidedWithGround)
			vy -= dt * 0.00001;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(colliable_objects, coEvents);

		if (x < 400)
			x = x;
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
				if (e->nx != 0) {
					if (dynamic_cast<Elevator*>(e->obj)) {
						x = x + dx - (min_tx * dx + nx * 0.1f);
						vx = dynamic_cast<Elevator*>(e->obj)->GetDirection() ? JAR_VX : -JAR_VX;
					}
					else {
						vx = 0;
					}

				}
				if (e->ny != 0) {
					if (dynamic_cast<Elevator*>(e->obj)) {
						y = y + (min_ty * dy - ny * 0.1f) - 3;
						vx = dynamic_cast<Elevator*>(e->obj)->GetDirection() ? JAR_VX : -JAR_VX;
						if (e->ny < 0) {
							elevator = dynamic_cast<Elevator*>(e->obj);
							vy = 0;
						}

						else {
							vy = -vy;
						}
					}
					if (dynamic_cast<Ground*>(e->obj)) {
						vx = 0;
						collidedWithGround = true;
						if (e->ny < 0)	vy = 0;
						else {
							vy = -vy;
						}
					}

				}
				if (dynamic_cast<JumpingCommandBox*>(e->obj)) {
					x = x + dx - (min_tx * dx + nx * 0.1f);
					y = y + dy - (min_ty * dy - ny * 0.1f);
				}
			}
		}
		vx = 0;
	}
	if (elevator != NULL) {
		if (x + width > elevator->x && x < elevator->x + elevator->width && vx == 0) {
			vx = elevator->GetDirection() ? JAR_VX : -JAR_VX;

		}
		else if(!(x + width > elevator->x && x < elevator->x + elevator->width)){
			vx = 0;
		}
	}
	else {
		//vx = 0;
	}
}
void Jar::Appear(float x, float y, bool dir) {
	hidden = false;
	this->x = x;
	this->y = y;
	vx = dir ? JAR_VX : -JAR_VX;
	vy = JAR_VY;
	collidedWithGround = false;
}
Jar::Jar(int x, int y, int type, bool hidden)
{
	if (!hidden) collidedWithGround = true;
	this->hidden = hidden;
	this->x = x;
	this->y = y;
	this->type = type;
	if (type == PINK_LIQUID) height = JAR_HEIGHT - 2;
	else if (type == ORANGE_LIQUID) height = JAR_HEIGHT;
	else height = JAR_HEIGHT - 1;
	width = JAR_WIDTH;
}
void Jar::GetPushed(float dx) {
	x += dx;
}
void Jar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!taken && !hidden) {
		l = x;
		t = y;
		r = x + width;
		b = y - height;
	}
	else {
		l = t = b = r = 0;
	}

}