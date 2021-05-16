#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);

	camera = CCamera::GetInstance();
	mapHeight = 0.0f;
	mapWidth = 0.0f;
	//position = -1;

}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_SETTINGS	7

#define OBJECT_TYPE_MARIO	1
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		if (sprite_id!=0) ani->Add(sprite_id, frame_time);
	}
	CAnimations::GetInstance()->Add(ani_id, ani);

	if (atoi(tokens[0].c_str()) < 3000) {
		ani = new CAnimation();
		for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
		{
			int sprite_id = atoi(tokens[i].c_str());
			int frame_time = atoi(tokens[i + 1].c_str());
			if (sprite_id != 0) ani->Add(sprite_id+10000, frame_time);
		}
		CAnimations::GetInstance()->Add(ani_id + 10000, ani);
	}
	
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	
	if (atoi(tokens[0].c_str()) < 30) {
		for (int i = 1; i < tokens.size(); i++)
		{
			int ani_id = atoi(tokens[i].c_str());

			LPANIMATION ani = animations->Get(ani_id + 10000);
			s->push_back(ani);
		}
	}
	
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[2].c_str());
	float x = atof(tokens[3].c_str());
	float y = atof(tokens[4].c_str());

	// int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CGimmick(x,y); 
		player = (CGimmick*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case 50:
		obj = new Ground(x, this->GetMapHeight() - y, atof(tokens[5].c_str()), atof(tokens[6].c_str()));
		break;
	/*case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;*/
	/*case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;*/
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	// obj->SetPosition(x, y);

	/*LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);*/
	objects.push_back(obj);
}
void CPlayScene::_ParseSection_SETTINGS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	if (tokens[0] == "map_width")
		mapWidth = atoi(tokens[1].c_str())*16;
	else if (tokens[0] == "map_height")
		mapHeight = atoi(tokens[1].c_str())*16;
	//else if (tokens[0] == "objects") {
	//	string pathFile = tokens[1];

	//	fstream pFile;
	//	pFile.open(pathFile, fstream::in);
	//	string temp;

	//	while (pFile.good())
	//	{
	//		getline(pFile, temp);

	//		if (temp[0] == '/' && temp[1] == '/') continue;	// skip comment lines
	//		if (temp[0] == '#' || temp == "" || temp[0] == '\t') continue;	// skip comment lines

	//		_ParseSection_OBJECTS(temp);
	//	}

	//	pFile.close();
	//}
	else if (tokens[0] == "title_map") {
		string pathFile = tokens[1];

		int spriteId = atoi(tokens[2].c_str());

		LPSPRITE spriteMap = CSprites::GetInstance()->Get(spriteId);
		tileMap = new TileMap(mapWidth, mapHeight, spriteMap, pathFile);
	}
	else if (tokens[0] == "quadtree") {
		quadTree = new QuadTree();
		quadTree->BuildTree(tokens[1], tokens[2]);
		player = CGimmick::GetInstance();
	}
	else
		DebugOut(L"[ERROR] Unknown scene setting %s\n", ToWSTR(tokens[0]).c_str());

}
bool CPlayScene::_checkInBoundMap()
{
	float l1 = 0, t1 = 0, r1 = mapWidth, b1 = mapHeight, l2, t2, r2, b2;
	player->GetBoundingBox(l2, t2, r2, b2);
	return CGame::IsColliding(l1, t1, r1, b1, l2, t2, r2, b2) && t2 < b1;
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "" || line[0] == '\t') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[SETTINGS]") { 
			section = SCENE_SECTION_SETTINGS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;

			//case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Assets\\Images\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}
void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < quadTree->GetListObject().size(); i++)
	{
		coObjects.push_back(quadTree->GetListObject()[i]);
	}
	/*if (coObjects.size() >= 1) 
		coObjects = coObjects;*/
	CGameObject* o;
	for (int i = 0; i < quadTree->GetListObject().size(); i++)
	{
		o = quadTree->GetListObject()[i];
		if (dynamic_cast<StaticObject*>(o)&& dynamic_cast<MStair*>(o)) {
			dynamic_cast<MStair*>(o)->Update(dt);
		}
		else o->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	//CGame *game = CGame::GetInstance();
	//cx -= game->GetScreenWidth();
	//cy -= game->GetScreenHeight();

	//CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	camera->HandleUpdateFollowPlayer(mapWidth, mapHeight);

}

void CPlayScene::Render()
{
	/*for (int i = 0; i < quadTree->GetListObject().size(); i++)
		quadTree->GetListObject()[i]->Render();*/

	tileMap->Render();
	coObjects = quadTree->GetListObject();

	/*for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (dynamic_cast<Enemy*>(coObjects.at(i))) {
			if (coObjects.at(i)->IsEnable() && !coObjects.at(i)->IsDead()) {
				bool isContain = false;
				for (int j = 0; j < listEnemies.size(); j++)
				{
					if (coObjects.at(i)->GetID() == listEnemies.at(j)->GetID()) {
						isContain = true;
						break;
					}
				}
				if (!isContain) {
					listEnemies.push_back(dynamic_cast<Enemy*>(coObjects.at(i)));
				}
			}
		}
	}

	for (size_t i = 0; i < listEnemies.size(); i++)
	{
		if (listEnemies.at(i)->IsEnable() && !listEnemies.at(i)->IsDead()) {
			bool isContain = false;
			for (size_t j = 0; j < coObjects.size(); j++)
			{
				if (listEnemies.at(i)->GetID() == coObjects.at(j)->GetID()) {
					isContain = true;
					break;
				}
			}
			if (!isContain) {
				coObjects.push_back(listEnemies.at(i));

			}
		}
	}*/

	/*for (size_t i = 0; i < listItems.size(); i++)
	{
		listItems.at(i)->Render();
	}*/

	for (int i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Render();
	}

	//player->Render();
	float x, y;
	player->GetPosition(x, y);
	DebugOut(L"location in PlayScene: %f, %f\n", x, y);

	//scoreBoard->Render();
	
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	coObjects.clear();
	listItems.clear();
	//listEnemies.clear();
	player = nullptr;

	if (tileMap) {
		tileMap->Unload();
		delete tileMap;
		tileMap = nullptr;
	}
	if (quadTree) {
		// quadTree->Unload();
		delete quadTree;
		quadTree = nullptr;
	}
	CScene::Unload();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick*mario = CGimmick::GetInstance();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(GIMMICK_STATE_JUMP);
		break;
	case DIK_A: 
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CGimmick *mario = CGimmick::GetInstance();

	// disable control key when Mario die 
	if (mario->GetState() == GIMMICK_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(GIMMICK_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(GIMMICK_STATE_WALKING_LEFT);
	else
		mario->SetState(GIMMICK_STATE_IDLE);
}