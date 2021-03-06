#pragma once
#include "../GameObject.h"
#include "../Gimmick.h"
class Enemy:public CGameObject
{
	string category = "ENEMY";
protected:
	bool beaten = false;
	bool direction;
	bool outOfCamera = false;
	int carryJar = -1;

public:
	void SetBeaten(bool b) { beaten = b; }
	bool IsBeaten() { return beaten; }
	void SetDirection(bool d) { direction = d; }
	bool GetDirection() { return direction; }
	void SetOutOfCamera(bool d) { direction = d; }
	bool IsOutOfCamera() { return direction; }
	int IsCarryingJar() { return carryJar; }
};

