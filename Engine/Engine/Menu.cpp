#include "Menu.h"


Menu::Menu(void)
{
}
Menu::~Menu(void)
{
}

bool Menu::Initialize(D3DClass* d,HWND hw,SpriteRenderer* sprend)
{
	d3d = d;
	hwnd = hw;
	spba = new SpriteBatch();
	bound.x =0;
	bound.y =0;
	bound.h = 600;
	bound.w = 800;
	rotation=0;
	Zval = 1.0f;
	background=new Sprite();
	background->Initialize(0,0,800,600);
	background->texPath=L"C:/RepoBond/Controls/Menu/background.png";
	background->Zval = 1.0f;
	spba->Initialize(sprend,background);

	exitBut = new eButton();
	exitBut->Initialize(0,30,100,30,L"Exit",spba,sprend);
	AddControl(exitBut);

	convBut = new eButton();
	convBut->Initialize(0,120,100,30,L"Convert",spba,sprend);
	AddControl(convBut);
	
	eLabel* text = new eLabel(0,80,sprend);
	eLabel* text2 = new eLabel(0,100,sprend);
	text->SetTextSize(14);
	text->SetTextColor(1,1,1,1);
	text2->SetTextSize(14);
	text2->SetTextColor(1,1,1,1);
	text->SetText(L"Place files to be converted to C:/RepoBond/Models/Conversions");
	text2->SetText(L"the files will be outputted to RepoBond Models");
	AddControl(text);
	AddControl(text2);
	//we add them to controls to hook up with events
	spba->UpdateSprites();
	sprend->shader->texs->UpdateTextureArray(sprend->device);
	return true;
}

void Menu::Render(SpriteRenderer* sprend)
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

void Menu::EventHandler(Event ev)
{
	if(ev.hasSender)
	{
		if(ev.sender==exitBut)
		{
			PostQuitMessage(1);
			return;
		}
		if(ev.sender == convBut)
		{
			
		}
	}
	if(ev.type == Object2D::mouse_input)
	{
		return;
	};
}