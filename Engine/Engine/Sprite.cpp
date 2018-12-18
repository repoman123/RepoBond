////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Sprite.h"


Sprite::Sprite()
{

}
Sprite::~Sprite()
{
}

bool Sprite::Initialize(int x,int y,int width, int height)
{
	bound.w = width;
	bound.h = height;
	bound.x = x;
	bound.y = y;
	UV = D3DXVECTOR2(0,0);
	Zval=0;
	rotation=0;
	return true;
}

bool Sprite::Initialize(int x, int y)
{
	bound.x = x;
	bound.y = y;
	UV = D3DXVECTOR2(0,0);
	Zval=0;
	rotation=0;
	return true;
}

void Sprite::RotateDeg(float f)
{
	rotation = (180.0f / (float)D3DX_PI)*f;
}

void Sprite::SetPos(int x, int y)
{
	bound.x =x;
	bound.y =y;
}

void Sprite::SetSize(int w,int h)
{
	bound.w = w;
	bound.h =h;
}

void Sprite::SetColor(float x,float y,float z,float w)
{
	color.x = x;
	color.y = y;
	color.z = z;
	color.w = w;
}