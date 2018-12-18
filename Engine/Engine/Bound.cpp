#include "Bound.h"


Bound::Bound(void)
{
}
Bound::Bound(unsigned int x,unsigned int y,unsigned int w,unsigned int h)
{
	this->x =x;
	this->y=y;
	this->h=h;
	this->w =w;
}
Bound::~Bound(void)
{
}

bool Bound::Intersects(unsigned int px,unsigned int py)
{
	if(px>x && px<x+w && py>y && py<y+h)
		return true;
	else
	return false;
}

bool Bound::Intersects(Bound* b)
{
	if(b->x>x && b->x<x+w && b->y>y && b->y<y+h)
		return true;
	else
	return false;
}

D3DXVECTOR2 Bound::GetScreenPos(unsigned int sw,unsigned int sh)
{	
	D3DXVECTOR2 v;
	v.x = (float)(x-((float)sw/2));
	v.y = -(float)x+((float)sh/2)-h;
	return v;
}
