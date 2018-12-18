#include "Level.h"


Level::Level(void)
{
	botCount=0;
	playerCount=0;
	model=0;
	down.SetDirection(0,0,-1);
}

Level::~Level(void)
{
}

bool Level::Initialize(ModelLoader* loader)
{
	down.SetDirection(0,0,-1);
	ModelLoader load;
	multiplayer=false;
	bool result;
	model=new Model();
	result = loader->Load(L"C:/RepoBond/Models/test.re",model);
	if(!result)
	{
		return false;
	}
	model->CreateHeightMap();
	result = loader->Load(L"C:/RepoBond/Models/target.re",&target);
	target.Meshes[0]->Translate(20,20,20);
	target.Meshes[1]->Translate(20,20,20);
	return true;
}

void Level::UpdateBots(unsigned int fps)
{
	unsigned int i=0;
	if(botCount>0)
	do
	{
		Bots[i]->Update(fps);
		i++;
	}while(i<botCount);
}

void Level::UpdatePlayers(unsigned int fps)
{
		UpdatePlayersS(fps);
}

void Level::UpdatePlayersS(unsigned int fps)
{
	unsigned int i=0;
	bool hmap=true;
#ifndef NO_RAYTRACE
	float d;
	float d2;
	down.SetPosition(player->pos.x,player->pos.y,player->pos.z+1.8f);
	bool inter=model->Intersects(&down,&d);
	if(GetAsyncKeyState(VK_LBUTTON)!=0)
	{
	float ds;
	unsigned int mIndex;
	if(model->Intersects(&player->shoot,&ds,&mIndex))
	{
		model->Meshes[mIndex]->mat->has_color=true;
		model->Meshes[mIndex]->mat->color=D3DXVECTOR4(((float)GetRand(0,100))/100,((float)GetRand(0,100))/100,((float)GetRand(0,100))/100,1.0f);
		int test=0;
	}
	}
	float sd;
	if(model->Intersects(&player->shoot,&sd))
		int test=0;
	if(inter)
	{
		player->pos.z += (1.8f-d);
		hmap = false;
	}
	bool inter2=model->Intersects(&player->dir,&d2);
	if(inter2)
	{
		if(d2>0.5f)
		{
			player->moved=true;
			player->canMove=true;
		}
		else
		{
			player->moved=false;
			player->canMove=false;
		}
	}
	else
	{
		player->moved=true;
	}
#endif
#ifndef GHOST_MODE
	if(hmap == true)
	{
	float h = model->hmap->GetHeight(&player->pos);
	player->pos.z = h+2.8f;
	}
#endif
	player->Update(fps);
}

unsigned int Level::GetRand(unsigned int a,unsigned int b)
{
	std::uniform_int_distribution<int> distribution(a,b);
	return distribution(gen);
}

bool Level::IsMultiplayer()
{
	return multiplayer;
}