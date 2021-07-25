#pragma once
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;
	unordered_map<int, int>	heights;

public: 
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor, int height);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);
	int GetHeight(unsigned int i) { return heights[i]; }

	void Clear();
	void Clear(vector<int> listId);

	static CTextures * GetInstance();
};