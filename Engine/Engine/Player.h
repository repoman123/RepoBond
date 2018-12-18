#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "ModelLoader.h"
#include "Light.h"
#include "Key.h"
#include "Ray.h"
#include "Gun.h"
#include "globals.h"
#include "HUD.h"
#include "guns.h"
const int maxHealth = 100;
class Player
{
public:
	enum View
	{
		TPV,
		Left,
		Right,
	};
	Player(void);
	~Player(void);
	bool Initialize(wchar_t* file,ID3D10Device*,HWND,Shaders*,SpriteRenderer* sr,Light* l);
	void Jump();
	void Render();
	void SetGun(Gun*);
	void Update(unsigned int fps);
	bool moved;
	bool rotated;
	bool renderStarted;
	bool shot;
	char gunId;
	int health;
	int armor;
	Gun* current;
	Gun* primary;
	Gun* secondary;
	D3DClass* d3d;
	Camera* cam;
	Shaders* shaders;
	ID3D10Device* device;
	SpriteRenderer* sprend;
	Light* mLight;
	Model* model;
	View view;
	D3DXVECTOR3 pos;
	float moveSpeed;
	bool canMove;
	Ray dir;
	Ray shoot;
	D3DXVECTOR3 GetMovement();
	D3DXVECTOR3 move;
	D3DXVECTOR3 lastMove;
	D3DXMATRIX mat;
	bool up, down,left,right,jump,run;
	unsigned int root,head,arm,sp1,hand,gunInd;
	D3DXVECTOR3 rotation;
	HUD* hud;
	LRESULT CALLBACK TranslateMessage(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam);

private:

};

#endif
