#pragma once
#include"GameObject.h"
class Node
{
	int id;
	Node* children[4];
	unordered_map<int, LPGAMEOBJECT> listObj;
	int width, height, x, y;
public:
	Node();
	Node(int id, int w, int h, int x, int y);
	~Node();
	void Insert(int id, LPGAMEOBJECT obj);
	unordered_map<int, LPGAMEOBJECT> GetListObject();
	void Unload();
	Node** GetChildren();
	bool OverlapWithCam();
};

class Node;
typedef Node* LPNODE;