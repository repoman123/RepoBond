#include "Object2D.h"


Object2D::Object2D(void)
{
	controlCount=0;
	UV = D3DXVECTOR2(0.0f,0.0f);
	Zval = 1.0f;
	rotation=0.0f;
	focused = false;
	hasParent=false;
}
Object2D::~Object2D(void)
{
}

bool Object2D::Intersects(int x,int y)
{
	if(bound.Intersects(x,y))
		return true;
	else
		return false;
}

bool Object2D::Intersects(Object2D* ob)
{
	if(bound.Intersects(&bound))
		return true;
	else
		return false;
}

void Object2D::AddControl(Object2D* ob)
{
	ob->parent=this;
	ob->hasParent = true;
	Controls.insert(pair<int,Object2D*>(controlCount,ob));
	controlCount++;
}

void Object2D::EventCaller(Event ev)
{
	if(hasParent && ev.hasSender)
	{
	parent->EventHandler(ev);
	return;
	}
	if(controlCount>0 && ev.hasSender==false)
	{
	unsigned int i =0;
	do
	{
		if(ev.type==Object2D::mouse_input)
				Controls[i]->focused=false;
		if(ev.info.hasPoint)
		{
			if(Controls[i]->bound.Intersects(ev.info.point.x,ev.info.point.y))
			{
				if(ev.type==Object2D::mouse_input)
					Controls[i]->focused=true;	
				Controls[i]->EventCaller(ev);
				return;
			}
		}
		else
		{
			if(Controls[i]->focused)
				Controls[i]->EventCaller(ev);
			return;
		}
		
		i++;

	}while(i<controlCount);
	}
	else
	{
		EventHandler(ev);
		return;
	}
}

