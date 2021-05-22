#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Tile.h"
using namespace std;

class TileMap
{
private:
	Tile** matrix;
	int rows;
	int cols;
	float width;
	float height;
	float tileWidth;
	float tileHeight;
	int spritePerRow;
	LPSPRITE sprite;
	void LoadListTile(string file);
public:
	TileMap(float _width, float _height, LPSPRITE _sprite, string pathFile);
	~TileMap();
	void Render();
	void Unload();
};