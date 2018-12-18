////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#ifdef _WIN32

int threadCount;
SystemClass* System;
bool runUpdate;
bool runRender;

//update thread
void Update(void*)
{
	while(runUpdate)
	{
	System->Update();
	}
}
//render thread
void Render(void*)
{
	while(runRender)
	System->Render();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;
	HANDLE  hUpdateMutex;
	HANDLE  hRenderMutex;
	threadCount=0;
	hUpdateMutex = CreateMutex( NULL, TRUE, NULL );  // Cleared 
    hRenderMutex = CreateMutex( NULL, TRUE, NULL );      // Set 
	runRender=true;
	runUpdate=true;
	// Create the system object.
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if(result)
	{
		threadCount++;
		_beginthread(Update, 0, &threadCount );
		threadCount++;
		_beginthread(Render, 0, &threadCount );
		if(System->Run()==false)
		{
			runRender=false;
			runUpdate=false;
			//we wait so threads stop
			Sleep(100);
			ReleaseMutex(hUpdateMutex);
			threadCount--;
			ReleaseMutex(hRenderMutex);
			threadCount--;
		}
	}
	// All threads done. Clean up handles.
    CloseHandle( hRenderMutex );
    CloseHandle( hUpdateMutex );
	// Shutdown and release the system object.
	System->Dispose();
	delete System;
	System = 0;

	return 0;
}


#endif

#ifdef _MAC_OSX



#endif