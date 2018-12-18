#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H

#include "Object2D.h"

class ControlManager
{
public:
	ControlManager(void);
	~ControlManager(void);
	void Render(SpriteRenderer*);
	LRESULT CALLBACK TranslateMessage(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam);
	void EventCaller(Object2D::Event ev);
	unordered_map <int,Object2D *> Controls;
	void AddControl(Object2D*);
	unsigned int controlCount;
};

#endif
