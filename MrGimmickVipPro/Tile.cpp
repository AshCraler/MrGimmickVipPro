#include "Tile.h"


Tile::Tile()
{
	camera = CCamera::GetInstance();
}

Tile::~Tile()
{
}

Tile::Tile(int _id, RECT _bound, D3DXVECTOR2 _position)
{
	id = _id;
	bound = _bound;
	position = _position;
}

void Tile::setID(int _id)
{
	id = _id;
}

void Tile::SetPosition(D3DXVECTOR2 _position)
{
	position = _position;
}

void Tile::SetBound(RECT _bound)
{
	bound = _bound;
}

void Tile::SetSprite(LPSPRITE _sprite)
{
	sprite = _sprite;
}

void Tile::Render()
{
	sprite->Draw(position.x, position.y, bound);
}

bool Tile::CheckInBoundCamera(int screenWidth, int screenHeight)
{
	D3DXVECTOR3 cameraPosition = camera->GetPosition();

	/*return (left >= cameraPosition.x && top >= cameraPosition.y
		&& right <= cameraPosition.x + screenWidth && bottom <= cameraPosition.y + screenHeight);*/
	float boundLeft = cameraPosition.x;
	float boundTop = cameraPosition.y;
	float boundRight = boundLeft + screenWidth;
	float boundBottom = boundTop + screenHeight;

	float left = position.x;
	float top = position.y;
	float right = left + bound.right - bound.left;
	float bottom = top + bound.bottom - bound.top;

	if (left > boundRight || right < boundLeft
		|| top > boundBottom || bottom < boundTop)
	{
		return false;
	}

	return true;
}
