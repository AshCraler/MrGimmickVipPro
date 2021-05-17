#include "ScorePanel.h"

void ScorePanel::Render() {
	int y = CCamera::GetInstance()->GetHeight();
	CSprite* sp = CSprites::GetInstance()->Get(SCORE_PANEL_SPRITE_ID);
	sp->DrawWithoutCamera(0, y);

	//int score = CGimmick::GetInstance()->GetScore();
	int score = 11234;
	int temp;
	int count = 0;
	while (score != 0) {
		temp = score % 10;
		score = score / 10;
		CSprites::GetInstance()->Get(DIGIT_SPRITE_ID_STARTS_AT+temp)->DrawWithoutCamera(
			X_MARGIN_OF_SCORE + (NUMBER_OF_DIGITS_IN_SCORE - count - 1)*DIGIT_SIZE,
			y+Y_MARGIN_OF_SCORE);
		count++;
	}
}