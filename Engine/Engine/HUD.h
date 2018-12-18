#ifndef HUD_H
#define HUD_H

#include "D3D10Class.h"
#include "SpriteRenderer.h"
#include "Object2D.h"
#include "SpriteBatch.h"

class HUD: public Object2D
{
public:
	HUD(void);
	~HUD(void);
	bool Initialize(ID3D10Device*,HWND,SpriteRenderer*);
	void SetPos(int,int){return;};
	void SetSize(int,int){return;};
	void Render(SpriteRenderer*);
	void EventHandler(Event ev);
	SpriteBatch* spba;
	HWND hwnd;
	ID3D10Device* dev;
	Sprite* cursor;
};

#endif
