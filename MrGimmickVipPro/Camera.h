#pragma once
#include <d3dx9.h>

class CCamera
{
private:
	D3DXVECTOR3 position;
	static CCamera* __instance;
	CCamera(float x, float y);
	bool is_follow_player;
	int width, height;
public:
	static CCamera* GetInstance();
	~CCamera();
	D3DXVECTOR3 GetPositionInCamera(D3DXVECTOR3 position);
	void SetCameraPosition(float x, float y);
	D3DXVECTOR3 GetPosition();
	void SetFollowPlayer(bool isLock) { is_follow_player = isLock; }
	bool IsFollowPlayer() { return is_follow_player; }
	float GetX() { return this->position.x; }
	float GetY() { return this->position.y; }
	virtual void HandleUpdateFollowPlayer(int mapWidth, int mapHeight);
	void SetWidth(int _width) { this->width = _width; }
	void SetHeight(int _height) { this->height = _height; }
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }
	void SetSize(int _width, int _height);
	bool CheckPositionInboundCamera(float x, float y);
};
