#include "Player.h"


Player::Player(void)
{
	model=0;
	cam=0;
	up=false;
	down=false;
	left=false;
	right=false;
	rotated=false;
	hud=0;
}

Player::~Player(void)
{
}

bool Player::Initialize(wchar_t* file,ID3D10Device* dev,HWND handle,Shaders* sh,SpriteRenderer* sr,Light* light)
{
	moved=false;
	moveSpeed=2.6f;
	health = maxHealth;
	shot=false;
	bool result = true;
	model=new Model();
	sprend = sr;
	canMove=true;
	move=D3DXVECTOR3(0,0,0);
	ModelLoader* load = new ModelLoader();
	load->Initialize(sh,dev,&handle);
	result = load->Load(file,model);
	if(result==false)
		return false;
	primary = new Gun();
	result = load->Load(L"C:/RepoBond/Models/ak47.re",primary);
	if(result==false)
		return false;
	secondary = new Gun();
	result = load->Load(L"C:/RepoBond/Models/glock.re",secondary);
	if(result==false)
		return false;

	hud = new HUD();
	if(!hud->Initialize(dev,handle,sr))
		return false;
	root = model->GetIndex("ROOT");
	sp1 = model->GetIndex("Spine2");
	hand=model->GetIndex("R_Hand");
	model->root->SetAnimEnd(38,true);
	current = secondary;
	model->objects[hand]->AddChild(current->handle1);
	gunInd = model->objects[hand]->childCount-1;
	SetGun(current);
	
	cam = model->Cameras[0];
	this->mLight = light;
	pos = D3DXVECTOR3(0,0,30);
	dir.dir=D3DXVECTOR3(0,0,-1);
	shaders = sh;
	device = dev;
	rotation.x=0;
	rotation.y=0;
	rotation.z=0;
	model->Update();
	return true;
}

D3DXVECTOR3 Player::GetMovement()
{
	D3DXVECTOR3 v;
	v.x=0;
	v.y=0;
	v.z=0;
	moved=false;
	if(GetAsyncKeyState(VK_T))
	{
		view = TPV;
		cam = model->Cameras[view];
	}
	if(GetAsyncKeyState(VK_F))
	{
		view = Right;
		cam = model->Cameras[view];
	}
	if(GetAsyncKeyState(VK_G))
	{
		view = Left;
		cam = model->Cameras[view];
	}
	if(GetAsyncKeyState(VK_LSHIFT))
	{
		moveSpeed*=3.0f;
		model->animationSpeed=1.8f;
	}
	else
	{
		model->animationSpeed=1.0f;
	}
	if(GetAsyncKeyState(VK_W))
	{
		v.y = moveSpeed/2;
		moved = true;
	}
	if(GetAsyncKeyState(VK_SPACE))
	{
		v.z = 1.0f;
		moved = true;
	}
	if (GetAsyncKeyState(VK_A))
	{
		v.x = moveSpeed/2;
		moved = true;
	}
	if(GetAsyncKeyState(VK_S))
	{
		v.y = -moveSpeed/1.5f;
		moved = true;
	}
	if(GetAsyncKeyState(VK_D))
	{
		v.x = -moveSpeed/2;
		moved = true;
	}
	#ifdef GHOST_MODE
	if(GetAsyncKeyState(VK_Q))
	{
		v.z = moveSpeed;
	}
	if(GetAsyncKeyState(VK_E))
	{
		v.z = -moveSpeed;
	}
	#endif
	return v;
}

void Player::Update(unsigned int fps)
{
	moveSpeed = 10.0f/(float)fps;
	POINT Cursor;
	GetCursorPos(&Cursor);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	int cx = GetSystemMetrics(SM_CXSCREEN)/2;
	int cy = GetSystemMetrics(SM_CYSCREEN)/2;
	int x = Cursor.x - cx;
	int y = Cursor.y - cy;
	SetCursorPos(cx,cy);
	D3DXMATRIX rotX,rotZ;
	float turnSpeed=100;
	float rot = rotation.x + (float)y/turnSpeed;
	if(rot < torad(70) && rot > -torad(80))
	rotation.x += (float)y/turnSpeed;
	rotation.z += (float)x/turnSpeed;
	if(x!=0 || y!=0)
		rotated=true;
	else
		rotated=false;
	D3DXMatrixRotationZ(&rotZ,rotation.z);

#ifndef NO_RAYTRACE
	if(moved)
	{
	move = this->GetMovement();

	if(move.x !=0 || move.y !=0 || move.z !=0)
	{
	lastMove=move;
	model->root->StartAnimation(true,false);
	D3DXVec3TransformCoord(&move, &move,&rotZ);
	D3DXVECTOR3 raydir;
	D3DXVec3Normalize(&raydir,&move);
	pos+=move;
	dir.SetDirection(&raydir);
	dir.SetPosition(pos.x,pos.y,pos.z + 1.0f);
	}
	else
		model->root->StopAnimation(true,false);
	}
	else
	{
		move = this->GetMovement();
		if(move.x !=0 || move.y !=0 || move.z !=0)
		{
		model->root->StartAnimation(true,false);
		D3DXVec3TransformCoord(&move, &move,&rotZ);
		D3DXVECTOR3 raydir;
		D3DXVec3Normalize(&raydir,&move);
		dir.SetDirection(&raydir);
		dir.SetPosition(pos.x,pos.y,pos.z + 1.0f);
		}
		else
			model->root->StopAnimation(true,false);
	}
#endif
#ifdef NO_RAYTRACE
	move = this->GetMovement();
	//model->Update(fps);
	if(move.x !=0 || move.y !=0 || move.z !=0)
	{
	lastMove=move;
	D3DXVec3TransformCoord(&move, &move,&rotZ);
	D3DXVECTOR3 raydir;
	D3DXVec3Normalize(&raydir,&move);
	pos+=move;
	dir.SetDirection(&raydir);
	dir.SetPosition(pos.x,pos.y,pos.z + 1.0f);
	}
#endif

	if(GetAsyncKeyState(VK_1))
	{
		SetGun(primary);
		gunId = GID_GLOCK;
	}
	if(GetAsyncKeyState(VK_2))
	{
		SetGun(secondary);
		gunId =GID_AK47;
	}
	if(GetAsyncKeyState(VK_LBUTTON)!=0)
	{
		current->animationSpeed=0.0f;
		shot=true;
	}
	else
	{
		current->animationSpeed=1.0f;
		model->objects[sp1]->SetAnimStart(current->animShootStart,true);
		model->objects[sp1]->SetAnimEnd(current->animShootEnd,true);
		model->objects[sp1]->StartAnimation(true,false);
		shot=false;
	}
	
	model->objects[root]->Rotate(0,0,rotation.z);
	model->objects[root]->Translate(pos.x,pos.y,pos.z);


	model->objects[sp1]->Rotate(0,0,rotation.x);


	D3DXMATRIX view=cam->GetView();
	D3DXMATRIX proj = cam->GetProjection();
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView,NULL,&view);
	// Calculate the direction of the picking ray in view space.
	//POINT p;
	//GetCursorPos(&p);
	//float px,py;
	//px = ((2.0f * (float)p.x) / (float)800) - 1.0f;
	//py = (((2.0f * (float)p.y) / (float)600) - 1.0f) * -1.0f;
	//px = (float)p.x / (float)proj._11;
	//py = (float)p.y / (float)proj._22;
	//shoot.origDir.x = (px * invView._11) + (py * invView._21) + invView._31;
	//shoot.origDir.y = (px * invView._12) + (py * invView._22) + invView._32;
	//shoot.origDir.z = (px * invView._13) + (py * invView._23) + invView._33;
	//shoot.origPos = cam->GetPosition();
	shoot.SetPosition(&current->dir.origPos);
	shoot.SetDirection(&current->dir.origDir);
	// Normalize the ray direction.
	D3DXVec3Normalize(&shoot.origDir, &shoot.origDir);
	int tste=0;
}

void Player::Render()
{	
	this->renderStarted=true;
	unsigned int n=0;
	D3DXMATRIX viewMatrix, projectionMatrix;
	projectionMatrix = cam->GetProjection();
	viewMatrix = cam->GetView();
	do
	{
		//set mesh to pipeline for rendering
		Mesh::RenderBuffers(model->Meshes[n],device);
		//Render the mesh using the texture shader.
		if(model->Meshes[n]->mat->has_color)
		{
			shaders->ColorRender(device,model->Meshes[n]->GetIndexCount(),Base::GetMatrix(model->Meshes[n]), &viewMatrix, &projectionMatrix,
			mLight->GetDirection(), mLight->GetAmbientColor(),
			mLight->GetDiffuseColor(), cam->GetPosition(), 
			mLight->GetSpecularColor(), mLight->GetSpecularPower(),model->Meshes[n]->mat->color);
		}
		else
		if(model->Meshes[n]->hasBones==false)
		{
			shaders->LightRender(device,model->Meshes[n]->GetIndexCount(),Base::GetMatrix(model->Meshes[n]), &viewMatrix, &projectionMatrix,model->Meshes[n]->mat->diffuseTex,
			mLight->GetDirection(), mLight->GetAmbientColor(),
			mLight->GetDiffuseColor(), cam->GetPosition(), 
			mLight->GetSpecularColor(), mLight->GetSpecularPower());
		}
		else
		{	
			shaders->BoneRender(device,model->Meshes[n]->GetIndexCount(),Base::GetMatrix(model->Meshes[n]), &viewMatrix, &projectionMatrix,model->Meshes[n]->mat->diffuseTex,
			mLight->GetDirection(), mLight->GetAmbientColor(),
			mLight->GetDiffuseColor(), cam->GetPosition(), 
			mLight->GetSpecularColor(), mLight->GetSpecularPower(),model->Meshes[n]->BoneTransforms,model->Meshes[n]->boneCount);
		}
		n++;
	}while(n<(unsigned int)model->GetMeshCount());
	n=0;
	do
	{
		
		//set mesh to pipeline for rendering
		Mesh::RenderBuffers(current->Meshes[n],device);
		//Render the mesh using the texture shader.
		if(current->Meshes[n]->mat->has_color)
		{
			shaders->ColorRender(device,current->Meshes[n]->GetIndexCount(),Base::GetMatrix(current->Meshes[n]), &viewMatrix, &projectionMatrix,
			mLight->GetDirection(), mLight->GetAmbientColor(),
			mLight->GetDiffuseColor(), cam->GetPosition(), 
			mLight->GetSpecularColor(), mLight->GetSpecularPower(),current->Meshes[n]->mat->color);
		}
		else
		{
			shaders->LightRender(device,current->Meshes[n]->GetIndexCount(),Base::GetMatrix(current->Meshes[n]), &viewMatrix, &projectionMatrix,current->Meshes[n]->mat->diffuseTex,
			mLight->GetDirection(), mLight->GetAmbientColor(),
			mLight->GetDiffuseColor(), cam->GetPosition(), 
			mLight->GetSpecularColor(), mLight->GetSpecularPower());
		}
		n++;
	}while(n<current->meshCount);
	hud->Render(sprend);
	this->renderStarted=false;
}

void Player::Jump()
{
	
}

void Player::SetGun(Gun* gun)
{
	//we cannot switch the weapon while we are rendering the gun so we wait till
	//render is finished
	do
	{

	}while(this->renderStarted==true);
	current = gun;
	model->objects[hand]->children.erase(gunInd);
	model->objects[hand]->childCount--;
	model->objects[hand]->AddChild(gun->handle1);
	model->objects[sp1]->SetAnimPos(gun->animShootStart,true);
	model->objects[sp1]->SetAnimStart(gun->animShootStart,true);
	model->objects[sp1]->SetAnimEnd(gun->animShootEnd,true);
}

LRESULT CALLBACK Player::TranslateMessage(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam)
{
	POINT cursor;
	POINT point;
	GetCursorPos(&cursor);
	HWND hw = GetActiveWindow();
	ScreenToClient(hw,&cursor);
	switch(msg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			if(wparam == VK_W)
			{
				up=true;
			}
			if(wparam == VK_A)
			{
				left=true;
			}
			if(wparam == VK_D)
			{
				right=true;
			}
			if(wparam == VK_S)
			{
				down=true;
			}
			if(wparam == VK_SPACE)
			{
				run=true;
			}
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			if(wparam == VK_W)
			{
				up=false;
			}
			if(wparam == VK_A)
			{
				left=false;
			}
			if(wparam == VK_D)
			{
				right=false;
			}
			if(wparam == VK_S)
			{
				down=false;
			}
			if(wparam == VK_SPACE)
			{
				run=false;
			}
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wparam);
			
			return 0;
		}
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}