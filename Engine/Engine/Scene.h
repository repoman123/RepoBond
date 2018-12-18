#ifndef SCENE_H
#define SCENE_H

#include "modelclass.h"
#include <map>
#include "Ray.h"
#include "Shaders.h"

class Scene
{
public:
	Scene(void);
	~Scene(void);
	map<unsigned int,Model*> Models;
	void Initialize(const char* path);
	float* heightX;
	float* heightY;
	int Width;
	int Depth;
	void GenerateHeightMap();
};

#endif