#include "Item.h"

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y - height;
	
}
Fish::Fish() {
	for (int i = 0; i < NUMBER_OF_FISH; i++) {
		Xs[i] = rand() % 500;
		if (Xs[i] < 0)
			x = x;
		Ys[i] = rand() % 200;
		VXs[i] = ((rand() % 20)-10) * 0.001;
		VYs[i]= ((rand() % 20) - 10) * 0.0005;
		types[i] = rand() % 3;
		turn[i] = false;
	}
	animation_set = CAnimationSets::GetInstance()->Get(-20);
}
Fish::~Fish(){}
void Fish::Render() {
	for (int i = 0; i < NUMBER_OF_FISH; i++) {
		if (!turn[i]) {
			if (VXs[i] < 0) {
				animation_set->at(types[i])->Render(Xs[i], Ys[i]);
			}
			else {
				animation_set->at(types[i] + 3)->Render(Xs[i], Ys[i]);

			}

		}
		else {
			if (VXs[i] < 0) {
				animation_set->at(types[i]+6)->Render(Xs[i], Ys[i]);
			}
			else {
				animation_set->at(types[i] + 3+6)->Render(Xs[i], Ys[i]);

			}
		}
	}
}
void Fish::Update(DWORD dt) {
	for (int i = 0; i < NUMBER_OF_FISH; i++) {
		if (!turn[i]) {
			Xs[i] += VXs[i] * dt;
			Ys[i] += VYs[i] * dt;
		}
		
		if (Xs[i] < 0) {
			if (VXs[i] < 0) {
				//VXs[i] = -VXs[i];
				Xs[i] = 0 + 1;
				/*if (VXs[i] < 0) {
					VXs[i] -= 0.001;
				}
				else {
					VXs[i] += 0.001;
				}*/
				turn[i] = true;
				changeVX[i] = GetTickCount();
			}
			
			
		}
		else if (Xs[i] > 470) {
			if (VXs[i] > 0) {
				//VXs[i] = -VXs[i];
				/*if (VXs[i] < 0) {
					VXs[i] -= 0.01;
				}
				else {
					VXs[i] += 0.01;
				}*/
				Xs[i] = 470 - 1;
				turn[i] = true;
				changeVX[i] = GetTickCount();
			}
			
		}
		else {
			if (Ys[i] < 10) {
				VYs[i] = -VYs[i];
				Ys[i] = 10;

			}
			else if (Ys[i] > 170) {
				VYs[i] = -VYs[i];
				Ys[i] = 170;

			}
			else {
				if (turn[i]) {
					if (GetTickCount() - changeVX[i] > 100) {
						turn[i] = false;
						VXs[i] = ((rand() % 10)) * 0.0005 * (VXs[i] > 0 ? -1 : 1);
					}
				}
				if (rand() % 1000 == 0) {
					VYs[i] = ((rand() % 20) - 10) * 0.0005;
				}
				else {
					/*if (Xs[i] < 30 || Xs[i]>410) {
						if (rand() % 400 == 0) {
							turn[i] = true;
							changeVX[i] = GetTickCount();

						}
					}
					else*/ 
					if (rand() % 2000<=abs(Xs[i]-250)/25) {
						turn[i] = true;
						changeVX[i] = GetTickCount();

					}
				}
			}
		}
		
		if (VXs[i] == 0) {
			if (Xs[i] < 250)VXs[i] = 0.01f;
			else VXs[i] = -0.01f;
		}
	}
}
void Fish::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0 + 0;
	b = 0 - 0;

}