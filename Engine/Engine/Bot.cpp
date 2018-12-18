#include "Bot.h"


Bot::Bot(void)
{
	model=0;
	cam=0;
}

Bot::~Bot(void)
{
}

bool Bot::Initialize(wchar_t* file,ID3D10Device* dev,HWND handle,Shaders* sh,Light* light)
{
	moved=false;
	moveSpeed=2.6f;
	shot=false;
	bool result = true;
	model=new Model();
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

void Bot::Update(unsigned int fps)
{
	if(moved)
	{
		model->root->StartAnimation(true,false);
	}
	else
	{
		model->root->StopAnimation(true,false);
	}

	model->objects[root]->Rotate(0,0,rotation.z);
	model->objects[root]->Translate(pos.x,pos.y,pos.z);
	model->objects[sp1]->Rotate(0,0,rotation.x);
}

void Bot::Render(Camera* cam)
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
	this->renderStarted=false;
}

void Bot::SetGun(Gun* gun)
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

void Bot::Jump()
{
	
}