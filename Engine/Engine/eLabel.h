#ifndef ELABEL_H
#define ELABEL_H

#include "Object2D.h"
#include "Sprite.h"
#include "eText.h"
#include "SpriteBatch.h"
#include <string>
class eLabel : public Object2D
{
public:
	enum BorderStyle
	{
		none,
		textBorder,
		labelBorder,
		textlabelBorder,
	};
	eLabel(int,int,const wchar_t*,SpriteRenderer*);
	eLabel();
	~eLabel(void);
	void Render(SpriteRenderer*);
	void EventHandler(Event ev){return;};
	void SetPos(int,int);
	void SetSize(int,int);
	void SetText(wchar_t*);
	void SetText(char*);
	void UpdateText();
	void SetTextSize(unsigned int);
	void SetTextColor(float x,float y,float z,float w);
	void SetBackColor(D3DXVECTOR4*);
	void SetBackColor(float,float,float,float);
	BorderStyle style;
	unsigned int BorderWidth;
	unsigned int TextBorderWidth;
	eFont* font;
	wstring* text;
private:
	SpriteBatch* spb;
	Sprite* backsp;
	D3DXVECTOR4 backColor;
	eText* etext;
};

#endif
