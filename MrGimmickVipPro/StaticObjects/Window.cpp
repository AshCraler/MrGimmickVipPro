#include "Window.h"
Window::Window(int x, int y, SmallBlackBugWithAntena* b) {
	this->x = x;
	this->y = y;
	this->width = WIDTH;
	this->height = HEIGHT;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATIO_SET_ID));
	this->state = CLOSING_STATE;
	this->bug = b;
	
}
Window::~Window() {}
void Window::Render() {
	animation_set->at(state)->Render(x, y+32);
	RenderBoundingBox();
}
void Window::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (state == CLOSING_STATE) {
		l = x; r = x + width;
		t = y; b = y - height;
	}
	else {
		l = r = 0;
		t = b = 0;
	}
	
}
void Window::OpenWindow() {
	this->state = OPENING_STATE;
	opening_timer = GetTickCount();
}
void Window::Update(DWORD dt) {
	if (state == OPENING_STATE && GetTickCount() - opening_timer > OPENING_TIME) {
		this->state = OPENED_STATE;
		bug->SetActive();
	}
}