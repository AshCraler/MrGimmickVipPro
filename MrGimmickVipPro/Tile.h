#pragma once
#include<Windows.h>
#include <d3dx9.h>
#include<d3d9.h>
#include "Sprites.h"
#include "Camera.h"
#define CHANGE_SPRITE_TIME	500

class Tile
{
	int id;
	RECT bound;
	D3DXVECTOR2 position;
	LPSPRITE sprite;
	CCamera* camera;
	int boundSetID = -1;
	int startAt = 0;
	DWORD changeSpriteTimer;
public:
	static unordered_map<int, vector<RECT>> bounds;
	static unordered_map<int, int> spriteTimes;
	Tile();
	~Tile();
	Tile(int _id, RECT _bound, D3DXVECTOR2 _position);
	void setID(int _id);
	void SetPosition(D3DXVECTOR2 _position);
	void SetBound(RECT _bound);
	void SetSprite(LPSPRITE _sprite);
	int GetID() { return id; }
	D3DXVECTOR2 GetPosision() { return this->position; }
	RECT GetBound() { this->bound; }
	void Render();
	bool CheckInBoundCamera(int screenWidth, int screenHeight);
	void SetBoundSet(int id, int startAt) {
		boundSetID = id;
		this->startAt = startAt;
	}
};

class Tile;
typedef Tile* LPTILE;