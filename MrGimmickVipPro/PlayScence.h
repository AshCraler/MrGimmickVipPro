#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Gimmick.h"
#include "Camera.h"
#include "TileMap.h"
#include "QuadTree.h"
#include "./ScorePanel/ScorePanel.h"

class IntroScene :public CScene {
public:
	Intro* intro;
	CCamera* camera;

	//CCamera* camera;
	friend class CIntroScenceKeyHandler;

	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
};
class CIntroScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

class MenuScene :public CScene {
public: 
	CMenu* menu;
	CCamera* camera;

	//CCamera* camera;
	friend class CMenuScenceKeyHandler;

	MenuScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
};
class CMenuScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CMenuScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

class CPlayScene: public CScene
{
public: 
	CGimmick *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> listItems;
	//vector<Enemy*> listEnemies;


	vector<int> listIdTextures;
	vector<int> listIdSprites;
	vector<int> listIdAnimations;
	vector<int> listIdAnimationSets;

	TileMap* tileMap = NULL;
	QuadTree* quadTree = NULL;

	float mapWidth, mapHeight;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_SETTINGS(string line);


	bool _checkInBoundMap();

	CCamera* camera;
	int stage;
	int defaultTimeGame;
	int remainTime;
	bool isGameOver;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	int position;

	CGimmick * GetPlayer() { 
		/*if (player == NULL) player = CGimmick::GetInstance();
		return player;*/
		return CGimmick::GetInstance();
	} 

	friend class CPlayScenceKeyHandler;

	float GetMapWidth() { return mapWidth; }
	float GetMapHeight() { return mapHeight; }
	int GetRemainTime() { return remainTime; }
	void SetDefaultTime(int time) { this->defaultTimeGame = time; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

