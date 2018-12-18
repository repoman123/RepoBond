#ifndef MENU_H
#define MENU_H


#include "D3D10Class.h"

#include "SpriteRenderer.h"
#include "Object2D.h"
#include "eLabel.h"
#include "eButton.h"
using namespace std;

const unsigned int MAX_OBJS = 10;

class GameMenu : public Object2D
{
public:
	GameMenu(void);
	~GameMenu(void);
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
};

#endif