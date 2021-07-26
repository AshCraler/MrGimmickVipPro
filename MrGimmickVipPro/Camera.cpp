#include "Camera.h"
#include "SceneManager.h"
#include "Utils.h"

CCamera* CCamera::__instance = NULL;

CCamera* CCamera::GetInstance()
{
	if (__instance == NULL) __instance = new CCamera(0, 0);
	return __instance;
}


CCamera::CCamera(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = 0;
	is_follow_player = true;
}


CCamera::~CCamera()
{
}

D3DXVECTOR3 CCamera::GetPositionInCamera(D3DXVECTOR3 position)
{
	D3DXVECTOR3 pos;
	pos.x = position.x - this->position.x;
	pos.y = position.y - this->position.y;
	return D3DXVECTOR3(pos.x, pos.y, 0);
}

void CCamera::SetCameraPosition(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
}

D3DXVECTOR3 CCamera::GetPosition()
{
	return this->position;
}

void CCamera::HandleUpdateFollowPlayer(int mapWidth, int mapHeight)
{
	float player_x, player_y, cx, cy;

	CPlayer* player = CGimmick::GetInstance();;

	player->GetPosition(player_x, player_y);
	//DebugOut(L"location in Cammera: %f, %f\n", player_x, player_y);
	if (dynamic_cast<CGimmick*>(player)->ISLocked()) {
		int xF = dynamic_cast<CGimmick*>(player)->GetViewFrom(), xC = position.x;
		if (mapWidth > width - 15) {
			if (player_x < (width - 15) / 2) {
				cx = 0;
			}
			else if (player_x + (width - 15) / 2 > mapWidth) {
				cx = mapWidth - (width - 15);
			}
			else {
				cx = player_x + (width - 15) / 2 - (width - 15);
			}
		}
		else {
			cx = 0;
		}
		if (cx < xF) {
			cx = xF ;

			if (xC < cx) {
				cx = xC + 2;
			}
			else if (xC > cx) {
				cx = xC - 2;

			}
			else {
				cx = xF;
			}
		}
		else if (cx > xF) {
			//cx = xC - 3;

		}
		else {
			//cx = xF;

		}
		/*if (xC < xF) {
			cx = xC + 3;
		}
		else if (xC > xF) {
			cx = xC - 3;
			
		}
		else {
			cx = xF;
			
		}*/
	}
	else if (dynamic_cast<CGimmick*>(player)->IsLockedTo()) {
		int xF = dynamic_cast<CGimmick*>(player)->GetViewTo(), xC = position.x;
		if (mapWidth > width - 15) {
			if (player_x < (width - 15) / 2) {
				cx = 0;
			}
			else if (player_x + (width - 15) / 2 > mapWidth) {
				cx = mapWidth - (width - 15);
			}
			else {
				cx = player_x + (width - 15) / 2 - (width - 15);
			}
		}
		else {
			cx = 0;
		}

		if (cx +width< xF) {
			//return;
		}
		else if (cx +width> xF) {
			cx = xF - width;
			if (xC < cx) {
				cx = xC + 2;
			}
			else if (xC > cx) {
				cx = xC - 2;

			}
			else {
				cx = xF-width;
			}
			
		}
		else {
			//cx = xC - width;
		}
		
	}
	else {
		if (mapWidth > width - 15) {
			if (player_x < (width - 15) / 2) {
				cx = 0;
			}
			else if (player_x + (width - 15) / 2 > mapWidth) {
				cx = mapWidth - (width - 15);
			}
			else {
				cx = player_x + (width - 15) / 2 - (width - 15);
			}
		}
		else {
			cx = 0;
		}
	}
 	

	if (mapHeight > height)
	{
		if (player_y + player->GetHeight() / 2 < mapHeight - height / 2) {
			cy = player_y - (player->GetHeight() / 2) + (height / 2);
			if (cy < height) {
				cy = height;
			}
		}
		else {
			cy = mapHeight ;
		}
	}
	else {
		cy = mapHeight > height;
	}
	if (cy < 0) cy = 0;
	//DebugOut(L"location in Cammera: %f, %f\n", cx, cy);

	if (is_follow_player) {
		this->SetCameraPosition((int)cx, (int)cy);
	}
	else {
		/*if (player->x <= this->GetX()) {
			player->x = this->GetX();
		}
		if (player->x >= this->GetX() + width - player->GetWidth() - 1) {
			player->x = this->GetX() + width - player->GetWidth() - 1;
		}*/
		this->SetCameraPosition(0, 150);
	}
}

void CCamera::SetSize(int _width, int _height)
{
	this->width = _width; this->height = _height;
}

bool CCamera::CheckPositionInboundCamera(float x, float y)
{
	if (x > position.x + width || x < position.x)
		return false;
	if (y > position.y + height || y < position.y)
		return false;
	return true;
}
