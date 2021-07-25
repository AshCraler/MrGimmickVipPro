#pragma once
#include "StaticObject.h"
class Portal:public StaticObject
{
public:
	int map, scene;
public:
	Portal(int x, int y, int width, int height, int map, int scene);
	~Portal();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

