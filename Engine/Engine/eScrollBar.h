#ifndef eScrollBar_H
#define eScrollBar_H

#include "Object2D.h"
#include "SpriteBatch.h"
#include "Sprite.h"

class eScrollBar : public Object2D
{
public:
	eScrollBar(ID3D10Device* device,int x,int y,int l,bool vertical,SpriteBatch*);
	eScrollBar(ID3D10Device* device,int x,int y,int l,bool vertical);
	eScrollBar();
	~eScrollBar(void);
	void Render(SpriteRenderer*);
	void SetPos(int,int);
	void SetSize(int,int);
	void KeyDown(Key){return;};
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
private:
	SpriteBatch* sb;
	bool child;
	Sprite* slider;
	bool vertical;
};

#endif
