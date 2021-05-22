#pragma once
//#include "GameObject.h"
#include "StaticObject.h"
class Ground : public StaticObject
{
public:
	Ground();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	Ground(int width, int height);
	Ground(int x, int y, int w, int h);
	~Ground();
};

