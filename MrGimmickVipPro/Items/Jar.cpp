#include "Jar.h"

Jar::~Jar()
{
}

void Jar::Render()
{
	if (!taken) {
		CSprites::GetInstance()->Get(JAR_SPRITE_ID_STARTS_AT + type)->Draw(x, y);
		RenderBoundingBox();
	}
}


Jar::Jar(int x, int y, int type)
{
	this->x = x;
	this->y = y;
	this->type = type;
	if (type == PINK_LIQUID) height = JAR_HEIGHT - 2;
	else if (type == ORANGE_LIQUID) height = JAR_HEIGHT;
	else height = JAR_HEIGHT - 1;
	width = JAR_WIDTH;
}