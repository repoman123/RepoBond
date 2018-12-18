#ifndef eTextBox_H
#define eTextBox_H

#include "Object2D.h"
#include "Sprite.h"
#include "eScrollBar.h"
#include "eText.h"
#include "SpriteBatch.h"

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
	eTextBox(int x,int y,int w,int h,SpriteRenderer*,bool MLine);
	eTextBox();
	~eTextBox(void);
	void Render(SpriteRenderer*);
	void SetPos(int,int);
	void SetSize(int,int);
	void KeyDown(Key);
	void KeyUp(Key){return;};
	void KeyPress(Key){return;};
	void LButDown(int,int);
	void RButDown(int,int){return;};
	void MButDown(int,int){return;};
	void LButPress(int,int){return;};
	void MButPress(int,int){return;};
	void RButPress(int,int){return;};
	void LButUp(int,int){return;};
	void RButUp(int,int){return;};
	void MButUp(int,int){return;};
	void MW(int);
	void Move(int,int){return;};
	void MSG(UINT msg, WPARAM wparam, LPARAM lparam){return;};

	void SetBackColor(D3DXVECTOR4*);
	void SetBackColor(float,float,float,float);
	BorderStyle style;
	unsigned int BorderWidth;
	unsigned int TextBorderWidth;
	wstring** Lines;
	bool MultiLine;
private:
	D3DXVECTOR4 backColor;
	eScrollBar* udscbar;
	eScrollBar* lrscbar;
	SpriteBatch* sb;
	int lineCount;
	int cline;//current line
	int cc;//current character
};

#endif
