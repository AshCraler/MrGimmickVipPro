#pragma once
#include "Camera.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include "GameObject.h"
#include "Node.h"
#include "DynamicObjects/HangingMachine.h"
#include "DynamicObjects/GunMachine.h"
#include "DynamicObjects/HangingElevator.h"
#include "DynamicObjects/Boat.h"
#include "DynamicObjects/WindowThrowingShell.h"
#include "DynamicObjects/WheelGunMachine.h"

#include "./StaticObjects/Ground.h"
#include "./StaticObjects/Elevator.h"
#include "./StaticObjects/Hill.h"
#include "./StaticObjects/MStair.h"
#include "./StaticObjects/Pipe.h"
#include "./StaticObjects/Portal.h"
#include "StaticObjects/Window.h"
#include "./Items/BigPrize.h"
#include "./Items/Jar.h"

#include "./Enemies//SmallBlackBug.h"
#include "./Enemies/GreenFatStuff.h"
#include "./Enemies/LongNeckedBird.h"
#include "./Enemies/Turtle.h"
#include "./Enemies//Utility/JumpingCommandBox.h"
#include "./Enemies/Bosses/GiantBlackBug.h"
#include "./Enemies/Bosses/BrownStuff.h"
#include "./Enemies/Bosses/BlackMan.h"
#include "./Enemies/PinkWorm.h"
#define WINDOW	-1
#define SMALL_BLACK_BUG	18
#define HANGING_MACHINE	50
#define GUN_MACHINE	51
#define HANGING_ELEVATOR 55
#define JUMPING_COMMAND_BOX	900
#define BBA	19
#define GBB	10
#define TURTLE	23
#define BOAT	54
#define WTS	-15
#define LNB	24
#define WGM	53
#define SSB	-20
#define BCB	-21
#define OBHS	-22
#define GFS	25
#define BS	12
#define BM	14
#define PIPE	-10
#define PIPE_CAN_WALK	-9
#define PORTAL	-11
#define PINKWORM	22
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

	//vector<Node*> GetNodeListWithCamera();
	vector<CGameObject*> GetListObject();
};

