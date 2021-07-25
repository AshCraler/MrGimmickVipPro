#pragma once
#include "StaticObject.h"
#include "../Enemies/SmallBlackBugWithAntena.h"
#define ANIMATIO_SET_ID	-1
#define WIDTH	48
#define HEIGHT	80
#define CLOSING_STATE	0
#define OPENING_STATE	1
#define OPENED_STATE	2
#define OPENING_TIME	1000
class Window :public StaticObject
{
	DWORD opening_timer;
	SmallBlackBugWithAntena* bug;
public:
	Window(int x, int y, SmallBlackBugWithAntena* b);
	~Window();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OpenWindow();
	void Update(DWORD dt);
};

