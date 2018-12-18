#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include "Frustum.h"
#include <d3dx10math.h>


class BoundingBox
{
public:
	BoundingBox(void);
	~BoundingBox(void);

	void fromPoints(D3DXVECTOR3* vs,int numVerts);
	void Transform(D3DXMATRIX* m);
	void Merge(BoundingBox*);
	void Enlarge(float);
	void AxisAligne();

	BoundingBox* Merge(BoundingBox*,BoundingBox*);
	D3DXVECTOR3* GetPoints();

	static D3DXVECTOR3 GetDimensions(BoundingBox*);
	static D3DXVECTOR3 GetOrigDimensions(BoundingBox*);
	static float Volume(BoundingBox*);
	
	static bool IntersectsOrig(BoundingBox*,D3DXVECTOR3*,D3DXVECTOR3*,D3DXVECTOR3*);
	static bool IntersectsOrig(BoundingBox*,BoundingBox*);
	static bool IntersectsOrig(BoundingBox*,float x,float y,float z);
	static bool IntersectsOrig(BoundingBox*,D3DXVECTOR3*);

	static bool Intersects(BoundingBox*,BoundingBox*);
	static bool Intersects(D3DXVECTOR3* min,D3DXVECTOR3* max);
	static bool Intersects(BoundingBox*,Frustum*);
	static bool Intersects(BoundingBox*,D3DXVECTOR3*);
	static bool Intersects(BoundingBox*,float,float,float);

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	D3DXMATRIX lastTransform;
	D3DXVECTOR3 origMin;
	D3DXVECTOR3 origMax;
private:
	bool transformed;
};

#endif
