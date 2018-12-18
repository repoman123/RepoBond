#include "eButton.h"


eButton::eButton(void)
{
}

eButton::~eButton(void)
{
}

void eButton::Initialize(int x,int y,int w, int h,const wchar_t* text,SpriteBatch* sp,SpriteRenderer* sprend)
{
	this->sp = sp;
	bound.x =x;
	bound.y=y;
	bound.w = w;
	bound.h=h;
	etext = new eText();
	etext->Initialize(text,(int)bound.x+8,(int)bound.y,sprend);
	etext->SetTextSize((unsigned int)((float)h/1.5f));
	etext->zval = 0.0f;
	etext->UpdateText();

	sprite = new Sprite();
	sprite->Initialize(bound.x,bound.y,bound.w,bound.h);
	sprite->texPath = L"C:/RepoBond/Controls/button.png";
	sprite->Zval = 0.5f;
	sp->AddSprite(sprite);
}

void eButton::EventHandler(Event ev)
{
	if(ev.type == Object2D::mouse_input)
	{
		if(ev.info.msg == WM_LBUTTONDOWN)
		{
			ev.hasSender = true;
			ev.sender = this;
			EventCaller(ev);
			return;
		}
	}
}

void eButton::Render(SpriteRenderer* sprend)
{
	sprend->SetScissorRect(bound.x,bound.y,bound.w,bound.h);
	sprend->textShader->SetTextColor(D3DXVECTOR4(1,1,1,0));
	etext->Render();
}

void eButton::SetPos(int x,int y)
{
	bound.x = x;
	bound.y = y;
}

void eButton::SetSize(int w,int h)
{
	bound.w = w;
	bound.h = h;
}
