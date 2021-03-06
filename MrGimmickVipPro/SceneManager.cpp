#include "SceneManager.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include <iostream>
#include <fstream>
#include "Game.h"

using namespace std;

CSceneManager* CSceneManager::__instance = NULL;

CSceneManager::CSceneManager()
{
}

void CSceneManager::SetPlayer(CPlayer* player)
{
	this->player = player;
}

CSceneManager* CSceneManager::GetInstance()
{
	if (!__instance)
		__instance = new CSceneManager();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

//belongs to CGame,
void CSceneManager::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

//belongs to CGame, parse scene listed in line to scenes
void CSceneManager::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);

	//LPSCENE scene = new CPlayScene(id, path);
	scenes[id] = this->getSceneById(id, path);
}

/*
	Load game campaign file and load/initiate first scene
*/
void CSceneManager::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "") continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}

void CSceneManager::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();;

	CTextures::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();
}

LPSCENE CSceneManager::GetCurrentScene()
{
	//DebugOut(L"[INFO] scenes.size game %d\n", scenes.size());
	//DebugOut(L"[INFO] current_scene game %d\n", current_scene);
	return scenes[current_scene];
}

void CSceneManager::BeforeSwitchScene(int scene_id)
{
	next_scene = scene_id;
	_isSwitchScene = true;
}

void CSceneManager::SetFunctionGetSceneByID(function<LPSCENE(int, LPCWSTR)> getSceneById)
{
	this->getSceneById = getSceneById;
}

void CSceneManager::SetFunctionHandleSwitchScene(function<void(CSceneManager*)> handleSwitchScene)
{
	this->handleSwitchScene = handleSwitchScene;
}

//void CSceneManager::SwitchScene()
//{
//
//	DebugOut(L"[INFO] Switching to scene %d\n", next_scene);
//
//
//	scenes[current_scene]->Unload();
//
//
//	_isSwitchScene = false;
//
//	this->handleSwitchScene(this);
//
//	//if (next_scene < 1) {
//	//	CGame::GetInstance()->SetDeviationY(0);
//	//}
//	//else {
//
//	//	CGame::GetInstance()->SetDeviationY(45);
//	//	CPlayScene* s = dynamic_cast<CPlayScene*>(scenes[next_scene]);
//	//	if (current_scene > 0) {
//	//		CPlayScene* oldScene = dynamic_cast<CPlayScene*>(scenes[current_scene]);
//	//		s->SetDefaultTime(oldScene->GetRemainTime());
//	//	}
//	//	s->position = -1;
//	//}
//
//	CGame::GetInstance()->SetKeyHandler(scenes[next_scene]->GetKeyEventHandler());
//	current_scene = next_scene;
//	scenes[next_scene]->Load();
//}