#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	if (id == 400000)
		id = id;
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

//void CSprite::Draw(float x, float y, int alpha)
//{
//	CGame * game = CGame::GetInstance();
//	game->Draw(x, y, texture, left, top, right, bottom, alpha);
//}

void CSprite::Draw(float x, float y, int alpha, int r, int g, int b)
{
	CGame::GetInstance()->Draw(x + dx, y + dy, texture, left, top, right, bottom, alpha, r, g, b);
}

void CSprite::Draw(float x, float y, RECT rect, int alpha, int r, int g, int b)
{
	CGame::GetInstance()->Draw(x + dx, y + dy, texture, rect, alpha, r, g, b);
}
void CSprite::DrawWithoutCamera(float x, float y, int alpha, int r, int g, int b)
{
	CGame::GetInstance()->DrawWithoutCamera(x + dx, y + dy, texture, left, top, right, bottom, alpha, r, g, b);
}

void CSprite::DrawWithoutCamera(float x, float y, RECT rect, int alpha, int r, int g, int b)
{
	CGame::GetInstance()->DrawWithoutCamera(x + dx, y + dy, texture, rect, alpha, r, g, b);
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

void CSprites::Clear(vector<int> listId)
{
	for (size_t i = 0; i < listId.size(); i++)
	{
		LPSPRITE s = Get(listId.at(i));

		delete s;
		sprites.erase(listId.at(i));
	}
}

