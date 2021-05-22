#pragma once
#include "Scence.h"
#include "Player.h"
#include <unordered_map>
#include <functional>
#include "Gimmick.h"

//using namespace std;

class CSceneManager
{
private:
	CSceneManager();
	static CSceneManager* __instance;
	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	int next_scene;
	CPlayer* player;
	bool _isSwitchScene;
	function<LPSCENE(int, LPCWSTR)> getSceneById;
	function<void(CSceneManager*)> handleSwitchScene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
public:
	void SetPlayer(CPlayer* player);
	CPlayer* GetPlayer() { return CGimmick::GetInstance(); }
	static CSceneManager* GetInstance();
	~CSceneManager();
	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene();
	int GetCurrentSceneId() { return current_scene; }
	int GetNextSceneId() { return next_scene; }
	void SwitchScene(int scene_id);
	void BeforeSwitchScene(int scene_id);
	bool IsSwitchScene() { return _isSwitchScene; };
	void EndSwitchScene() { _isSwitchScene = false; }
	void SetFunctionGetSceneByID(function<LPSCENE(int, LPCWSTR)> getSceneById);
	void SetFunctionHandleSwitchScene(function<void(CSceneManager*)> handleSwitchScene);
	LPSCENE GetScene(int id) { return scenes[id]; }
};
