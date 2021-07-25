#include "TileMap.h"
#include "Camera.h"
#include "Utils.h"
#include "Gimmick.h"

using namespace std;

TileMap::TileMap(float _width, float _height, LPSPRITE _sprite, string pathFile)
{
	width = _width;
	height = _height;
	sprite = _sprite;
	tileHeight = tileWidth = 16;
	this->LoadListTile(pathFile);
}


TileMap::~TileMap()
{

}

void TileMap::LoadListTile(string pathFile) {
	fstream pFile;

	pFile.open(pathFile, fstream::in);
	string lineString;

	vector<string> tokens;

	pFile.good();
	getline(pFile, lineString);
	tokens = split(lineString, "\t");
	rows = atoi(tokens[0].c_str());
	cols = atoi(tokens[1].c_str());
	width = cols*tileWidth;
	height = rows*tileHeight;

	matrix = new LPTILE[rows];

	pFile.good();
	getline(pFile, lineString);
	spritePerRow = atoi(lineString.c_str());

	getline(pFile, lineString);
	tokens = split(lineString, "\t");
	int margin1 = atoi(tokens[0].c_str()), margin2 = atoi(tokens[1].c_str());

	getline(pFile, lineString);
	tokens = split(lineString, "\t");
	int mSprites = atoi(tokens[0].c_str());

	for (int i = 0; i < mSprites; i++) {
		pFile.good();
		getline(pFile, lineString);
		tokens = split(lineString, "\t");
		if (Tile::bounds.find(atoi(tokens[0].c_str())) == Tile::bounds.end()) {
			Tile::spriteTimes[atoi(tokens[0].c_str())] = atoi(tokens[1].c_str());
			vector<RECT> bs;
			for (int i = 2; i < tokens.size(); i++) {
				RECT bound;
				int id = atoi(tokens[i].c_str());
				bound.bottom = sprite->bottom - (id / spritePerRow) * (tileHeight + margin1) - margin2;
				bound.left = (id % spritePerRow) * (tileWidth + margin1) + margin2;
				bound.right = bound.left + tileWidth;
				bound.top = bound.bottom - tileHeight;

				bs.push_back(bound);
			}
			Tile::bounds[atoi(tokens[0].c_str())] = bs;
		}
	}
	for (int i = 0; i < rows; i++)
	{
		pFile.good();
		getline(pFile, lineString);
		tokens = split(lineString, "\t");
		matrix[rows- i-1] = new Tile[tokens.size()];

		for (int j = 0; j < cols; j++)
		{
			RECT bound;
			D3DXVECTOR2 position;
			int id = atoi(tokens[j].c_str()) - 1;
			if (id < 1000) {
				matrix[rows - i - 1][j].setID(id);

				bound.bottom = sprite->bottom - (id / spritePerRow) * (tileHeight + margin1) - margin2;
				bound.left = (id % spritePerRow) * (tileWidth + margin1) + margin2;
				bound.right = bound.left + tileWidth;
				bound.top = bound.bottom - tileHeight;

				position.x = j * tileWidth;
				position.y = (rows - i - 1) * tileHeight;
				matrix[rows- i-1][j].SetBound(bound);
				matrix[rows- i-1][j].SetPosition(position);
				matrix[rows- i-1][j].SetSprite(sprite);
			}
			else {
				id++;
				position.x = j * tileWidth;
				position.y = (rows - i - 1) * tileHeight;

				matrix[rows- i-1][j].SetBoundSet(id/100,id%100 );
				matrix[rows- i-1][j].SetPosition(position);
				matrix[rows- i-1][j].SetSprite(sprite);
			}
			

			//DebugOut(L"[INFO] tile add: %d, %d, %d, %d, %d, %f, %f \n", id, bound.left, bound.top, bound.right, position.x, position.y);
		}
	}

	pFile.close();
}


void TileMap::Render()
{
	if (!CGimmick::GetInstance()->finalScene) {
		CCamera* camera = CCamera::GetInstance();
		int rowStart;
		int rowEnd;
		int colStart;
		int colEnd;

		if ((camera->GetY()-camera->GetHeight()) / tileHeight < 0)
			rowStart = 0;
		else
			rowStart =(camera->GetY() -camera->GetHeight())/ tileHeight;

		if ((camera->GetY() / tileHeight+ 1) > rows)
			rowEnd = rows;
		else
			rowEnd = ((camera->GetY() / tileHeight) + 1);

		if ((camera->GetX() / tileWidth) < 0)
			colStart = 0;
		else
			colStart = (camera->GetX() / tileWidth);

		if (((camera->GetX() / tileWidth + camera->GetWidth() / tileWidth) + 1) > cols)
			colEnd = cols;
		else
			colEnd = ((camera->GetX() / tileWidth + camera->GetWidth() / tileWidth) + 1);

		for (int i = rowStart; i < rowEnd; i++)
		{
			for (int j = colStart; j < colEnd; j++)
			{
				matrix[i][j].Render();
			}
		}
	}
	else {
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				matrix[i][j].Render();
			}
		}
	}
	

	/*for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (matrix[i][j].CheckInBoundCamera(camera->GetWidth(), camera->GetHeight()))
			{
				matrix[i][j].Render();
			}
		}
	}*/
}
void TileMap::Unload()
{
	if (matrix)
	{
		/*for (int i = 0; i < rows; i++)
		{
			delete matrix[i];
		}*/
		//delete matrix;
		//matrix = nullptr;
	}

	if (sprite)
	{
		sprite = nullptr;
	}
}