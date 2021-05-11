#pragma once
#include "Camera.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include "GameObject.h"
#include "Node.h"
#include "Ground.h"
using namespace std;
class QuadTree
{
	static QuadTree* instance;
	unordered_map<int, Node*> listNode;
	vector<CGameObject*> listObject;
public:
	QuadTree();
	~QuadTree();

	static QuadTree* GetInstance();
	void BuildTree(string nodeFile, string objectFile);
	void _Load_NODES(string filePath);
	void _Load_OBJECTS(string filePath);

	vector<CGameObject*> GetListObject();
};

