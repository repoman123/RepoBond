#include "eLabel.h"

eLabel::eLabel(int x,int y,const wchar_t* t,SpriteRenderer* sprend)
{
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
	BorderWidth =3;
	TextBorderWidth=1;
	bound.x = x;
	bound.y = y;
	font = sprend->font;
	etext = new eText();
	etext->Initialize(t,x,y,sprend);
	bound.h = sprend->GetFontHeight();
	text = &etext->text;
	text->assign(t);
	bound.w = sprend->MeasureString(t);
	
}

eLabel::eLabel()
{
	BorderWidth =1;
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
}

eLabel::~eLabel(void)
{
}

void eLabel::Render(SpriteRenderer* sprend)
{
	etext->X = bound.x;
	etext->Y = bound.y;
	etext->Render();
}

void eLabel::SetPos(int x, int y)
{
	bound.x = x;
	bound.y = y;
}

void eLabel::SetSize(int w,int h)
{
	bound.w = w;
	bound.h = h;
}

void eLabel::UpdateText()
{
	etext->UpdateText();
	bound.w = etext->GetTextLengthInPixels();
}

void eLabel::SetText(wchar_t* chs)
{
	etext->text.assign(chs);
	etext->UpdateText();
	bound.w = etext->GetTextLengthInPixels();
}

void eLabel::SetText(char* chs)
{
	unsigned int len = strlen(chs);
	wchar_t* str = new wchar_t[len+1];
	unsigned int i =0;
	do
	{
		str[i] = (wchar_t)chs[i];
		i++;
	}while(i<len);
	str[len]=0;
	text->assign(str);
	etext->UpdateText();
	bound.h = 100;
	bound.w = etext->GetTextLengthInPixels();
}

void eLabel::SetBackColor(D3DXVECTOR4* c)
{
	backColor = *c;
}

void eLabel::SetTextColor(float x,float y,float z,float w)
{
	etext->textColor.x =x;
	etext->textColor.y =y;
	etext->textColor.z =z;
	etext->textColor.w =w;
}

void eLabel::SetTextSize(unsigned int s)
{
	etext->SetTextSize(s);
}

void eLabel::SetBackColor(float x,float y,float z,float w)
{
	backColor.x = x;
	backColor.y = y;
	backColor.z = z;
	backColor.w = w;
}
