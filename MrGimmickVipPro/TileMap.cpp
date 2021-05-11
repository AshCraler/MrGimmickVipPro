#include "TileMap.h"
#include "Camera.h"
#include "Utils.h"

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

	for (int i = 0; i < rows; i++)
	{
		pFile.good();
		getline(pFile, lineString);
		tokens = split(lineString, "\t");
		matrix[i] = new Tile[tokens.size()];

		for (int j = 0; j < cols; j++)
		{
			RECT bound;
			D3DXVECTOR2 position;
			int id = atoi(tokens[j].c_str()) - 1;
			matrix[i][j].setID(id);

			bound.top = (id / spritePerRow) * (tileHeight+1) + 1 ;
			bound.left = (id % spritePerRow) * (tileWidth+1) + 1;
			bound.right = bound.left + tileWidth;
			bound.bottom = bound.top + tileHeight;

			position.x = j * tileWidth;
			position.y = i * tileHeight;
			matrix[i][j].SetBound(bound);
			matrix[i][j].SetPosition(position);
			matrix[i][j].SetSprite(sprite);

			//DebugOut(L"[INFO] tile add: %d, %d, %d, %d, %d, %f, %f \n", id, bound.left, bound.top, bound.right, position.x, position.y);
		}
	}

	pFile.close();
}


void TileMap::Render()
{
	CCamera* camera = CCamera::GetInstance();
	int rowStart;
	int rowEnd;
	int colStart;
	int colEnd;

	if ((camera->GetY() / tileHeight) < 0)
		rowStart = 0;
	else
		rowStart = (camera->GetY() / tileHeight);

	if (((camera->GetY() / tileHeight + camera->GetHeight() / tileHeight) + 1) > rows)
		rowEnd = rows;
	else
		rowEnd = ((camera->GetY() / tileHeight + camera->GetHeight() / tileHeight) + 1);

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