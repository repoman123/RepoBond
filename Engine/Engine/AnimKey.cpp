#include "AnimKey.h"

AnimKey::AnimKey()
{
	value=0;
}
AnimKey::AnimKey(D3DXVECTOR3* val,double t)
{
	value=0;
	value = val;
	time = t;
}
AnimKey::AnimKey(D3DXQUATERNION* val,double t)
{
	value=0;
	rotvalue = val;
	time = t;
}
AnimKey::AnimKey(float x,float y,float z,double t)
{
	value =0;
	value = new D3DXVECTOR3(x,y,z);
	time =t;
}
AnimKey::AnimKey(float x,float y,float z,float w,double t)
{
	value =0;
	D3DXQUATERNION q;
	q.x = x;
	q.y = y;
	q.z = z;
	q.w = w;
	rotvalue = &q;
	time =t;
}
AnimKey::~AnimKey(void)
{
	delete value;
}
