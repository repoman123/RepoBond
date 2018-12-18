#include "HUD.h"


HUD::HUD(void)
{
}

HUD::~HUD(void)
{
}

bool HUD::Initialize(ID3D10Device* device,HWND hw,SpriteRenderer* sprend)
{
	dev = device;
	hwnd = hw;
	spba = new SpriteBatch();
	bound.x =0;
	bound.y =0;
	bound.h = 600;
	bound.w = 800;
	rotation=0;
	Zval = 1.0f;
	cursor=new Sprite();
	cursor->Initialize(50,50,50,50);
	cursor->texPath=L"C:/RepoBond/Controls/HUD/Cursor.png";
	cursor->Zval=1.0f;
	spba->Initialize(sprend,cursor);
	//we add them to controls to hook up with events
	spba->UpdateSprites();
	return true;
}

void HUD::Render(SpriteRenderer* sprend)
{
	ShowCursor(true);
	unsigned int i =0;
	spba->Render();
	if(controlCount>0)
	do
	{
		Controls[i]->Render(sprend);
		i++;
	}while(i<controlCount);
	sprend->SetScissorFS();
}

void HUD::EventHandler(Event ev)
{
	if(ev.hasSender)
	{
		//if(ev.sender==exitBut)
		//{
			//PostQuitMessage(1);
			return;
		//}
	}
	if(ev.type == Object2D::mouse_input)
	{
		return;
	};
}