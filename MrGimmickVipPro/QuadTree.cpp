#include "QuadTree.h"
#include "Utils.h"
#include "Gimmick.h"
#include <algorithm>

using namespace std;
QuadTree* QuadTree::instance = NULL;
QuadTree::QuadTree()
{
}

QuadTree::~QuadTree()
{
}

QuadTree* QuadTree::GetInstance()
{
	if (!instance)
		instance = new QuadTree();
	return instance;
}


void QuadTree::BuildTree(string nodeFile, string objectFile)
{
	_Load_OBJECTS(objectFile);
	_Load_NODES(nodeFile);

	//linking node there
	int id, parent_id, child_id;
	for (auto node : listNode) {
		id = node.first;
		parent_id = id / 10;
		/*if (parent_id == 12) 
			parent_id = 12;*/
		if (listNode.find(parent_id)!=listNode.end()) {
			child_id = id % 10;
			listNode[parent_id]->GetChildren()[child_id] = node.second;
		}
	}
}
void QuadTree::_Load_OBJECTS(string filePath) {
	fstream pFile;
	pFile.open(filePath, fstream::in);
	string line;
	vector<string> tokens;

	CGameObject* o;
	while (pFile.good()) {
		getline(pFile, line);
		if (line[0] == '#' || line == "" || line[0] == '\t') continue;
		tokens = split(line, "\t");
		int i;
		switch (atoi(tokens[2].c_str())) {
		case 1:
			CGimmick::GetInstance()->SetPosition(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));
			listObject.push_back(CGimmick::GetInstance());
			break;
		case 51:
		case 52:
			o = new BigPrize(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[2].c_str()) - 51);
			listObject.push_back(o);
			break;
		case 60:
		case 61:
			o = new Jar(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[2].c_str()) - 60);
			listObject.push_back(o);
			break;
		case 510:
			o = new Hill(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) != 1);
			listObject.push_back(o);
			break;
		case 520:
			o = new Elevator(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) != 2);
			listObject.push_back(o);
			break;
		case 530:
			o = new MStair(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()));
			listObject.push_back(o);
			break;
		case 500:
		default:
			o = new Ground(atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()));
			
			listObject.push_back(o);
		}
	}
	pFile.close();
}
void QuadTree::_Load_NODES(string filePath) {
	fstream pFile;
	pFile.open(filePath, fstream::in);
	string line;
	vector<string> tokens;
	int nObject;
	while (pFile.good()) {
		getline(pFile, line);
		if (line[0] == '#') continue;
		tokens = split(line, "\t");

		Node* node = new Node(atoi(tokens[0].c_str()), 
			atoi(tokens[3].c_str()),
			atoi(tokens[4].c_str()),
			atoi(tokens[1].c_str()),
			atoi(tokens[2].c_str()));

		nObject = tokens.size() - 5;
		for (int i = 0; i < nObject; i++)
		{
			node->Insert(atoi(tokens[5+i].c_str()), 
				listObject[atoi(tokens[5 + i].c_str())]);
			// new CTreeObject(obj->getId(), obj->getBound().getX(), obj->getBound().getY(), obj->getBound().getWidth(), obj->getBound().getHeight(), obj->getDirect()));
		}
		listNode[atoi(tokens[0].c_str())] = node;
	}
	pFile.close();
}
vector<CGameObject*> QuadTree::GetListObject() {
	if (!listNode[1])
		return vector<CGameObject*>();

	Node* nd;
	vector<CGameObject*> objects;
	objects.push_back(CGimmick::GetInstance());
	for (auto n : listNode) {
		nd = (Node*)n.second;
		if (nd->GetChildren()[0] != NULL || !nd->OverlapWithCam()) continue;
		//objects.insert(objects.end(), nd->GetListObject().begin(), nd->GetListObject().end());
		for (auto o : nd->GetListObject()) {
			if (find(objects.begin(), objects.end(), o.second) != objects.end()) continue;
			else objects.push_back(o.second);
		}
	}
	

	//vector<CGameObject*> listObject;
	//unordered_map<int, CGameObject*> list = instance->listNode[1]->GetListObject();

	//for (auto obj : list)
	//{
	//	//Object* object = obj.second;
	//	listObject.push_back(obj.second);
	//}

	return objects;
}
