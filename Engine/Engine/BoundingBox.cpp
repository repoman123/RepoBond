#include "BoundingBox.h"
BoundingBox::BoundingBox(void)
{
}

BoundingBox::~BoundingBox(void)
{
}

void BoundingBox::Transform(D3DXMATRIX* m)
{
	lastTransform = *m;
	D3DXVec3TransformCoord(&min,&origMin,m);
	D3DXVec3TransformCoord(&max,&origMax,m);
	transformed =true;
}

bool BoundingBox::Intersects(BoundingBox* b,Frustum* f)
{
	D3DXVECTOR3 d = GetDimensions(b);
	if(Frustum::CheckCube(f,b->min.x,b->min.y,b->min.z,d.x,d.y,d.z))
	return true;
	else
	return false;
}

bool BoundingBox::Intersects(BoundingBox* b,BoundingBox* b2)
{
	D3DXVECTOR3 d = b->GetDimensions(b);
	D3DXVECTOR3 d2 = b->GetDimensions(b2);
	if(d>d2)
	{
		if(
		Intersects(b,b2->min.x     ,b2->min.y     ,b2->min.z)	  || Intersects(b,b2->max.x     ,b2->min.y     ,b2->min.z)     ||
		Intersects(b,b2->max.x     ,b2->max.y     ,b2->min.z)     || Intersects(b,b2->min.x     ,b2->max.y     ,b2->min.z)     ||
		Intersects(b,b2->min.x     ,b2->min.y     ,b2->max.z)     || Intersects(b,b2->max.x     ,b2->min.y     ,b2->max.z)     ||
		Intersects(b,b2->max.x     ,b2->max.y     ,b2->max.z)     || Intersects(b,b2->min.x     ,b2->max.y     ,b2->max.z)
		)
		return true;
		else
		return false;
	}
	else
	{
		if(
		Intersects(b2,b->min.x     ,b->min.y     ,b->min.z)	    || Intersects(b2,b->max.x     ,b->min.y     ,b->min.z)     ||
		Intersects(b2,b->max.x     ,b->max.y     ,b->min.z)     || Intersects(b2,b->min.x     ,b->max.y     ,b->min.z)     ||
		Intersects(b2,b->min.x     ,b->min.y     ,b->max.z)     || Intersects(b2,b->max.x     ,b->min.y     ,b->max.z)     ||
		Intersects(b2,b->max.x     ,b->max.y     ,b->max.z)     || Intersects(b2,b->min.x     ,b->max.y     ,b->max.z)
		)
		return true;
		else
		return false;
	}
	return false;
}

bool BoundingBox::IntersectsOrig(BoundingBox* b,BoundingBox* b2)
{
	if(
	IntersectsOrig(b,b2->origMin.x     ,b2->origMin.y     ,b2->origMin.z)	  || IntersectsOrig(b,b2->origMax.x     ,b2->origMin.y     ,b2->origMin.z)     ||
	IntersectsOrig(b,b2->origMax.x     ,b2->origMax.y     ,b2->origMin.z)     || IntersectsOrig(b,b2->origMin.x     ,b2->origMax.y     ,b2->origMin.z)     ||
	IntersectsOrig(b,b2->origMin.x     ,b2->origMin.y     ,b2->origMax.z)     || IntersectsOrig(b,b2->origMax.x     ,b2->origMin.y     ,b2->origMax.z)     ||
	IntersectsOrig(b,b2->origMax.x     ,b2->origMax.y     ,b2->origMax.z)     || IntersectsOrig(b,b2->origMin.x     ,b2->origMax.y     ,b2->origMax.z)
	)
	return true;
	else
	return false;
}

bool BoundingBox::Intersects(BoundingBox* b,D3DXVECTOR3* v)
{
	if((b->max.x >= v->x && b->min.x <= v->x) ||
		(b->max.y >= v->y && b->min.y <= v->y) ||
		(b->max.z >= v->z && b->min.z <= v->z))
		return true;
	else
		return false;
}

bool BoundingBox::Intersects(BoundingBox* b,float x,float y,float z)
{
	if((b->max.x >= x && b->min.x <= x) ||
		(b->max.y >= y && b->min.y <= y) ||
		(b->max.z >= z && b->min.z <= z))
		return true;
	else
		return false;
}

bool BoundingBox::IntersectsOrig(BoundingBox* b,D3DXVECTOR3* t1,D3DXVECTOR3* t2,D3DXVECTOR3* t3)
{
	//we find the min and max
	//which we wil use as a bounding box min and max
	D3DXVECTOR3 vmin;
	vmin.x= min(t1->x,t2->x);
	vmin.y= min(t1->y,t2->y);
	vmin.z= min(t1->z,t2->z);
	
	vmin.x= min(vmin.x,t3->x);
	vmin.y= min(vmin.y,t3->y);
	vmin.z= min(vmin.z,t3->z);

	D3DXVECTOR3 vmax;
	vmax.x= max(t1->x,t2->x);
	vmax.y= max(t1->y,t2->y);
	vmax.z= max(t1->z,t2->z);
	
	vmax.x= max(vmax.x,t3->x);
	vmax.y= max(vmax.y,t3->y);
	vmax.z= max(vmax.z,t3->z);
	BoundingBox b2;
	b2.origMax=vmax;
	b2.origMin=vmin;
	if(IntersectsOrig(b,&b2))
		return true;
	else
		return false;
}

float BoundingBox::Volume(BoundingBox* b)
{
	D3DXVECTOR3 d = GetDimensions(b);
	return d.x*d.y*d.z;
}

bool BoundingBox::IntersectsOrig(BoundingBox* b,float x,float y,float z)
{
	if((b->origMax.x >= x && b->origMin.x <= x) &&
		(b->origMax.y >= y && b->origMin.y <= y) &&
		(b->origMax.z >= z && b->origMin.z <= z))
		return true;
	else
		return false;
}

bool BoundingBox::IntersectsOrig(BoundingBox* b,D3DXVECTOR3* v)
{
	if((b->origMax.x > v->x && b->origMin.x < v->x) &&
		(b->origMax.y > v->y && b->origMin.y < v->y) &&
		(b->origMax.z > v->z && b->origMin.z < v->z))
		return true;
	else
		return false;
}

void BoundingBox::AxisAligne()
{
	min = origMin;
	max = origMax;
}

void BoundingBox::fromPoints(D3DXVECTOR3* vecs,int numVertices)
{
	int i=0;
	origMin*=-1;
	do
	{
		if(origMin.x > vecs[i].x)
			origMin.x = vecs[i].x;
		if(origMin.y > vecs[i].y)
			origMin.y = vecs[i].y;
		if(origMin.z > vecs[i].z)
			origMin.z = vecs[i].z;
		if(origMax.x < vecs[i].x)
			origMax.x = vecs[i].x;
		if(origMax.y < vecs[i].y)
			origMax.y = vecs[i].y;
		if(origMax.z < vecs[i].z)
			origMax.z = vecs[i].z;
		i++;
	}while(i<numVertices);
}

D3DXVECTOR3* BoundingBox::GetPoints()
{
	D3DXVECTOR3* vs = new D3DXVECTOR3[8];

	vs[0].x = min.x;
	vs[0].y = min.y;
	vs[0].z = min.z;

	vs[1].x = max.x;
	vs[1].y = min.y;
	vs[1].z = min.z;

	vs[2].x = max.x;
	vs[2].y = max.y;
	vs[2].z = min.z;
	
	vs[3].x = min.x;
	vs[3].y = max.y;
	vs[3].z = min.z;

	vs[4].x = max.x;
	vs[4].y = min.y;
	vs[4].z = max.z;

	vs[5].x = min.x;
	vs[5].y = min.y;
	vs[5].z = max.z;

	vs[6].x = min.x;
	vs[6].y = max.y;
	vs[6].z = max.z;

	vs[7].x = max.x;
	vs[7].y = max.y;
	vs[7].z = max.z;

	return vs;
}

D3DXVECTOR3 BoundingBox::GetOrigDimensions(BoundingBox* b)
{
	D3DXVECTOR3 v = D3DXVECTOR3(
		abs(b->origMax.x - b->origMin.x),
		abs(b->origMax.y - b->origMin.y),
		abs(b->origMax.z - b->origMin.z));
	return v;
}

D3DXVECTOR3 BoundingBox::GetDimensions(BoundingBox* b)
{
	D3DXVECTOR3 v = D3DXVECTOR3(
		abs(b->max.x - b->min.x),
		abs(b->max.y - b->min.y),
		abs(b->max.z - b->min.z));
	return v;
}

BoundingBox* BoundingBox::Merge(BoundingBox* b1,BoundingBox* b2)
{
	BoundingBox* b = new BoundingBox();
	b->origMin = b1->origMin+b2->origMin;
	b->origMax = b1->origMax+b2->origMax;
	return b;
}

void BoundingBox::Merge(BoundingBox* other)
{
	if(min.x > other->min.x)
		min.x = other->min.x;

	if(min.y > other->min.y)
		min.y = other->min.y;

	if(min.z > other->min.z)
		min.z = other->min.z;

	if(max.x < other->max.x)
		max.x = other->max.x;

	if(max.y < other->max.y)
		max.y = other->max.y;

	if(max.z < other->max.z)
		max.z = other->max.z;
}

void BoundingBox::Enlarge(float f)
{
	origMin.x-=f;
	origMin.y-=f;
	origMin.z-=f;

	origMax.x+=f;
	origMax.y+=f;
	origMax.z+=f;
}
