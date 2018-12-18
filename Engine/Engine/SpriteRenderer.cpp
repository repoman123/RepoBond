#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(void)
{
	shaders=0;
	D3DXMatrixLookAtLH(&view,&D3DXVECTOR3(0.0f,0.0f,-1.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f),&D3DXVECTOR3(0.0f,1.0f,0.0f));
}

SpriteRenderer::~SpriteRenderer(void)
{
}

bool SpriteRenderer::Initialize(ID3D10Device* device, HWND hw,int width,int height,eFont* font,Shaders* sh)
{
	bool result;
	this->device = device;
	this->font = font;
	shaders = sh;
	textShader=new TextShader();
	result = textShader->Initialize(device,hw);
	if(!result)
		return false;
	w=width;
	h=height;
	return true;
}

unsigned int SpriteRenderer::MeasureString(const wchar_t* chs)
{
	unsigned int i =0;
	unsigned int l = wcslen(chs);
	float x=0;
	if(l!=0)
	do
	{
		x+= font->GetCharacter(chs[i])->w;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

unsigned int SpriteRenderer::MeasureString(const char* chs)
{
	unsigned int i =0;
	unsigned int l = strlen(chs);
	float x=0;
	if(l!=0)
	do
	{
		x+=font->GetCharacter((wchar_t)chs[i])->w*fs+ts;
		i++;
	}while(i<l);
	return (int)ceil(x);
}

unsigned int SpriteRenderer::GetFontHeight()
{
	return font->GetHeight();
}

void SpriteRenderer::SetScissorRect(int x,int y,int rw,int rh)
{
	D3D10_RECT rec;
	rec.bottom = y+rh;
	rec.left = x;
	rec.right = x+rw;
	rec.top = y;
	device->RSSetScissorRects(1,&rec);
}

void SpriteRenderer::SetScissorFS()
{
	D3D10_RECT rec;
	rec.bottom = h;
	rec.left = 0;
	rec.right = w;
	rec.top = 0;
	device->RSSetScissorRects(1,&rec);
}

void SpriteRenderer::Begin(D3DXMATRIX* ortho)
{
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	shaders->SetOrtho(ortho);
	shaders->SetView(&view);
	textShader->SetOrtho(ortho);
	textShader->SetView(&view);
}

void SpriteRenderer::End()
{
	//we reset the scissor rect to whole screen
	SetScissorRect(0,0,800,600);
}

void SpriteRenderer::SetFont(eFont* efont)
{
	font = efont;
	textShader->SetTexture(font->Texture);
}

D3DXVECTOR2 SpriteRenderer::ToScreenPos(unsigned int x,unsigned int y,unsigned int tw,unsigned int th)
{
	D3DXVECTOR2 v;
	v.x = (float)(x-((float)w/2));
	v.y = (float)(-((float)y)+(((float)h/2)-th));
	return v;
}
