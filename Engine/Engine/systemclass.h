////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN
//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include "Key.h"
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "Network.h"

#include "graphicsclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "guns.h"
const bool NETWORK_ENABLED=true;
//#define FULL_HD

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	unsigned int windowWidth;
unsigned int windowHeight;
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();
	bool Initialize();
	void Dispose();
	bool Run();
	bool closing;
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	bool Update();
	bool Render();
	void InitializeWindows();
	void DisposeWindows();
	void HandleInput();
	int fps;

	Key key;
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	POINT Cursor;
	GraphicsClass* m_Graphics;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	Network* network;
	TimerClass* m_Timer;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif