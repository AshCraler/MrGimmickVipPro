#pragma once
#include "../GameObject.h"
#define NUMBER_OF_FISH	50
class Item:public CGameObject
{
protected:
	int type;
	bool taken;
	string category = "ITEM";
public:
	int GetType() { return type; }
	bool IsTaken() { return taken; }
	void SetTaken(bool taken) { this->taken = taken; x = y = width = height = 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class Fish:public	CGameObject {
public:
	float Xs[NUMBER_OF_FISH];
	float Ys[NUMBER_OF_FISH];
	float VXs[NUMBER_OF_FISH];
	float VYs[NUMBER_OF_FISH];
	int types[NUMBER_OF_FISH];
	DWORD changeVX[NUMBER_OF_FISH];
	bool turn[NUMBER_OF_FISH];

	Fish();
	~Fish();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
};