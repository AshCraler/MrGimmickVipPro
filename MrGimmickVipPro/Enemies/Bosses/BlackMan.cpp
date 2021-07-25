#include "BlackMan.h"
#include "../../StaticObjects/Ground.h"
#include "../../Gimmick.h"

BFire::BFire(int x, int y, float vx, float vy) {
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;

	width =	 FIRE_WIDTH;
	height = FIRE_HEIGHT;
	
	state = FIRE_FALLING;
	explodingTimer = GetTickCount();
	animation_set = CAnimationSets::GetInstance()->Get(FIRE_ANMATION_SET_ID);
}
void BFire::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == FIRE_EXPLODING) {
		left = top = bottom = right = 0;
	}
	else {
		left = x;
		top = y;
		right = left + width;
		bottom = top - height;
	}
	
}
void BFire::Render() {
	if (!(state == FIRE_EXPLODING && GetTickCount() - explodingTimer > EXPLODING_TIME)) {
		RenderBoundingBox();

		animation_set->at(state)->Render(x, y);
	}

}
void BFire::SetDestroyed() {
	vy = 0;
	vx = 0;
	state = FIRE_EXPLODING;
}
void BFire::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects){
	CGameObject::Update(dt);

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
					vx = 0;
					state = FIRE_EXPLODING;
					y += 11;
					explodingTimer = GetTickCount();

				}
			}
			else {
				x = x - min_tx * dx - nx * 0.1f+dx;
				y = y - min_ty * dy + ny * 0.1f+dy;
			}
		}
	}

}
BFire::~BFire() {}

Spark::Spark(int x, int y, bool tr, int ani) {
	direction = tr;
	this->x = x;
	this->y = y;
	width = SPARK_WIDTH;
	height = SPARK_HEIGHT;
	if (direction) {
		vx = SPARK_VX;
	}
	else {
		vx = -SPARK_VX;
	}
	animation_set = CAnimationSets::GetInstance()->Get(SPARK_ANMATION_SET_ID);
	this->ani = ani;
}
void Spark::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void Spark::Render() {
	if (direction) {
		if(x> BM_LEFT_X+BM_WIDTH)
			animation_set->at(ani+4)->Render(x, y);
	}
	else {
		if(x<BM_RIGHT_X)
			animation_set->at(ani)->Render(x, y);

	}
	RenderBoundingBox();

}
void Spark::Update(DWORD dt) {
	CGameObject::Update(dt);
	x += dx;
	y += dy;

}
Spark::~Spark() {}

BlackMan::BlackMan(int x, int y) {
	this->x = BM_RIGHT_X;
	this->y = y;
	this->state = BM_INACTIVE_STATE;
	width = BM_WIDTH;
	height = BM_HEIGHT;
	vx = vy = 0;
	form = blackman;
	animation_set = CAnimationSets::GetInstance()->Get(BM_ANIMATION_SET_ID);
	//remainingHit = BS_SURVIVING_TIMES;
	bm_RemainingHits = BM_SURVIVING_TIMES;

}

BlackMan::~BlackMan() {}

void BlackMan::Render() {
	if (state != BM_INACTIVE_STATE) {
		if (form == blackman) {
			if (state != BM_APPEARING_STATE) {
				if (!(state == BM_ATTACKING_STATE && GetTickCount() - injuringTimer < BM_INJURING_TIME)) {
					int aniFrom = x == BM_RIGHT_X ? 0 : animation_set->size() / 2;
					animation_set->at(state + aniFrom)->Render(x, y);
					if (sparks.size() != 0) {
						for (int i = 0; i < sparks.size(); i++) {
							sparks.at(i)->Render();
						}
					}

					if (state == BM_ATTACKING_STATE && GetTickCount() - aboutToAttackTimer < ABOUT_TO_ATTACK_TIME) {
						CAnimationSets::GetInstance()->Get(SPARK_ANMATION_SET_ID)
							->at(x == BM_LEFT_X ? 4 : 0)->Render(x == BM_RIGHT_X ? BM_RIGHT_X - SPARK_WIDTH : BM_LEFT_X + width, y - 16);
						if(sparks.size()!=0)
							sparks[0]->Render();
					}
				}
				else {
					int aniFrom = x == BM_RIGHT_X ? 0 : animation_set->size() / 2;
					animation_set->at(4 + aniFrom)->Render(x, y);
				}

			}
			else {
				int aniFrom = x == BM_RIGHT_X ? 0 : animation_set->size() / 2;
				animation_set->at(BM_DISAPPEARING_STATE + aniFrom)->Render(x, y);
			}
		}
		else if (form == falling_sparks) {
			if (GetTickCount() - dyingTimer<3000) {
				animation_set->at(WARIOR_GETTING_INJURED)->Render(x, y);
			}
			else if (GetTickCount() - spreadingSparkTimer<2000 && spreadingSparkTimer!=0) {
				for (int i = 0; i < 10; i++) {
					animation_set->at(15)->Render(x+distance*cos(D3DXToRadian(i*(360/10))), y + distance * sin(D3DXToRadian(i * (360 / 10))));

				}
			}
			else {
				for (int i = 0; i < 100; i++) {
					animation_set->at(16)->Render(Xs[i] + x, Ys[i]+y);

				}
			}
		}
		else {
			if (sparks.size() != 0) {
				for (int i = 0; i < WARIOR_NUMBER_OF_SPARK; i++) {
					sparks.at(i)->Render();
				}
			}
			if (fires.size() != 0) {
				for (int i = 0; i < NUMBER_OF_FIRES; i++) {
					fires.at(i)->Render();
				}
			}
			if(state!= WARIOR_GETTING_THROWN)
				animation_set->at(state)->Render(x, y);
			else {
				if (abs(vy) < 0.02) {
					animation_set->at(13)->Render(x, y);
				}
				else if (vy > 0) {
					animation_set->at(9)->Render(x, y);

				}
				else {
					animation_set->at(14)->Render(x, y);

				}
			}
			if (state != WARIOR_RELEASING_FIRE && state!= WARIOR_STANDING)
				x = x;
			if (state == WARIOR_ATTACKING)
				x = x;
			if (state == WARIOR_STANDING)
				x = x;
			if (state == WARIOR_GETTING_INJURED)
				x = x;
			if (state == WARIOR_GETTING_THROWN)
				x = x;
			if (state == WARIOR_JUMPING_STATE)
				x = x;
			if (state == WARIOR_RELEASING_FIRE)
				x = x;
			if (state == WARIOR_RELEASING_SPARK)
				x = x;
			if (state == WARIOR_ATTACKING)
				x = x;
		}
		

		/*switch (state) {
		case WALKING_STATE:
			animation_set->at(case)
		}*/
		RenderBoundingBox();
	}

}

void BlackMan::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {
	if (state == BM_INACTIVE_STATE && x - CGimmick::GetInstance()->x < BM_MAX_DISTANCE_TO_GM) {
		state = BM_ATTACKING_STATE;
		existingTimer = GetTickCount();
		aboutToAttackTimer = GetTickCount();
		releasedSparks = false;
		if (sparks.size() != 1) {
			sparks.clear();
			sparks.push_back(new Spark(x == BM_RIGHT_X ?
				BM_RIGHT_X - SPARK_WIDTH : BM_LEFT_X + width,
				y - 76, x == BM_LEFT_X, 0));
			sparks[0]->vx = 0;

		}
		//waitToAttack = GetTickCount();
	}
	else{
		if (form == blackman) {
			CGameObject::Update(dt);
			vy -= 0.00001 * dt;

			if (state==BM_ATTACKING_STATE&& GetTickCount() - existingTimer > BM_EXISTING_TIME) {
				state = BM_DISAPPEARING_STATE;
				disappearingTimer = GetTickCount();
			}
			else if (state==BM_DISAPPEARING_STATE && GetTickCount()-disappearingTimer>DISAPPEARING_TIME) {
				state = BM_INVISIBLE_STATE;
				invisibleTimer = GetTickCount();
			}
			else if (state == BM_INVISIBLE_STATE && GetTickCount() - invisibleTimer > INVISBLE_TIME) {
				state = BM_APPEARING_STATE;
				appearingTimer = GetTickCount();
				if (bm_RemainingHits == 1) {
					x = BM_RIGHT_X;
				}else 
					x = x == BM_RIGHT_X ? BM_LEFT_X : BM_RIGHT_X;

			}
			else if (state == BM_APPEARING_STATE && GetTickCount() - appearingTimer > APPEARING_TIME) {
				state = BM_ATTACKING_STATE;

				existingTimer = GetTickCount();
				aboutToAttackTimer = GetTickCount();
				releasedSparks = false;

				if (sparks.size() != 1) {
					sparks.clear();
					sparks.push_back(new Spark(x == BM_RIGHT_X ?
						BM_RIGHT_X - SPARK_WIDTH: BM_LEFT_X + width,
						y - 16, x == BM_LEFT_X, 0));
					sparks[0]->vx = 0;

				}
				
			}
			if (state == BM_ATTACKING_STATE && GetTickCount() - aboutToAttackTimer > ABOUT_TO_ATTACK_TIME && !releasedSparks) {
				releasedSparks = true;
				sparkExistingTimer = GetTickCount();
				sparks.clear();
				for (int i = 0; i < NUMBER_OF_SPARK; i++) {
					sparks.push_back(new Spark(x == BM_RIGHT_X ?
						BM_RIGHT_X - SPARK_WIDTH +i* SPARK_WIDTH : BM_LEFT_X + width - i * SPARK_WIDTH,
						y-16, x == BM_LEFT_X, i%4));

				}
				
			}
			if (GetTickCount() - sparkExistingTimer > SPARK_EXISTING_TIME && releasedSparks) {
				sparks.clear();
			}
			if (sparks.size() != 0) {
				for (int i = 0; i < sparks.size(); i++) {
 					sparks.at(i)->Update(dt);
				}
			}
			
			if (state == BM_ATTACKING_STATE) {
				CGimmick* g = CGimmick::GetInstance();

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

							}
						}
					}
				}
			}
			
		}
		else if(form==warior){
			if (state == WARIOR_TRANSFORMING_STATE) {
				if (GetTickCount() - transformingTimer > TRANSFORMING_TIME) {
					state = WARIOR_READY_STATE;
					readyTimer = GetTickCount();
					x += WARIOR_MARGIN_X_TO_READY;
					y -= 3;
				}
				
			}
			else if(state == WARIOR_READY_STATE ){
				if (GetTickCount() - readyTimer > WARIOR_READY_TIME) {
					state = WARIOR_STANDING;
					//y -= 5;
				}
				
			}
			else {
				CGimmick* g = CGimmick::GetInstance();
				if (state == WARIOR_RELEASING_SPARK) {
					if (GetTickCount() - releasingSparkTimer > WARIOR_RELEASING_SPARK_TIME) {
						state = WARIOR_STANDING;
						y -= 12;
						x += 5;
						/*y += 12;
						x += 5;*/

					}
				}
				else if (state == WARIOR_RELEASING_FIRE) {
					if (GetTickCount() - releasingFireTimer > WARIOR_RELEASING_FIRE_TIME) {
						state = WARIOR_STANDING;
						/*x += 9;
						y += 15;*/
						y -= 15;
						//x += 5;
					}
				}
				else if (state == WARIOR_WALKING_STATE) {
					if (GetTickCount() - walkingTimer > WARIOR_WALKING_TIME) {
						vx = 0;
						state = WARIOR_STANDING;
						x += 6;
						y -= 4;
					}
				}
				else if (state == WARIOR_STANDING) {
					if (x-g->x > WARIOR_LONG_RANGE * 1.2) {
						if (rand() % 5 >= 1) {
							state = WARIOR_JUMPING_STATE;
							vx = WARIOR_JUMPING_VX;
							vy = WARIOR_JUMPING_VY;
						}
						else{
							state = WARIOR_RELEASING_SPARK;
							releasingSparkTimer = GetTickCount();
							sparkExistingTimer = GetTickCount();
							releasedSparks = true;
							vx = vy = 0;
							y += 12;
							x -= 5;

							for (int i = 0; i < WARIOR_NUMBER_OF_SPARK; i++) {
								sparks.push_back(new Spark(x - SPARK_WIDTH - WARIOR_MARGIN_X_TO_READY,
									y + 17 + height - SPARK_HEIGHT, false, i % 4));
							}
						}
					}
					if (x - g->x > WARIOR_LONG_RANGE) {
						if (rand() % 5 >= 4) {
							state = WARIOR_JUMPING_STATE;
							vx = WARIOR_JUMPING_VX;
							vy = WARIOR_JUMPING_VY;
						}
						else if(rand() % 4 >= 3){
							state = WARIOR_RELEASING_SPARK;
							releasingSparkTimer = GetTickCount();
							sparkExistingTimer = GetTickCount();
							releasedSparks = true;
							vx = vy = 0;
							y += 12;
							x -= 5;

							for (int i = 0; i < WARIOR_NUMBER_OF_SPARK; i++) {
								sparks.push_back(new Spark(x - SPARK_WIDTH - WARIOR_MARGIN_X_TO_READY,
									y - 17 - height + SPARK_HEIGHT, false, i % 4));
							}
						}
						else {
							vx = -WARIOR_WALKING_VX;
							state = WARIOR_WALKING_STATE;
							walkingTimer = GetTickCount();

							x -= 6;
							y += 4;
							/*if (g->vx > 0) {
								if (!vx > 0) {
									state = WARIOR_WALKING_STATE;
									vx = WARIOR_WALKING_VX;
									walkingTimer = GetTickCount();
									x -= 6;
									y -= 4;
								}
							}
							else if (g->vx < 0) {
								if (!vx < 0) {
									vx = -WARIOR_WALKING_VX;
									state = WARIOR_WALKING_STATE;
									walkingTimer = GetTickCount();

									x -= 6;
									y -= 4;
								}
							}*/

						}
					}
					else if (x - g->x > WARIOR_MIDDLE_RANGE) {
						if (rand() % 5 >= 4) {
							vx = 0;
							state = WARIOR_RELEASING_FIRE;
							/*x -= 9;
							y -= 15;*/
							y += 15;
							//x -= 5;
							releasingFireTimer = GetTickCount();
							fires.clear();
							for (int i = 0; i < NUMBER_OF_FIRES; i++) {
								fires.push_back(new BFire(x + 14, y - 35, fire_VXs[i], -fire_VYs[i]));
							}

						}
						else if (rand() % 4 >= 3) {
							if (vx != 0) {
								vx = -WARIOR_WALKING_VX;
							}
						}
						else {
							if (g->vx > 0) {
								if (!vx > 0) {
									state = WARIOR_WALKING_STATE;
									vx = WARIOR_WALKING_VX;
									walkingTimer = GetTickCount();
									x -= 6;
									y += 4;
								}
							}
							else if (g->vx < 0) {
								if (!vx < 0) {
									vx = -WARIOR_WALKING_VX;
									state = WARIOR_WALKING_STATE;
									walkingTimer = GetTickCount();

									x -= 6;
									y += 4;
								}
							}

						}
					}
					else if (x - g->x < WARIOR_SHORT_RANGE) {
						if (rand() % 5 >= 2) {
							state = WARIOR_RELEASING_SPARK;
							releasingSparkTimer = GetTickCount();
							sparkExistingTimer = GetTickCount();
							releasedSparks = true;
							vx = vy = 0;
							y += 12;
							x -= 5;

							for (int i = 0; i < WARIOR_NUMBER_OF_SPARK; i++) {
								sparks.push_back(new Spark(x - SPARK_WIDTH - WARIOR_MARGIN_X_TO_READY,
									y - 17 - height + SPARK_HEIGHT, false, i % 4));
							}
						}
						else {
							state = WARIOR_GETTING_THROWN;
							vx = WARIOR_BACK_VX;
							vy = -1.5*WARIOR_BACK_VY;
						}
						
					}
					else {
						if (rand() % 100 >= 99) {
							vx = 0;
							state = WARIOR_RELEASING_FIRE;
							/*x -= 9;
							y -= 15;*/
							y += 15;
							//x -= 5;
							releasingFireTimer = GetTickCount();
							fires.clear();
							for (int i = 0; i < NUMBER_OF_FIRES; i++) {
								fires.push_back(new BFire(x+14, y-35, fire_VXs[i], -fire_VYs[i]));
							}

						}
						else {
							if (g->vx > 0) {
								if (!vx > 0) {
									state = WARIOR_WALKING_STATE;
									vx = WARIOR_WALKING_VX;
									walkingTimer = GetTickCount();
									x -= 6;
									y += 4;
								}
							}
							else if (g->vx < 0) {
								if (!vx < 0) {
									vx = -WARIOR_WALKING_VX;
									state = WARIOR_WALKING_STATE;
									walkingTimer = GetTickCount();

									x -= 6;
									y += 4;
								}
							}
							
						}
						
					}
				
				}
				
				
				if (GetTickCount() - sparkExistingTimer > SPARK_EXISTING_TIME && releasedSparks) {
					sparks.clear();
				}
				if (sparks.size() != 0) {
					for (int i = 0; i < WARIOR_NUMBER_OF_SPARK; i++) {
						sparks[i]->Update(dt);
					}
				}
				if (fires.size() != 0) {
					for (int i = 0; i < NUMBER_OF_FIRES; i++) {
						fires[i]->Update(dt, colliable_objects);
					}
				}
				CGameObject::Update(dt);
				vy -= 0.000015 * dt;

				vector<LPCOLLISIONEVENT> coEvents;
				vector<LPCOLLISIONEVENT> coEventsResult;

				coEvents.clear();
				CalcPotentialCollisions(colliable_objects, coEvents);

				if (coEvents.size() == 0) {
					x += dx;
					y += dy;
					if (x > WARIOR_RIGHT_BOUNDARY) x = WARIOR_RIGHT_BOUNDARY;
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
							if ((state == WARIOR_GETTING_INJURED && GetTickCount() - injuringTimer > WARIOR_INJURING_TIME) ||
								state == WARIOR_GETTING_THROWN || state ==WARIOR_JUMPING_STATE) {
								state = WARIOR_STANDING;
								vx = 0;
							}
							else if (state == WARIOR_GETTING_INJURED) {
								vx = 0;
							}
							vy = 0;
						}
					}
					if (x > WARIOR_RIGHT_BOUNDARY) x = WARIOR_RIGHT_BOUNDARY;

				}
			}

			
		}
		else {
			if (GetTickCount() - dyingTimer < 3000) {

			}
			else {
				if (spreadingSparkTimer == 0) {
					vx = 0.005f;
					distance = 0;
					x += 10;
					y -= 20;
					spreadingSparkTimer = GetTickCount();
				}
				else {
					if (GetTickCount() - spreadingSparkTimer < 2000) {
						dx = vx * dt;
						distance += dx;
					}
					else {
						if (fallingSparkTimer == 0) {
							fallingSparkTimer = GetTickCount();
							vy = -0.006f;
							vx = 0.01f;
							y = 0;
							x = 288;
						}
						else {
							CGameObject::Update(dt);
							y += dy;
							x += dx;
							if (abs(vx) > 0.01) {
								if (vx > 0) {
									vx -= 0.00001 * dt;
									increase = false;
								}
								else {
									vx += 0.00001 * dt;
									increase = true;
								}
							}
							else {
								if(increase)vx += 0.000005 * dt;
								else vx -= 0.000005 * dt;
							}
						}
					}
				}
			}
		}
	}
	

}
void BlackMan::GetHit() {
	if (form == blackman) {
		if (GetTickCount() - injuringTimer > BM_INJURING_TIME) {
			bm_RemainingHits -= 1;
			if (bm_RemainingHits == 0) {
				form = warior;
				state = WARIOR_TRANSFORMING_STATE;
				transformingTimer = GetTickCount();
				vx = vy = 0;
				height = WARIOR_HEIGHT;
				width = WARIOR_WIDTH;
				sparks.clear();
				bm_RemainingHits = WARIOR_SURVIVAL_TIMES;
				animation_set = CAnimationSets::GetInstance()->Get(WARIOR_ANIMATION_SET_ID);
			}
			else {
				injuringTimer = GetTickCount();
			}
		}
		
	}
	else {
		if (GetTickCount() - injuringTimer > WARIOR_INJURING_TIME) {
			bm_RemainingHits -= 1;
			if (bm_RemainingHits != 0) {
				state = WARIOR_GETTING_INJURED;
				injuringTimer = GetTickCount();
				vx = WARIOR_INJURING_VX;
				vy = WARIOR_INJURING_VY*2;

			}
			else {
				form = falling_sparks;
				vx = vy=0;
				dyingTimer = GetTickCount();
				for (int i = 0; i < 100; i++) {
					Ys[i] = rand() % 200;
					Xs[i] = rand() % 300;
				}
			}
		}
		
		
	}

}
void BlackMan::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (form == blackman) {
		if (state == BM_ATTACKING_STATE) {
			left = x;
			top = y;
			right = left + width;
			bottom = top - height;
		}
		else {
			left = top = right = bottom = 0;
		}
	}
	else if(form==warior){
		
		if (state == WARIOR_RELEASING_FIRE) {
			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 20 ;
			right = left + width;
			bottom = top - height;
		}
		else if (state == WARIOR_RELEASING_SPARK) {

			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 17 ;
			right = left + width;
			bottom = top - height;
		}
		else if (state == WARIOR_WALKING_STATE) {
			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 9;
			right = left + width;
			bottom = top - height;
		}
		else if (state == WARIOR_GETTING_INJURED) {
			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 9;
			right = left + width;
			bottom = top - height;
		}
		else if (state == WARIOR_GETTING_THROWN) {
			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 9;
			right = left + width;
			bottom = top - height;
		}
		else {
			left = x - WARIOR_MARGIN_X_TO_READY;
			top = y - 5;
			right = left + width;
			bottom = top - height;
		}
		
	}
	else {
		left = top = bottom = right = 0;
	}

	
}