#pragma once
#include "../StaticObjects/Hill.h"
#include "../Enemies/SmallBlackBug.h"
#include "DynamicObject.h"
#define FALLING_SPEED 0.008f
#define FLYING_STATE	2
#define ROLLING_STATE	1
#define SHELL_EXPLODING_STATE	2
#define NUMBER_OF_ANIMATIONS	2
#define SHELL_ANIMATIO_SET_ID	52
#define SHELL_WIDTH 16
#define SHELL_HEIGHT 16
#define SHELL_GRAVITY 0.0001f
#define ROLLING_X_SPEED 0.02f
#define ROLLING_Y_SPEED 0.04f
#define SHELL_X_RANGE	10
#define SHELL_EXISTING_TIME	5000

#define OBHS_INACTIVE_STATE	0
#define OBHS_RELEASED_STATE	1
#define OBHS_EXPLODED_STATE	2
#define OBHS_X_SPEED	0.02f
#define OBHS_Y_SPEED	0.02f
#define EXISTING_TIME	2000
#define EXPLODING_TIME	2000
class OnBugHeadShell : public DynamicObject {
	BombCaryingBug* bug;
	DWORD existingTime = NULL, explodingTime=NULL;
public:

	OnBugHeadShell(BombCaryingBug* b);
	~OnBugHeadShell();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void GetReleased();
	void Explode();

};
class Shell :public DynamicObject
{
	int onHill = 0;
	DWORD existingTimer, explodingTimer;
	enum  onHillStages
	{
		OutOfArea = 0,
		Enter = 1,
		Touching = 2,
	};
	Hill* hill;
	int type = rolling;
	int markedX = 0;
public:
	enum ShellTypes {
		rolling = 1,
		flying = 2,
	};
	Shell(int x, int y, int direction, bool dir =false);
	~Shell();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetVX(float _vx);
	void SetVY(float _vy);
	void Explode();
};

