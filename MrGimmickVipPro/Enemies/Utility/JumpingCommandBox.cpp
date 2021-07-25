#include "JumpingCommandBox.h"

JumpingCommandBox::JumpingCommandBox(int x, int y,int width, int height, bool dir) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
}
JumpingCommandBox::~JumpingCommandBox() {}
void  JumpingCommandBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void JumpingCommandBox::Render() {
	RenderBoundingBox();
}

ReturningCommandBox::ReturningCommandBox(int x, int y, int width, int height, bool dir) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
}
ReturningCommandBox::~ReturningCommandBox() {}
void  ReturningCommandBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void ReturningCommandBox::Render() {
	RenderBoundingBox();
}

ReturnOrNotCommandbBox::ReturnOrNotCommandbBox(int x, int y, int width, int height, bool dir, bool r) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
	this->returnOrNot = r;
}

ReturnOrNotCommandbBox::~ReturnOrNotCommandbBox() {}
void  ReturnOrNotCommandbBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void ReturnOrNotCommandbBox::Render() {
	RenderBoundingBox();
}
SelectiveJumpingCommandBox::SelectiveJumpingCommandBox(int x, int y, int width, int height, bool dir, int vx) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
}
SelectiveJumpingCommandBox::~SelectiveJumpingCommandBox() {}
void  SelectiveJumpingCommandBox::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void SelectiveJumpingCommandBox::Render() {
	RenderBoundingBox();
}
LockingViewPoint::LockingViewPoint(int x, int y, int width, int height, bool dir, int fr) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
	this->from = fr;
}
LockingViewPoint::~LockingViewPoint() {}
void  LockingViewPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void LockingViewPoint::Render() {
	RenderBoundingBox();
}
LockingViewToPoint::LockingViewToPoint(int x, int y, int width, int height, bool dir, int fr) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->direction = dir;
	this->from = fr;
}
LockingViewToPoint::~LockingViewToPoint() {}
void  LockingViewToPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + width;
	bottom = top - height;
}
void LockingViewToPoint::Render() {
	RenderBoundingBox();
}