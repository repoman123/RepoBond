#ifndef BOUND_H
#define BOUND_H

#include <D3DX10math.h>
class Bound
{
public:
	Bound(void);
	Bound(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
	~Bound(void);
	bool Intersects(unsigned int,unsigned int);
	bool Intersects(Bound*);
	D3DXVECTOR2 GetScreenPos(unsigned int,unsigned int);
	unsigned int h,w,x,y;
	
};

#endif
