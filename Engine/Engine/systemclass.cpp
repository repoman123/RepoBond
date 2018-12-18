////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Graphics = 0;
	m_Timer = 0;
	closing=false;
	network=0;
}
SystemClass::SystemClass(const SystemClass& other)
{
}
SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	bool result;


	// Initialize the windows api.
	InitializeWindows();

	
	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}
	m_Fps->Initialize();

	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}
	m_Cpu->Initialize();
	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}
	// Initialize the graphics object.
	result = m_Graphics->Initialize(windowWidth, windowHeight,m_hwnd,m_Timer,m_Fps,m_Cpu);
	if(!result)
	{
		return false;
	}
	// If the network is enabled then create and initialize the network object.
	if(NETWORK_ENABLED)
	{
		network = new Network;
		if(!network)
		{
			return false;
		}

		result = network->Initialize("192.168.0.192",7000);
		if(!result)
		{
			MessageBox(m_hwnd, L"Could not initialize the network object.", L"Error", MB_OK);
			return false;
		}
	}
	return true;
}

void SystemClass::Dispose()
{
	// If the network was enabled then release the network object.
	if(NETWORK_ENABLED)
	{
		if(network)
		{
			network->Shutdown();
			delete network;
			network = 0;
		}
	}
	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu=0;
	}
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Dispose();
		delete m_Graphics;
		m_Graphics = 0;
	}


	// Dispose the window.
	DisposeWindows();
	
	return;
}

bool SystemClass::Run()
{
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	while(true)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			this->closing=true;
			return false;
		}
	}
}

bool SystemClass::Update()
{
	bool result;
	// Finally render the graphics to the screen.
	result = m_Graphics->Update();
	if(!result)
	{
		return false;
	}

	// If the network is enabled then do frame processing for it.
	if(NETWORK_ENABLED && !m_Graphics->showMenu)
	{
		network->Frame();
		if(m_Graphics->level->player->moved || m_Graphics->level->player->rotated)
		network->SendPositionUpdate(m_Graphics->level->player->pos.x,m_Graphics->level->player->pos.y,m_Graphics->level->player->pos.z
									,m_Graphics->level->player->rotation.x,m_Graphics->level->player->rotation.y,m_Graphics->level->player->rotation.z);
		if(m_Graphics->level->player->shot)
		network->SendShootUpdate(m_Graphics->level->player->shoot.origPos.x,m_Graphics->level->player->shoot.origPos.y,m_Graphics->level->player->shoot.origPos.z,
								 m_Graphics->level->player->shoot.origDir.x,m_Graphics->level->player->shoot.origDir.x,m_Graphics->level->player->shoot.origDir.x,m_Graphics->level->player->gunId);
		if(network->clientAdded)
		{
			Bot* b = new Bot();
			b->Initialize(L"C:/RepoBond/Models/bot.re",m_Graphics->m_D3D->GetDevice(),GetActiveWindow(),m_Graphics->shaders,m_Graphics->light);
			m_Graphics->level->Bots.insert(pair<unsigned int,Bot*>(m_Graphics->level->botCount,b));
			m_Graphics->level->botCount++;
			network->clientAdded=false;
		}
		if(m_Graphics->level->botCount>0)
		{
		unsigned int i=0;
		do
		{
			m_Graphics->level->Bots[i]->pos.x = network->clients[i]->px;
			m_Graphics->level->Bots[i]->pos.y = network->clients[i]->py;
			m_Graphics->level->Bots[i]->pos.z = network->clients[i]->pz;

			m_Graphics->level->Bots[i]->rotation.x = network->clients[i]->rx;
			m_Graphics->level->Bots[i]->rotation.y = network->clients[i]->ry;
			m_Graphics->level->Bots[i]->rotation.z = network->clients[i]->rz;
			i++;
		}while(i<m_Graphics->level->botCount);
		}
		if(network->shot)
		{
			float f;
			if(m_Graphics->level->player->model->Intersects(&network->shoot,&f))
			{
				if(network->gunId == GID_AK47)
					m_Graphics->level->player->health -= AK47_DAMAGE;
				else
				{
					if(network->gunId == GID_GLOCK)
						m_Graphics->level->player->health -= GLOCK_DAMAGE;
					if(m_Graphics->level->player->health==0)
						m_Graphics->showMenu=true;
				}
			}
			int test=0;
		}
	}

	return true;
}

bool SystemClass::Render()
{
	bool result;
	// Finally render the graphics to the screen.
	result = m_Graphics->Render();
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();
	if(!result)
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
		//if graphics not yet initialized return default
	if(m_Graphics!=0)
	{
		if(m_Graphics->menu!=0)
		{
			LRESULT res = m_Graphics->manager->TranslateMessage(hwnd,umsg,wparam,lparam);
			res = m_Graphics->level->player->TranslateMessage(hwnd,umsg,wparam,lparam);
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		else
		{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	else
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void SystemClass::InitializeWindows()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	windowWidth  = GetSystemMetrics(SM_CXSCREEN);
	windowHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)windowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		#ifdef FULL_HD
		windowWidth  = 1920;
		windowHeight = 1080;
		#endif

		#ifndef FULL_HD
		windowWidth  = 800;
		windowHeight = 600;
		#endif
		
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, windowWidth, windowHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::DisposeWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}