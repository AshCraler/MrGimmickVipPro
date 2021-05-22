#include "BigPrize.h"
BigPrize::BigPrize(int x, int y, int type) {
	this->x = x;
	this->y = y;
	this->type = type;
	if (type == FLOWER_POT) {
		this->width = POT_WIDTH;
		this->height = POT_HEIGHT;
	}
	else if (type == HOUR_GLASS) {
		this->width = GLASS_WIDTH;
		this->height = GLASS_HEIGHT;
	}
	
	animation_set = CAnimationSets::GetInstance()->Get(ANIMATION_SET_ID);
	int sparkleXRange = x - width;
	int sparkleYRange = y - height;

	for (int i = 0; i < NUMBER_OF_SPARKLES; i++) {
		sparkles.push_back(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ID)->at(rand() % 8 + SPARKLE_ANIMATION_ID_STARTS_AT));
		sparkleXs.push_back(sparkleXRange + rand() % (width*3));
		sparkleYs.push_back(sparkleYRange + rand() % (height * 2));
	}
}
void BigPrize::Render() {
	if (!taken) {
		animation_set->at(type)->Render(x, y);
		for (int i = 0; i < NUMBER_OF_SPARKLES; i++) {
			sparkles[i]->Render(sparkleXs[i], sparkleYs[i]);
		}
		RenderBoundingBox();
	}
	
}