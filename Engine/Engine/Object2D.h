#ifndef OBJECT2D_H
#define OBJECT2D_H

#include "Bound.h"
#include "SpriteRenderer.h"
#include <d3dx10math.h>
#include "Key.h"
#include <unordered_map>
class Object2D
{
public:
	Object2D(void);
	~Object2D(void);

	enum EventType
	{
		render,
		pos_changed,
		size_changed,
		key_input,
		mouse_input,
	};

	struct EventInfo
	{
		Key key;
		POINT point;
		int delta;
		bool hasPoint;
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	struct Event
	{
		bool hasSender;
		EventType type;
		EventInfo info;
		Object2D* sender;
	};

	Bound bound;
	D3DXVECTOR2 UV;
	float rotation;
	float Zval;
	virtual void Render(SpriteRenderer*)=0;
	virtual void SetPos(int,int)=0;
	virtual void SetSize(int,int)=0;
	virtual void EventHandler(Event ev)=0;
	void EventCaller(Event ev);
	bool Intersects(Object2D*);
	bool Intersects(int,int);
	unordered_map <int,Object2D *> Controls;
	void AddControl(Object2D*);
	unsigned int controlCount;
	bool hasParent;
	Object2D* parent;
	bool focused;
};

#endif
