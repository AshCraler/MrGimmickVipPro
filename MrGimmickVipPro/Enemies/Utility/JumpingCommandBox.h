#pragma once
#include "../../GameObject.h"
class JumpingCommandBox:public CGameObject
{
	bool collisionDirection; float vx;
public:
	JumpingCommandBox(int x, int y, int width, int height, bool dir);
	~JumpingCommandBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};

class ReturningCommandBox :public CGameObject
{
	bool collisionDirection; float vx;
public:
	ReturningCommandBox(int x, int y, int width, int height, bool dir);
	~ReturningCommandBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};
class ReturnOrNotCommandbBox :public CGameObject
{
	bool collisionDirection; float vx;
public:
	bool returnOrNot;

	ReturnOrNotCommandbBox(int x, int y, int width, int height, bool dir, bool r);
	~ReturnOrNotCommandbBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};
class SelectiveJumpingCommandBox :public CGameObject
{
	bool collisionDirection; float vx;
public:
	SelectiveJumpingCommandBox(int x, int y, int width, int height, bool dir, int vx);
	~SelectiveJumpingCommandBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};
class LockingViewPoint :public CGameObject
{
	float vx;
	int from;
public:
	LockingViewPoint(int x, int y, int width, int height, bool dir, int from);
	~LockingViewPoint();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	int GetFrom() { return from; };
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};
class LockingViewToPoint :public CGameObject
{
	float vx;
	int from;
public:
	LockingViewToPoint(int x, int y, int width, int height, bool dir, int from);
	~LockingViewToPoint();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	int GetTo() { return from; };
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
};