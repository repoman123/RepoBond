#ifndef BOT_H
#define BOT_H

#include "ModelLoader.h"
#include "Light.h"
#include "Key.h"
#include "Ray.h"
#include "guns.h"

class Bot
{
public:
	//bot skills
	float intelligence;
	float skill;
	float decesiveness;
	Bot(void);
	~Bot(void);
	bool Initialize(wchar_t* file,ID3D10Device*,HWND,Shaders*,Light* l);
	void Jump();
	void Render(Camera* cam);
	void Update(unsigned int fps);
	void SetGun(Gun*);
	bool moved;
	bool renderStarted;
	bool shot;
	char gunId;
	Gun* current;
	Gun* primary;
	Gun* secondary;
	float height;
	D3DClass* d3d;
	Ray shoot;
	Camera* cam;
	Shaders* shaders;
	ID3D10Device* device;
	Light* mLight;
	Model* model;
	D3DXVECTOR3 pos;
	float moveSpeed;
	int turn;
	bool canMove;
	Ray dir;
	D3DXVECTOR3 move;
	D3DXVECTOR3 lastMove;
	D3DXMATRIX mat;
	unsigned int root,head,sp1,hand,gunInd;
	D3DXVECTOR3 rotation;
};

#endif
