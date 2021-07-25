#include "ScorePanel.h"

void ScorePanel::Render() {
	int y = -CCamera::GetInstance()->GetHeight();

	CSprite* sp = CSprites::GetInstance()->Get(SCORE_PANEL_SPRITE_ID);

	sp->DrawWithoutCamera(0, y);

	//int score = CGimmick::GetInstance()->GetScore();
	int score = CGimmick::GetInstance()->score;
	float lives = CGimmick::GetInstance()->lives;
	int temp;
	int count = 0;
	while (score != 0) {
		temp = score % 10;
		score = score / 10;
		CSprites::GetInstance()->Get(DIGIT_SPRITE_ID_STARTS_AT+temp)->DrawWithoutCamera(
			X_MARGIN_OF_SCORE + (NUMBER_OF_DIGITS_IN_SCORE - count - 1)*DIGIT_SIZE,
			y-Y_MARGIN_OF_SCORE);
		count++;
	}
	if (lives == 0) {

	}
	else if(lives < 1){
		if (lives != 0) {
			CSprites::GetInstance()->Get(LED_POINT_DULL + 0)->DrawWithoutCamera(120, y-8);
		}

	}
	else if (lives < 2) {
		if (lives != 1) {
			CSprites::GetInstance()->Get(LED_POINT_DULL + 1)->DrawWithoutCamera(120, y - 8);
		}
		CSprites::GetInstance()->Get(LED_POINT_BRIGHT + 0)->DrawWithoutCamera(120, y - 8);

	}
	else if (lives < 3) {
		if (lives != 2) {
			CSprites::GetInstance()->Get(LED_POINT_DULL + 2)->DrawWithoutCamera(120, y - 8);
		}
		CSprites::GetInstance()->Get(LED_POINT_BRIGHT + 1)->DrawWithoutCamera(120, y - 8);
	}
	else if (lives < 4) {
		if (lives != 3) {
			CSprites::GetInstance()->Get(LED_POINT_DULL + 3)->DrawWithoutCamera(120, y - 8);
		}
		CSprites::GetInstance()->Get(LED_POINT_BRIGHT + 2)->DrawWithoutCamera(120, y - 8);
	}
	else {
		CSprites::GetInstance()->Get(LED_POINT_DULL + 3)->DrawWithoutCamera(120, y - 8);

	}

	vector<int> items = CGimmick::GetInstance()->items;
	for (int i = 0; i < items.size(); i++) {
		if (items[i] != 0) {
			CSprites::GetInstance()->Get(ITEM_SQUARE+items[i])->DrawWithoutCamera(175 + i*24, y);
		}
	}
	int pickItem = CGimmick::GetInstance()->pickedItem;
	if (pickItem != 0) {
		CSprites::GetInstance()->Get(ITEM_SQUARE_BRIGHT + pickItem)->DrawWithoutCamera(175, y);
	}
}