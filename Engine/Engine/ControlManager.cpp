#include "ControlManager.h"


ControlManager::ControlManager(void)
{
	controlCount=0;
}

ControlManager::~ControlManager(void)
{
}

void ControlManager::Render(SpriteRenderer* sprend)
{
	unsigned int i =0;
	if(controlCount!=0)
	do
	{
		sprend->SetScissorRect(Controls[i]->bound.x,Controls[i]->bound.y,Controls[i]->bound.w,Controls[i]->bound.h);
		Controls[i]->Render(sprend);
		i++;
	}while(i<controlCount);
}

LRESULT CALLBACK ControlManager::TranslateMessage(HWND hwnd,UINT msg, WPARAM wparam, LPARAM lparam)
{
	POINT point;
	POINT cursor;
	GetCursorPos(&cursor);
	HWND hw = GetActiveWindow();
	ScreenToClient(hw,&cursor);
	Object2D::Event ev;
	ev.info.wparam = wparam;
	ev.info.lparam = lparam;
	ev.info.msg = msg;
	ev.hasSender=false;
	switch(msg)
	{
		// Check if a key has been pressed on the keyboard.
		
		case WM_KEYDOWN:
		{
			ev.type = Object2D::key_input;
			ev.info.hasPoint = false;
			// If a key is pressed send it to the input object so it can record that state.
			ev.info.key.key = (unsigned int)wparam;
			ev.info.key.hasValue=false;
			EventCaller(ev);
			return 0;
		}

		case WM_KEYUP:
		{
			ev.type = Object2D::key_input;
			ev.info.hasPoint = false;
			// If a key is pressed send it to the input object so it can record that state.
			ev.info.key.key = (unsigned int)wparam;
			ev.info.key.hasValue=false;
			EventCaller(ev);
			return 0;
		}

		case WM_CHAR:
		{
			ev.info.hasPoint=false;
			ev.type = Object2D::key_input;
			ev.info.key.value = (wchar_t)wparam;
			ev.info.key.hasValue=true;
			EventCaller(ev);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_MBUTTONDOWN:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_MBUTTONUP:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			ev.info.hasPoint=true;
			ev.type = Object2D::mouse_input;
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.point = point;
			EventCaller(ev);
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			point.x = LOWORD(lparam);
			point.y = HIWORD(lparam);
			ev.info.hasPoint = true;
			ev.info.point = point;
			ev.info.delta = GET_WHEEL_DELTA_WPARAM(wparam);
			EventCaller(ev);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}

void ControlManager::EventCaller(Object2D::Event ev)
{
	if(controlCount>0)
	{
	unsigned int i =0;
	do
	{
		if(ev.info.hasPoint)
		{
			if(ev.type==Object2D::mouse_input)
				Controls[i]->focused=false;
		if(Controls[i]->bound.Intersects(ev.info.point.x,ev.info.point.y))
		{
			if(ev.type==Object2D::mouse_input)
				Controls[i]->focused=true;
			Controls[i]->EventCaller(ev);
		}
		}
		else
		{
			if(Controls[i]->focused == true)
				Controls[i]->EventCaller(ev);
		}
		i++;
	}while(i<controlCount);
	}
}

void ControlManager::AddControl(Object2D* ob)
{
	Controls.insert(pair<int,Object2D*>(controlCount,ob));
	controlCount++;
}