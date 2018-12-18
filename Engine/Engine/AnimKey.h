#ifndef ANIMKEY_H
#define ANIMKEY_H
#include <d3dx10math.h>
class AnimKey
{
public:
	AnimKey(D3DXVECTOR3*,double);
	AnimKey(D3DXQUATERNION*,double);
	AnimKey(float,float,float,double);
	AnimKey(float,float,float,float,double);
	AnimKey(void);
	~AnimKey(void);
	double time;
	D3DXVECTOR3* value;
	D3DXQUATERNION* rotvalue;
};
#endif

