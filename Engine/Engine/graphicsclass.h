////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

//#define D3D11;


#include "D3D10Class.h"
#include "D3D10Shaders.h"

#include <string>
#include <d3dx10math.h>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "GameMenu.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Light.h"
#include "Frustum.h"
#include "SpriteRenderer.h"
#include "ControlManager.h"
#include "Ray.h"
#include "eFont.h"
#include "Player.h"
#include "timerclass.h"
#include "cpuclass.h"
#include "fpsclass.h"
#include "Bot.h"
#include "Level.h"
#include "globals.h"
#include <iostream>
using namespace std;

/////////////
// GLOBALS //
/////////////
//#define NO_SCENE_RENDER
const bool FULL_SCREEN = false;

const bool VSYNC_ENABLED= false;


#ifdef SHOW_BOUNDS
#include "BoundRenderer.h"
#endif



////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
const float upSkipRatio=0.8f;
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	bool Initialize(int, int, HWND,TimerClass*,FpsClass*,CpuClass*);
	void Dispose();
	bool Update();
	bool showMenu;
	GameMenu* menu;
	Level* level;
	Shaders* shaders;
	ID3D10Device* device;
	bool update;
	D3DClass* m_D3D;
	Light* light;
	ControlManager* manager;
	ModelLoader* loader;
	bool Render();
private:

#ifdef SHOW_BOUNDS 
	BoundRenderer* brend;
#endif
	unsigned int upi;
	TimerClass* timer;
	FpsClass* fps;
	CpuClass* cpu;
	eLabel* fpsLabel;
	Frustum* frustum;
	ID3D10EffectShaderResourceVariable* shaderTextures;
	eFont* font;
	int frame;
	bool hmap;
	SpriteRenderer* sprend;
};
#endif