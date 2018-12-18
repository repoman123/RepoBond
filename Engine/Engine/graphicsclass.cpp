////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#define degtorad(degree) ((float)(D3DX_PI / 180.0f) * (degree)) // converts from degrees to radians
#define radtodeg(radian) ((float)(180.0f / D3DX_PI) * (radian)) // converts from radians to degrees
//macros

GraphicsClass::GraphicsClass()
{
	//all have to be initialized as
	//zero so we can see if they have been 
	//initialized in the dispose method
	m_D3D = 0;
	level=0;
	frustum=0;
	light=0;
	fpsLabel=0;
	menu=0;
	frame=0;
	showMenu=false;
}
GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd,TimerClass* tim,FpsClass* fc,CpuClass* cc)
{
	timer = tim;
	fps = fc;
	cpu = cc;
	bool result;
	// Create the Direct3D object.
	cout << "Initializing D3D10";
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		cout << "Initializing D3D10 Failed";
		return false;
	}
	
	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN,0.1f,1000);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	device = m_D3D->GetDevice();
	//initialize shaders
	shaders = new Shaders();
	result = shaders->Initialize(m_D3D->GetDevice(),hwnd);
	if(!result)
		return false;

	//init 2D
	font = new eFont();
	result = font->Initialize(m_D3D->GetDevice(),"C:/RepoBond/Fonts/Times New Roman.txt",90,36);
	if(!result)
		return false;
	sprend =0;
	sprend = new SpriteRenderer();
	result = sprend->Initialize(m_D3D->GetDevice(),hwnd,screenWidth,screenHeight,font,shaders);
	if(!result)
		return false;
	sprend->SetFont(font);
	manager = new ControlManager();
	menu = new GameMenu();
	manager->AddControl(menu);
	
	menu->Initialize(m_D3D,hwnd,sprend);
	fpsLabel = new eLabel(0,0,L"",sprend);
	fpsLabel->SetTextSize(20);
	fpsLabel->SetTextColor(0.8f,1.0f,1.0f,1.0f);

	loader = new ModelLoader();
	// Initialize the level->model object.
	cout << "Initializing Model";
	loader->Initialize(this->shaders,m_D3D->GetDevice(),&hwnd);

	level = new Level();
	//level->InitializeMultiplayer();
	level->Initialize(loader);
	if(!level)
		return false;

// Initialize the light object.
	light = new Light();
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(1.0f, 1.0f, -1.0f);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecularPower(32.0f);


	
	
	level->player = new Player();
	cout << "Initializing Player";
	level->player->Initialize(L"C:/RepoBond/Models/man.re",m_D3D->GetDevice(),hwnd,shaders,sprend,light);
	if(!level->player)
	{
		cout << "Initializing Player Failed";
		return false;
	}
	//initialize the frustum object
	frustum = new Frustum();
	if(!frustum)
		return false;
	

	if(shaders->texs->UpdateTextureArray(m_D3D->GetDevice())!=S_OK)
	return false;
	int i=0;
	i=0;
	#ifdef SHOW_BOUNDS
	brend = new BoundRenderer();
	brend->Initialize(m_D3D->GetDevice());
	//brend->AddBound(&level->model->Meshes[2]->quad,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
	//brend->AddBound(&level->model->Meshes[0]->quad,D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f));
	brend->AddBound(&level->player->dir,D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f));
	brend->AddBound(&level->player->shoot,D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f));
	brend->AddBound(&level->down,D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f));
	//brend->AddBound(&level->player->current->dir,D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f));
	//brend->AddBound(&level->model->Meshes[2]->quad);
	brend->UpdateRays();
	brend->UpdateBoxs();
	#endif
	
	return true;
}

void GraphicsClass::Dispose()
{
	// Release the light object.
	if(light)
	{
		delete light;
		light = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Dispose();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::Update()
{
	update=true;
	level->model->Update();
	level->UpdatePlayers(fps->GetFps());
	level->UpdateBots(fps->GetFps());
	if(showMenu==false)
	{
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			showMenu=true;
		}
	}
	else
		if(GetAsyncKeyState(VK_ESCAPE))
		{
		showMenu=false;
		}
	if(GetAsyncKeyState(VK_F1))
	{
	this->m_D3D->TogleFullScreen();
	}
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.4f,0.4f, 1.0f, 1.0f);
	sprend->SetScissorFS();
	level->model->UpdateAnimation(fps->GetFps());
	level->player->model->UpdateAnimation(fps->GetFps());
	level->player->current->UpdateAnimation(fps->GetFps());
	if(update)
	{
		brend->UpdateRays();
	level->model->Update();
	level->player->current->Update();
	level->player->model->Update();
	level->player->cam->Render();
	unsigned int i=0;
	if(level->botCount>0)
	do
	{
		level->Bots[i]->model->Update();
		i++;
	}while(i<level->botCount);
	}
	update=false;
	if(showMenu==false)
	{
	#ifdef SHOW_BOUNDS
	brend->UpdateRays();
	#endif
	level->player->Render();
	unsigned int n =0;
	#ifndef NO_SCENE_RENDER
	viewMatrix= level->player->cam->GetView();
	projectionMatrix=level->player->cam->GetProjection();
	//we get the device for rendering
	// Construct the frustum.
	unsigned int mr =0;
	do
	{
		mr++;
		//set mesh to pipeline for rendering
		Mesh::RenderBuffers(level->model->Meshes[n],device);
		//Render the mesh using the texture shader.
		if(level->model->Meshes[n]->mat->has_color)
		{
			shaders->ColorRender(device,level->model->Meshes[n]->GetIndexCount(),Base::GetMatrix(level->model->Meshes[n]), &viewMatrix, &projectionMatrix,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(),level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower(),level->model->Meshes[n]->mat->color);
		}
		else
		if(level->model->Meshes[n]->hasBones==false)
		{
			shaders->LightRender(device,level->model->Meshes[n]->GetIndexCount(),Base::GetMatrix(level->model->Meshes[n]), &viewMatrix, &projectionMatrix,level->model->Meshes[n]->mat->diffuseTex,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(), level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower());
		}
		else
		{	
			shaders->BoneRender(device,level->model->Meshes[n]->GetIndexCount(),Base::GetMatrix(level->model->Meshes[n]), &viewMatrix, &projectionMatrix,level->model->Meshes[n]->mat->diffuseTex,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(), level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower(),level->model->Meshes[n]->BoneTransforms,level->model->Meshes[n]->boneCount);
		}
		n++;
	}while(n<level->model->meshCount);

	mr =0;
	n=0;
	do
	{
		mr++;
		//set mesh to pipeline for rendering
		Mesh::RenderBuffers(level->target.Meshes[n],device);
		//Render the mesh using the texture shader.

		if(level->target.Meshes[n]->mat->has_color)
		{
			shaders->ColorRender(device,level->target.Meshes[n]->GetIndexCount(),Base::GetMatrix(level->target.Meshes[n]), &viewMatrix, &projectionMatrix,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(),level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower(),level->target.Meshes[n]->mat->color);
		}
		else
		if(level->target.Meshes[n]->hasBones==false)
		{
			shaders->LightRender(device,level->target.Meshes[n]->GetIndexCount(),Base::GetMatrix(level->target.Meshes[n]), &viewMatrix, &projectionMatrix,level->target.Meshes[n]->mat->diffuseTex,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(), level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower());
		}
		else
		{	
			shaders->BoneRender(device,level->target.Meshes[n]->GetIndexCount(),Base::GetMatrix(level->target.Meshes[n]), &viewMatrix, &projectionMatrix,level->target.Meshes[n]->mat->diffuseTex,
			light->GetDirection(), light->GetAmbientColor(),
			light->GetDiffuseColor(), level->player->cam->GetPosition(), 
			light->GetSpecularColor(), light->GetSpecularPower(),level->target.Meshes[n]->BoneTransforms,level->target.Meshes[n]->boneCount);
		}
	
		n++;
	}while(n<level->target.meshCount);

#endif
	n=0;
	if(level->botCount>0)
	do
	{
		level->Bots[n]->Render(level->player->cam);
		n++;
	}while(n<level->botCount);
	wchar_t* str= 0;
	str = new wchar_t[32];
	int fp = fps->GetFps();
	int cp = cpu->GetCpuPercentage();
	wsprintf(str,L"%i",fp);
	fpsLabel->text->assign(L"FPS: ");
	fpsLabel->text->append(str);
	str= 0;
	fpsLabel->text->append(L" Position:");

	str= 0;
	str = new wchar_t[64];
	wsprintf(str,L"%i",(int)level->player->pos.x);
	fpsLabel->text->append(L" X= ");
	fpsLabel->text->append(str);

	str= 0;
	str = new wchar_t[64];
	wsprintf(str,L"%i",(int)level->player->pos.y);
	fpsLabel->text->append(L" Y= ");
	fpsLabel->text->append(str);

	str= 0;
	str = new wchar_t[64];
	wsprintf(str,L"%i",(int)level->player->pos.z);
	fpsLabel->text->append(L" Z= ");
	fpsLabel->text->append(str);
	if(hmap==false)
		fpsLabel->text->append(L" HMap=false ");
	else
		fpsLabel->text->append(L" HMap=true ");
	fpsLabel->UpdateText();
	delete str;
	#ifdef SHOW_BOUNDS
	this->brend->Render(this->shaders,&viewMatrix,&projectionMatrix);
	#endif
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->TurnZBufferOff();
	
	sprend->Begin(&orthoMatrix);

	fpsLabel->Render(sprend);
	sprend->End();
	m_D3D->TurnZBufferOn();
	
	}
	else
	{
	m_D3D->GetOrthoMatrix(orthoMatrix);
	sprend->Begin(&orthoMatrix);
	m_D3D->TurnZBufferOff();
	HRESULT res;
	res= shaders->texs->UpdateTextureArray(m_D3D->GetDevice());
	shaders->texturesPtr->SetResource(shaders->texs->ppSRV);
	menu->Render(sprend);
	fpsLabel->Render(sprend);
	m_D3D->TurnZBufferOn();
	sprend->End();
	};
	m_D3D->EndScene();
	return true;
}