#ifndef MODEL_H
#define MODEL_H


#include "d3d10class.h"
#include "D3D10Shaders.h"

#include "textureclass.h"
#include <cstring>
#include "Mesh.h"
#include "Ray.h"
#include <unordered_map>
#include "Base.h"
#include "Camera.h"
#include "HeightMap.h"
#include "globals.h"

class Model
{
public:
	Model();
    Model(const Model&);
	~Model();
	bool CreateHeightMap();
	void AddMaterial(Material*);
	Base* root;
	int GetIndex(const char* name);
	unordered_map <unsigned int,Base*> objects;
	unordered_map <unsigned int,Mesh*> Meshes;
	unordered_map <unsigned int,Animation*> Animations;
	unordered_map <unsigned int,Camera*> Cameras;
	unsigned int AnimCount;
	unsigned int objCount;
	unsigned int camCount;
	unsigned int meshCount;
	float animationSpeed;
	int GetMeshCount();
	map <unsigned int,Material*> Materials;
	//Material* LoadMaterial(ID3D10Device*,aiMaterial*,HWND,Shaders*);
	//Animation* LoadAnim(aiAnimation**,const wchar_t* name,unsigned int numAnims);
	unsigned int MatCount;
	bool Intersects(Ray*,float*);
	bool Intersects(Ray*,float*,unsigned int* mIndex);
	bool Intersects(Ray*,float,float*);
	void UpdateAnimation(unsigned int fps);
	void Update();
	HeightMap* hmap;
	wstring filePath;
	wstring filename;
private:
	bool fileExists(const string& filename);
};

#endif