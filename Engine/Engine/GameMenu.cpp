#include "GameMenu.h"

GameMenu::GameMenu(void)
{
}
GameMenu::~GameMenu(void)
{
}

bool GameMenu::Initialize(D3DClass* d,HWND hw,SpriteRenderer* sprend)
{
	d3d = d;
	hwnd = hw;
	spba = new SpriteBatch();
	bound.x =0;
	bound.y =0;
	bound.h = sprend->h;
	bound.w = sprend->w;
	rotation=0;
	Zval = 1.0f;
	background=new Sprite();
	background->Initialize(0,0,sprend->w,sprend->h);
	background->texPath=L"C:/RepoBond/Controls/Menu/background.png";
	background->Zval = 1.0f;
	spba->Initialize(sprend,background);
	exitBut = new eButton();
	exitBut->Initialize(0,30,100,30,L"Exit",spba,sprend);
	AddControl(exitBut);
	//we add them to controls to hook up with events
	spba->UpdateSprites();
	sprend->shaders->texs->UpdateTextureArray(d->GetDevice());
	return true;
}

void GameMenu::Render(SpriteRenderer* sprend)
{
	ShowCursor(true);
	unsigned int i =0;
	sprend->SetScissorFS();
	spba->Render();
	if(controlCount>0)
	do
	{
		Controls[i]->Render(sprend);
		i++;
	}while(i<controlCount);
}

void GameMenu::EventHandler(Event ev)
{
	if(ev.hasSender)
	{
		if(ev.sender==exitBut)
		{
			PostQuitMessage(1);
			return;
		}
	}
	if(ev.type == Object2D::mouse_input)
	{
		return;
	};
}