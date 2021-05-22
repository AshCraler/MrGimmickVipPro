#include "Ground.h"

Ground::Ground()
{
}

void Ground::Render()
{
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

Ground::Ground(int width, int height)
{
	this->width = width;
	this->height = height;
}
Ground::Ground(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Ground::~Ground()
{
}
