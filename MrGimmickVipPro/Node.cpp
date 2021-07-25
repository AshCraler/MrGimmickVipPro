#include "Node.h"
#include "Camera.h"

Node::Node()
{
	listObj.clear();
	for (int i = 0; i < 4; i++)
		children[i] = NULL;
}

Node::Node(int id, int w, int h, int x, int y) {
	this->id = id;
	this->width = w;
	this->height = h;
	this->x = x;
	this->y = y;
}
Node::~Node()
{

}

void Node::Insert(int id, LPGAMEOBJECT object)
{
	listObj[id] = object;
}

void Node::Unload()
{
	//for (int i = 0; i < listObj.size(); i++)
	//	delete listObj[i];
	listObj.clear();
}
Node** Node::GetChildren()
{
	return children;
}
unordered_map<int, CGameObject*> Node::GetListObject() {
	unordered_map<int, CGameObject*> list_object;
	list_object.clear();

	if (!this->children[0])
		return this->listObj;
	else {
		for (int i = 0; i < 4; i++)
		{
			if (this->children[i]->OverlapWithCam())
			{
				unordered_map<int, CGameObject*> list_object_in_childs = children[i]->GetListObject();
				for (auto o : list_object_in_childs) {
					list_object[o.first] = o.second;
				}
			}
		}
	}
	return list_object;

}
bool Node::OverlapWithCam() {
	CCamera* cam = CCamera::GetInstance();
	if (id == 133 && cam->GetY()>230)
		id = 133;
	bool overlap1 = (this->x > (cam->GetX() + cam->GetWidth()));
	bool overlap2 = (this->x + this->width) < cam->GetX();
	bool overlap3 = this->y < (cam->GetY() - cam->GetHeight());
	bool overlap4 = (this->y - this->height) > cam->GetY();
	bool overlap = !(overlap1 || overlap2 || overlap3 || overlap4);
	if (overlap == true)
		overlap = overlap;
	/*bool overlap =
		(x >= cam->GetX() && x <= cam->GetX() + cam->GetWidth() && y >= cam->GetY()  && y <= cam->GetY() + cam->GetHeight()) ||
		(x + width >= cam->GetX() && x + width <= cam->GetX() + cam->GetWidth() && y >= cam->GetY() && y <= cam->GetY() + cam->GetHeight()) ||
		(x + width >= cam->GetX() && x + width <= cam->GetX() + cam->GetWidth() && y + height >= cam->GetY() && y + height <= cam->GetY() + cam->GetHeight()) ||
		(x >= cam->GetX() && x <= cam->GetX() + cam->GetWidth() && y + height >= cam->GetY() && y + height <= cam->GetY() + cam->GetHeight());*/

	/*bool overlap =
		(cam->GetX() >= x&& cam->GetX()<=x+width && cam->GetY()>=y&&cam->GetY()<=y+height)||
		(cam->GetX() + cam->GetWidth() >= x && cam->GetX() + cam->GetWidth() <= x + width && cam->GetY() >= y && cam->GetY() <= y + height) ||
		(cam->GetX() + cam->GetWidth() >= x && cam->GetX() + cam->GetWidth() <= x + width && cam->GetY() + cam->GetHeight() >= y && cam->GetY() + cam->GetHeight() <= y + height) ||
		(cam->GetX() >= x && cam->GetX() <= x + width && cam->GetY() + cam->GetHeight() >= y && cam->GetY() + cam->GetHeight() <= y + height) ||
*/

	/*if (overlap == true)
		overlap = true;*/
	return overlap;
}
