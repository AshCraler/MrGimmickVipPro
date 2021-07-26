#pragma once
#include "GameObject.h"
#define FLOATING_SPEED	0.001f
#define ISLAND_GRAVITY	0.000001f
class CPlayer : public CGameObject
{
public:
	virtual void Load(LPCWSTR File) = 0;
};

class CMenu :public CPlayer {
public:
	bool increasingSpeed = false;
	float	yCastle, vyCastle;
	bool castleIsDown = false;
	float vx1, vx2, vx3, vx4, vx5, vx6;
	float x1, x2, x3, x4, x5, x6;

	int nextStage = 0;
	int win = 1;
public:
	static CMenu* instance;
	void Load(LPCWSTR filepath) {}
	static CMenu* GetInstance();
	void Render();
	void Update(DWORD dt);
	void Control(int con);
	CMenu();
	~CMenu() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = top = bottom = right = 0; };
};

#define FULL_IMAGE_TIME	1000
#define FADING_TIME	5000
class Intro :public CPlayer {
public:
	int stage = 0;
	float op = -1;
	DWORD fullImageTimer = 0, fadingTimer = 0, lastChange;
public:
	static Intro* instance;
	void Load(LPCWSTR filepath) {}
	static Intro* GetInstance();
	void Render();
	void Update(DWORD dt);
	void Control(int con);
	Intro();
	~Intro() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = top = bottom = right = 0; };
};