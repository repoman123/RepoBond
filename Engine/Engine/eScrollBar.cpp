#include "eScrollBar.h"

eScrollBar::eScrollBar(ID3D10Device* device,int x,int y,int l,bool vert)
{
	child = false;
	sb = new SpriteBatch();
	bound.x = x;
	bound.y = y;
	if(vert)
	{
	bound.w = 16;
	bound.h =l;
	}
	else
	{
		bound.w = l;
		bound.h = 16;
	}
	slider = new Sprite();
	if(vert)
	slider->Initialize(device,L"C:/RepoBond/Controls/UDslider.png",x,y);
	else
	slider->Initialize(device,L"C:/RepoBond/Controls/LRslider.png",x,y);
	vertical =vert;
	sb->AddSprite(slider);
}

eScrollBar::eScrollBar(ID3D10Device* device,int x,int y,int l,bool vert,SpriteBatch* spb)
{
	child = true;
	sb = spb;
	bound.x = x;
	bound.y = y;
	if(vert)
	{
	bound.w = 16;
	bound.h =l;
	}
	else
	{
		bound.w = l;
		bound.h = 16;
	}
	slider = new Sprite();
	if(vert)
	slider->Initialize(device,L"C:/RepoBond/Controls/UDslider.png",x,y);
	else
	slider->Initialize(device,L"C:/RepoBond/Controls/LRslider.png",x,y);
	sb->AddSprite(slider);
	vertical =vert;
}

eScrollBar::eScrollBar()
{
}

eScrollBar::~eScrollBar(void)
{
}

void eScrollBar::Render(SpriteRenderer* sprend)
{
	if(child == false)
		sb->Render();
}

void eScrollBar::SetPos(int x, int y)
{
	bound.x = x;
	bound.y = y;
	slider->bound.x =x;
	slider->bound.y = y;
}

void eScrollBar::SetSize(int w,int h)
{
	bound.w = w;
	bound.h = h;
}

void eScrollBar::LButDown(int x,int y)
{
		if(vertical==false)
		slider->bound.x = x-8;
		else
		slider->bound.y = y-8;
}

void eScrollBar::MW(int d)
{
	if(d>0)
	slider->bound.y += 1;
	else
	slider->bound.y -= 1;
}

