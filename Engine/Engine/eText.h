#ifndef ETEXT_H
#define ETEXT_H

#include <d3d10.h>
#include <d3dx10math.h>
#include "eFont.h"
#include "SpriteRenderer.h"
#include <string>

class eText
{
public:
	eText(void);
	~eText(void);
	void Render();
	void Initialize(const wchar_t*,int,int,SpriteRenderer*);
	void Initialize(int,int,SpriteRenderer*);
	void Initialize(SpriteRenderer*);
	void RenderBuffers();
	void SetTextSize(unsigned int);
	int X,Y;
	D3DXVECTOR4 textColor;
	SpriteRenderer* sprend;
	bool UpdateText();
	wchar_t* GetText();
	unsigned int GetTextSize();
	unsigned int length;
	unsigned int GetTextLengthInPixels();
	unsigned int MeasureString(const wchar_t* chs);
	unsigned int MeasureString(const char* chs);
	unsigned int MeasureString(const wchar_t* chs,unsigned int fontSize);
	unsigned int MeasureString(const char* chs,unsigned int fontSize);
	float zval;
	eFont font;
	wstring text;
private:
	unsigned int lastlength;
	float fs;
	ID3D10Buffer *instBuf;
	bool InitializeBuffers();
	bool UpdateBuffers();
};

#endif ETEXT_H