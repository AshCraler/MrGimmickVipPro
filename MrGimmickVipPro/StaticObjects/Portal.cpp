#include "Portal.h"
Portal::Portal(int x, int y, int width, int height, int map, int scene) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->map = map;
	this->scene = map*10+scene;

}
Portal::~Portal() {}
void Portal::Render() {
	RenderBoundingBox();
}
void Portal::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x; r = x + width;
	t = y; b = y - height;
}