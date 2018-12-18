#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "D3D10Shaders.h"
#include "eFont.h"
#include "textshader.h"

class SpriteRenderer
{
public:
	struct VertexType
	{
		D3DXVECTOR2 position;
	    D3DXVECTOR2 texture;
	};

	struct SpriteInstance
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 texcoord;
		D3DXVECTOR2 size;
	};

	struct TextInstance
	{
		D3DXVECTOR3 location;
		D3DXVECTOR2 charSize;
		D3DXVECTOR2 UV;
	};
	SpriteRenderer(void);
	~SpriteRenderer(void);
	unsigned int MeasureString(const wchar_t*);
	unsigned int MeasureString(const char*);
	//void Render(int x,int y,int w, int h,ID3D10ShaderResourceView*);
	//void Render(int x,int y,int w, int h,float angle,ID3D10ShaderResourceView*);
	//void Render(int x,int y,int w, int h,D3DXVECTOR4*);
	//void Render(int x,int y,int w, int h,float angle,D3DXVECTOR4*);
	void SetScissorRect(int x,int y,int w,int h);
	void SetScissorFS();
	void Begin(D3DXMATRIX* ortho);
	void End();
	ID3D10Device* device;
	void SetFont(eFont*);
	bool Initialize(ID3D10Device*, HWND,int,int,eFont*,Shaders*);
	unsigned int GetFontHeight();
	int w;//width
	int h;//height
	int ts;//textspacing
	Shaders* shaders;
	TextShader* textShader;
	eFont* font;
	D3DXVECTOR2 ToScreenPos(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
private:
	D3DXMATRIX view;
	float fs;
};

#endif
