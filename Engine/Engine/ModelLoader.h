#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "modelclass.h"

#ifndef D3D11
#include "D3D10Shaders.h"
#endif
#ifdef D3D11
#include "D3D11Shaders.h"
#endif

#include <iostream>
#include <sys/stat.h>
#include <io.h>
#include "dirent.h"
#include <WinSock.h>
#include <Windows.h>
#include "Gun.h"

#ifdef _WIN32
	#define NEWLINE '\n'
#elif defined _MAC_OSX // OS 9
	#define NEWLINE '\r'
#else
	#define NEWLINE '\n' // Mac OS X uses \n
#endif

class ModelLoader
{
public:
	ModelLoader(void);
	~ModelLoader(void);
	void Initialize(Shaders*,ID3D10Device*,HWND*);
	bool Load(wchar_t* path,Model*);
	bool Load(wchar_t* path,Gun*);
	char* buf;
	unsigned long loc;
	map<unsigned int,unsigned int> boneTrInds;
	unsigned int bTrCount;
	char* atofcs;
	HWND* hwnd;
	Mesh* InMesh();
	Camera* InCamera();
	Base* InDummy();
	D3DXMATRIX InMatrix();
	Material* InMaterial();
	Animation* InAnimation();
	D3DXVECTOR3* InVec();
	D3DXVECTOR4* InVec4();
	D3DXQUATERNION* InQuat();
	Bone* InBone();
	Model* mod;
	Shaders* sh;
	ID3D10Device* dev;
private:
	void GetObjects();
	void GetMaterials();
	void InMeshProps(Mesh*);
	void Skip(unsigned int);
	void Skip(unsigned int,char);
	void SkipTo(unsigned int,char);
	float GetFloat();
	double GetDouble();
	int GetInt();
	char Next();
	char Peek();
	char* Get(unsigned int);
	string* Get();
	void Connections();
	bool GoNextParam();
	char* GetParam();
};

#endif
