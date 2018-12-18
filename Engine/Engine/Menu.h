#ifndef MENU_H
#define MENU_H

#include "d3d10class.h"
#include "SpriteRenderer.h"
#include "Object2D.h"
#include "eLabel.h"
#include "eButton.h"
using namespace std;

class Menu : public Object2D
{
public:
	Menu(void);
	~Menu(void);
	bool Initialize(D3DClass*,HWND,SpriteRenderer*);
	void SetPos(int,int){return;};
	void SetSize(int,int){return;};
	void Render(SpriteRenderer*);
	void EventHandler(Event ev);
private:
	HWND hwnd;
	D3DClass* d3d;
	SpriteBatch* spba;
	Sprite* background;
	eButton* exitBut;
	eButton* convBut;
};

#endif