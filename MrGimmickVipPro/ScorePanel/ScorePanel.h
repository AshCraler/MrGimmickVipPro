#pragma once
#include "../Sprites.h"
#include "../Camera.h"
#include "../Gimmick.h"
#define NUMBER_OF_DIGITS_IN_SCORE	8
#define X_MARGIN_OF_SCORE	16
#define Y_MARGIN_OF_SCORE	16
#define DIGIT_SIZE			8
#define X_MARGIN_OF_RESET	96
#define Y_MARGIN_OF_RESET	16
#define X_MARGIN_OF_BULBS	126
#define Y_MARGIN_OF_BULBS	32
#define BULB_DISTANCE	2
#define BULB_SIZE		9
#define X_MARGIN_OF_SQUARE	176
#define Y_MARGIN_OF_SQUARE	0
#define SQUARE_WIDTH		24
#define SQUARE_HEIGHT		32
#define DIGIT_SPRITE_ID_STARTS_AT	600301
#define SCORE_PANEL_SPRITE_ID	600201
class ScorePanel
{
public:
	static void Render();
};

