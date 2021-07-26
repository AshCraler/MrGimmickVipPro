#include "QuadTree.h"
#include "Utils.h"
#include "Gimmick.h"
#include <algorithm>
#include "Enemies/SmallBlackBugWithAntena.h"

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

	getline(pFile, line);
	getline(pFile, line);
	tokens = split(line, "\t");
	int height = atoi(tokens[0].c_str());

	CGameObject* o;
	while (pFile.good()) {
		getline(pFile, line);

		tokens = split(line, "\t");
		int i;
		switch (atoi(tokens[2].c_str())) {
		case 1:
			CGimmick::GetInstance()->SetPosition(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));
			listObject.push_back(CGimmick::GetInstance());
			break;
		case SMALL_BLACK_BUG:
			o = new SmallBlackBug(
				atoi(tokens[2].c_str()),
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[7].c_str()) == 1,
				atoi(tokens[8].c_str())!=-1?dynamic_cast<Jar*>(listObject[atoi(tokens[8].c_str())]):NULL);
			listObject.push_back(o);
			break;
		case JUMPING_COMMAND_BOX:
			o = new JumpingCommandBox(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str())==1);
			listObject.push_back(o);
			break;
		case 901:
			o = new ReturningCommandBox(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1);
			listObject.push_back(o);
			break;
		case 902:
			o = new ReturnOrNotCommandbBox(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1,
				atoi(tokens[8].c_str()) == 1);
			listObject.push_back(o);
			break;
		case 903:
			o = new SelectiveJumpingCommandBox(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1,
				atoi(tokens[8].c_str()));
			listObject.push_back(o);
			break;
		case 904:
			o = new LockingViewPoint(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1,
				atoi(tokens[8].c_str()));
			listObject.push_back(o);
			break;
		case 905:
			o = new LockingViewToPoint(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1,
				atoi(tokens[8].c_str()));
			listObject.push_back(o);
			break;
		case PIPE:
			o = new Pipe(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()));
			listObject.push_back(o);
			break;
		case PIPE_CAN_WALK:
			o = new Pipe(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()),
				true);
			listObject.push_back(o);
			break;
		case PORTAL:
			o = new Portal(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()),
				atoi(tokens[8].c_str()));
			listObject.push_back(o);
			break;
		case HANGING_MACHINE:
			o = new HangingMachine(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case GUN_MACHINE:
			o = new GunMachine(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case HANGING_ELEVATOR:
			o = new HangingElevator(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case 501:
		case 502:
			o = new BigPrize(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[2].c_str()) - 501);
			listObject.push_back(o);
			break;
		case BBA:
			o = new SmallBlackBugWithAntena(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case -200:
			o = new Fish();
			listObject.push_back(o);
			break;
		case WINDOW:
			o = new Window(
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				dynamic_cast<SmallBlackBugWithAntena*>(listObject[atoi(tokens[7].c_str())]));
			listObject.push_back(o);
			break;
		case GBB:
			o = new GiantBlackBug(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[7].c_str()),
				atoi(tokens[8].c_str()));
			listObject.push_back(o);
			break;
		case TURTLE:
			o = new Turtle(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()));
			listObject.push_back(o);
			break;
		case BOAT:
			o = new Boat(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()));
			listObject.push_back(o);
			break;
		case WTS:
			o = new WindowThrowingShell(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case LNB:
			o = new LongNeckedBird(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str())==1
				);
			listObject.push_back(o);
			break;
		case WGM:
			o = new WheelGunMachine(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case SSB:
			o = new StandingStillBug(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case BCB:
			o = new BombCaryingBug(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str())!=0?dynamic_cast<Jar*>(listObject[atoi(tokens[7].c_str())]):NULL);
			listObject.push_back(o);
			break;
		case OBHS:
			o = new OnBugHeadShell(
				dynamic_cast<BombCaryingBug*>(listObject.at(atoi(tokens[3].c_str()))));
			listObject.push_back(o);
			break;
		case GFS:
			o = new GreenFatStuff(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case BS:
			o = new BrownStuff(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case BM:
			o = new BlackMan(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()));
			listObject.push_back(o);
			break;
		case 60:
		case 61:
		case 62:
		case 63:
			o = new Jar(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[2].c_str()) - 60,
				atoi(tokens[5].c_str()) == 1);
			listObject.push_back(o);
			break;
		case 510:
			o = new Hill(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) == 1);
			listObject.push_back(o);
			break;
		case 520:
			o = new Elevator(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str())  -13,
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str()) != 2);
			listObject.push_back(o);
			break;
		case PINKWORM:
			o = new PinkWorm(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()));
			listObject.push_back(o);
			break;
		case 530:
			o = new MStair(
				atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
				atoi(tokens[5].c_str()),
				atoi(tokens[6].c_str()),
				atoi(tokens[7].c_str())==1,
				atoi(tokens[8].c_str())==1);
			listObject.push_back(o);
			break;
		case 500:

		default:
			o = new Ground(atoi(tokens[3].c_str()),
				height - atoi(tokens[4].c_str()),
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

		Node* node;
		if (listNode.size() != 0 && listNode[1] != NULL) {
			int height = listNode[1]->GetHeight();
			node = new Node(atoi(tokens[0].c_str()),
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[1].c_str()),
				height-atoi(tokens[2].c_str()));

		}
		else {
			node = new Node(atoi(tokens[0].c_str()),
				atoi(tokens[3].c_str()),
				atoi(tokens[4].c_str()),
				atoi(tokens[1].c_str()),
				atoi(tokens[2].c_str()));
		}
		
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
