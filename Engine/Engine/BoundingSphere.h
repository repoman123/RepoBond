#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H


#include <d3dx10math.h>
#include "Frustum.h"
const float SphereTightness=1.2f;
class BoundingSphere
{
public:
	BoundingSphere(void);
	~BoundingSphere(void);
	float radius;
	D3DXVECTOR3 center;
	bool Intersects(float,float,float);
	bool Intersects(D3DXVECTOR3*);
	bool Intersects(D3DXVECTOR3*,D3DXVECTOR3*);
	void Transform(D3DXMATRIX* m);
	bool Intersects(Frustum*);
	bool Intersects(BoundingSphere*);
	void fromPoints(D3DXVECTOR3* vs,int numVerts);
	float Volume();
	D3DXVECTOR3 origCenter;
};
	
#endif
