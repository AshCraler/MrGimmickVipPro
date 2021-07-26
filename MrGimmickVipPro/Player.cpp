#include "Player.h"
#include "SceneManager.h"

CMenu* CMenu::instance = NULL;

CMenu* CMenu::GetInstance() {
	if (instance == NULL) instance = new CMenu();
	return instance;
}
CMenu::CMenu() {
	vy = FLOATING_SPEED;
	vyCastle = 0 / 2;

	x = 45;
	y = 120;
	yCastle =y+ 31;
	increasingSpeed = false;
	animation_set = CAnimationSets::GetInstance()->Get(-30);
	win = 0;
	x1 = x + 0;
	x2 = x + 40;
	x3 = x + 90;
	x4 = x + 120;
	x5 = x + 180;
	x6 = x + 210;

	vx1 = -0.0018f;
	vx2 = -0.002f;
	vx3 = -0.0015f;
	vx4 = -0.0018f;
	vx5 = -0.0013f;
	vx6 = -0.0011f;

}
void CMenu::Update(DWORD dt) {
	CGameObject::Update(dt);
	y += dy;
	if (increasingSpeed) {
		vy += ISLAND_GRAVITY * 10;
		if (vy > FLOATING_SPEED) increasingSpeed = false;
	}
	else {
		vy -= ISLAND_GRAVITY * 10;
		if (vy < -FLOATING_SPEED) increasingSpeed = true;
	}

	yCastle += vyCastle * dt;
	if (castleIsDown) {
		vyCastle += ISLAND_GRAVITY * 10;
		if (vyCastle > FLOATING_SPEED) castleIsDown = false;
	}
	else {
		vyCastle -= ISLAND_GRAVITY * 10;
		if (vyCastle < -FLOATING_SPEED) castleIsDown = true;
	}
	x1 += vx1 * dt;
	x2 += vx2 * dt;
	x3 += vx3 * dt;
	x4 += vx4 * dt;
	x5 += vx5 * dt;
	x6 += vx6 * dt;
	if (x1 + 30 < 0)x1 = 256;
	if (x2 + 30 < 0)x2 = 256;
	if (x3 + 30 < 0)x3 = 256;
	if (x4 + 30 < 0)x4 = 256;
	if (x5 + 30 < 0)x5 = 256;
	if (x6 + 30 < 0)x6 = 256;
}
void CMenu::Render() {
	animation_set->at(10)->Render(0, 150);

	animation_set->at(0)->Render(x, y);
	animation_set->at(7)->Render(x + 50, yCastle);
	animation_set->at(8)->Render(x + 56, y - 54);
	animation_set->at(8)->Render(x + 65, y - 54);
	animation_set->at(6)->Render(x + 75, y-83);
	animation_set->at(2)->Render(x6, y +20);
	animation_set->at(1)->Render(x1, y + 0);
	animation_set->at(1)->Render(x2, y - 30);
	animation_set->at(2)->Render(x3, y - 80);
	animation_set->at(2)->Render(x4, y - 100);
	animation_set->at(3)->Render(x5, y - 160);

	if (nextStage == 0) {
		animation_set->at(5)->Render(x+81, y - 100);
		if (win == 1) {
			animation_set->at(4)->Render(x + 90, y - 90);

		}
		else if (win == -1) {
			animation_set->at(3)->Render(x + 90, y - 90);

		}
	}
	else if (nextStage == 1) {
		animation_set->at(5)->Render(x + 106, y - 85);

	}
	if (nextStage == 2) {
		animation_set->at(9)->Render(x + 112, y - 41);
		animation_set->at(5)->Render(x + 52, yCastle -5 );

	}

}
void CMenu::Control(int x) {
	if (x == 10) {
		if(nextStage%3==1){
			CSceneManager::GetInstance()->SwitchScene(21);
		}
		else if (nextStage%3 == 2) {
			CSceneManager::GetInstance()->SwitchScene(31);
		}else {
			CSceneManager::GetInstance()->SwitchScene(11);
		}

	}
}
Intro* Intro::instance = NULL;

Intro* Intro::GetInstance() {
	if (instance == NULL) instance = new Intro();
	return instance;
}
Intro::Intro() {
	x = 64;
	y = 150;
	

}
void Intro::Update(DWORD dt) {
	if (stage == 0 && fullImageTimer == 0 && op == -1) {
		fullImageTimer = GetTickCount();
	}
	else {
		if (fullImageTimer != 0) {
			if (GetTickCount() - fullImageTimer > FULL_IMAGE_TIME) {
				fullImageTimer = 0;
				op = 0;
				
			}
			else {
				
			}
		}
		else {
			if (op >= 255) {
				fullImageTimer = GetTickCount();
				stage++;
				if (stage == 14) {
					CSceneManager::GetInstance()->SwitchScene(41);
				}
			}
			else {
				op += 0.5;
			}
		}
		
	}
}
void Intro::Render() {
	CSprites::GetInstance()->Get(400015)->Draw(0, 150, 255, 255, 255);

	if (fullImageTimer==0) {
		CSprites::GetInstance()->Get(400001 + stage+1)->Draw(x, 115, op, op, op);
		CSprites::GetInstance()->Get(400001 + stage)->Draw(x, 115, 255 - op, 255 - op, 255 - op);

	}
	else {
		CSprites::GetInstance()->Get(400001 + stage)->Draw(x, 115, 255, 255, 255);

	}
	
}
void Intro::Control(int x) {
	if (x == 1) {
		if (GetTickCount() - lastChange > 1000) {
			lastChange = GetTickCount();
			stage++;
			fullImageTimer = GetTickCount();
			op = 0;

			if (stage == 14) {
				CSceneManager::GetInstance()->SwitchScene(41);

			}
		}
		

	}
}