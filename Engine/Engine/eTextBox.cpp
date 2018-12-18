#include "eTextBox.h"

eTextBox::eTextBox(int x,int y,int w,int h,SpriteRenderer* sprend,bool ml)
{
	sb= new SpriteBatch();
	sb->Initialize(sprend);
	MultiLine = ml;
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
	BorderWidth =3;
	TextBorderWidth=1;
	bound.x = x;
	bound.y = y;
	bound.w = w;
	bound.h =h;
	cline=0;
	cc=0;
	lineCount=1;
	if(MultiLine)
	{
	Lines = new wstring*[100];
	Lines[0]=new wstring();
	}
	else
	{
	Lines=new wstring*[1];
	Lines[0]=new wstring();
	}
	udscbar = new eScrollBar(sprend->device,bound.w-16,0,bound.h,true,sb);
	lrscbar = new eScrollBar(sprend->device,0,bound.h-16,bound.w-16,false,sb);
	//after initializing all child controls we update the sprites in the spritebatch
	sb->UpdateSprites();
	//we add them to controls to hook up with events
	AddControl(udscbar);
	AddControl(lrscbar);
	
}

eTextBox::eTextBox()
{
	BorderWidth =1;
	backColor.x = 1.0f;
	backColor.y = 1.0f;
	backColor.z = 1.0f;
	backColor.w = 1.0f;
}

eTextBox::~eTextBox(void)
{
}

void eTextBox::Render(SpriteRenderer* sprend)
{
	sb->Render();
	/*
	for (int i = 0; i < lineCount; i++)
    {
		if(i==cline)
		{
		Lines[i]->insert(cc,L"|");
		}
		int s = Lines[i]->length();
		int h = sprend->GetFontHeight();
		int ty = i*h;
		int w =0;
		const wchar_t* chs = Lines[i]->data();
		for (int c = 0; c < s; c++)
		{
			//sprend->RenderChar(chs[c],&w,ty);
		}
		if(i==cline)
		Lines[i]->erase(cc,1);
    }
	*/
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
	switch(k.key)
	{
		case VK_UP:
		{
			if(k.hasValue==false && MultiLine)
			{
			if(cline<=lineCount && cline>0)
			cline--;
			int l = Lines[cline]->length();
			if(cc>l)
				cc=l;
			}
			return;
		}
		case VK_DOWN:
		{
			if(k.hasValue==false && MultiLine)
			{
			if(cline<lineCount-1)
			cline++;
			int l = Lines[cline]->length();
			if(cc>l)
				cc=l;
			}
			return;
		}
		case VK_BACK:
		{
			if(k.hasValue==false)
			{
			if(Lines[cline]->length() > 0 && cc>0)
			{
			cc--;
			Lines[cline]->erase(cc,1);
			}
			else
			{
				if(MultiLine)
				{
				if(cline<=lineCount && cline>0)
				{
				Lines[cline]=0;
				lineCount--;
				cline--;
				}
				int l = Lines[cline]->length();
				cc=l;
				}
			}
			}
			return;
		}
		case VK_LEFT:
		{
			if(k.hasValue==false)
			{
			int l = Lines[cline]->length();
			if(l > 0 && cc>0)
			cc--;
			}
			return;
		}
		case VK_RIGHT:
		{
			if(k.hasValue==false)
			{
			int l = Lines[cline]->length();
			if(cc < l)
			cc++;
			}
			return;
		}
		case VK_RETURN:
		{
			if(k.hasValue==false && MultiLine)
			{
			cline++;
			Lines[cline]=new wstring();
			lineCount++;
			cc=0;
			}
			return;
		}
		default:
		{
			if(k.hasValue ==true)
			{
			wchar_t* te = new wchar_t[2];
			te[0]=k.value;
			te[1]=0;
			Lines[cline]->append(te);
			cc++;
			delete te;
			}
			return;
		}
	}
}

void eTextBox::LButDown(int x,int y)
{

}

void eTextBox::MW(int d)
{

}