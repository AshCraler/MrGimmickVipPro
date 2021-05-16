#include "Hill.h"


void Hill::Render()
{
	RenderBoundingBox();
}

void Hill::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

Hill::Hill(int x, int y, int width, int height, bool direction)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->up = direction;
}

Hill::~Hill()
{
}
