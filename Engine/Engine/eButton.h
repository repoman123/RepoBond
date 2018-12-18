#ifndef EBUTTON_H
#define EBUTTON_H

#include "Object2D.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "eText.h"
#include "eLabel.h"

class eButton : public Object2D
{
public:
	eButton(void);
	~eButton(void);
	void Render(SpriteRenderer*);
	void Initialize(int x,int y,int w, int h,const wchar_t* text,SpriteBatch* sp,SpriteRenderer* sprend);
	void SetPos(int,int);
	void SetSize(int,int);
	void EventHandler(Event ev);
	Sprite* sprite;
	eText* etext;
	SpriteBatch* sp;
};

#endif