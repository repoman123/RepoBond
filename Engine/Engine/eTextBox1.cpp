#include "eTextBox.h"

eTextBox::eTextBox(int x,int y,wchar_t* t,SpriteRenderer* sprend)
{
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
	Text = new wstring();
	SetText(t,sprend);
	BorderWidth =3;
	TextBorderWidth=1;
	sizeMode = this->text;
	bound.x = x;
	UpdateStr=false;
	bound.y = y;
}

eTextBox::eTextBox()
{
	BorderWidth =1;
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
	UpdateStr=false;
}

eTextBox::~eTextBox(void)
{
}

void eTextBox::Render(SpriteRenderer* sprend)
{
	if(UpdateStr)
		UpdateString(sprend);
	D3DXVECTOR4 mv = D3DXVECTOR4(0.5f,0.5f,0.5f,0.0f);
	if(style == textlabelBorder || style == labelBorder)
	sprend->Render(bound.x-BorderWidth-TextBorderWidth,bound.y-BorderWidth-TextBorderWidth,
		bound.w+BorderWidth*2+TextBorderWidth*2,bound.h+BorderWidth*2+TextBorderWidth*2,&(backColor-mv));
	if(style == textlabelBorder || style == textBorder)
	sprend->Render(bound.x-TextBorderWidth,bound.y-TextBorderWidth,
		bound.w+TextBorderWidth*2,bound.h+TextBorderWidth*2,&(backColor));
	else
		sprend->Render(bound.x,bound.y,bound.w,bound.h,&(backColor));
	if(Text->length()>0)
	sprend->RenderText(Text->data(),bound.x,bound.y);
}

void eTextBox::SetPos(int x, int y)
{
	bound.x = x;
	bound.y = y;
}

void eTextBox::SetSize(int w,int h)
{
	bound.w = w;
	bound.h = h;
	if(sizeMode == this->fixed_appendText)
		fixW = w;
}

void eTextBox::SetText(wchar_t* chs,SpriteRenderer* sprend)
{
	Text->assign(chs);
	bound.h = sprend->GetFontHeight();
	int s = sprend->MeasureString(chs);
	if(sizeMode == this->fixed_appendText)
		if(s>bound.w)
		bound.w = s;
		else
		bound.w = fixW;
	if(sizeMode == this->text)
	bound.w = s;
}

void eTextBox::UpdateString(SpriteRenderer* sprend)
{
	unsigned int s = Text->size();
	wchar_t *t = new wchar_t[s+1];
	wcscpy(t,Text->c_str());
	t[Text->size()]=0;
	int l = sprend->MeasureString(t);
	if(sizeMode == this->fixed_appendText)
		if(l>bound.w)
		bound.w = l;
		if(l<bound.w && l>fixW)
		bound.w = l;
	if(sizeMode == this->text)
	bound.w = l;
	UpdateStr=false;
}

void eTextBox::SetBackColor(D3DXVECTOR4* c)
{
	backColor = *c;
}

void eTextBox::SetBackColor(float x,float y,float z,float w)
{
	backColor.x = x;
	backColor.y = y;
	backColor.z = z;
	backColor.w = w;
}

void eTextBox::KeyDown(Key k)
{
	if(k.key == VK_BACK)
	{
		Text->erase(Text->length()-1,1);
		UpdateStr=true;
	}
	else
	{
	wchar_t* te = new wchar_t[2];
	te[0]=k.value;
	te[1]=0;
	Text->append(te);
	UpdateStr=true;
	delete te;
	}
}