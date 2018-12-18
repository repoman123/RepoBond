#ifndef ETEXTBOX_H
#define ETEXTBOX_H

#include "Object2D.h"
#include "Sprite.h"

class eTextBox : public Object2D
{
public:
	enum BorderStyle
	{
		none,
		textBorder,
		labelBorder,
		textlabelBorder,
	};
	enum SizeMode
	{
		text,
		fixed_appendText,
	};

	eTextBox(int,int,wchar_t*,SpriteRenderer*);
	eTextBox();
	~eTextBox(void);

	void Render(SpriteRenderer*);
	void SetPos(int,int);
	void SetSize(int,int);
	void KeyDown(Key);
	void KeyUp(Key){return;};
	void KeyPress(Key){return;};
	void LButDown(int,int){return;};
	void RButDown(int,int){return;};
	void MButDown(int,int){return;};
	void LButPress(int,int){return;};
	void MButPress(int,int){return;};
	void RButPress(int,int){return;};
	void LButUp(int,int){return;};
	void RButUp(int,int){return;};
	void MButUp(int,int){return;};
	void MW(int){return;};
	void Move(int,int){return;};

	void SetText(wchar_t*,SpriteRenderer*);
	void SetBackColor(D3DXVECTOR4*);
	void SetBackColor(float,float,float,float);
	BorderStyle style;
	SizeMode sizeMode;
	unsigned int BorderWidth;
	unsigned int TextBorderWidth;
private:
	D3DXVECTOR4 backColor;
	void UpdateString(SpriteRenderer*);
	wstring* Text;
	int fixW;
	bool UpdateStr;
};

#endif
