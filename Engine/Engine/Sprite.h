
////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef SPRITE_H
#define SPRITE_H

#include <d3dx10math.h>
#include "textureclass.h"
#include "Bound.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	bool Initialize(int x, int y);
	bool Initialize(int x,int y,int width, int height);
	void RotateDeg(float);
	Bound bound;
	wchar_t* texPath;
	float Zval;
	float rotation;
	bool hasTexture;
	unsigned int texIndex;
	D3DXVECTOR2 UV;
	void SetPos(int,int);
	void SetSize(int,int);
	void SetColor(float,float,float,float);
	D3DXVECTOR4 color;
	bool useColor;
};

#endif