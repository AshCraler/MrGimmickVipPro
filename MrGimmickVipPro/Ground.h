#pragma once
#include "GameObject.h"
class Ground : public CGameObject
{
	CSprite* sprite;
public:
	Ground();
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	Ground(int width, int height);
	Ground(int x, int y, int w, int h);
	~Ground();
};

