#include "BoundingSphere.h"
BoundingSphere::BoundingSphere(void)
{

}
BoundingSphere::~BoundingSphere(void)
{
}


bool BoundingSphere::Intersects(D3DXVECTOR3* v)
{
	if(Intersects(v->x,v->y,v->z))
		return true;
	else
		return false;
}

bool BoundingSphere::Intersects(float x,float y,float z)
{
	double d = sqrt(pow(center.x-x,2)+pow(center.y-y,2) + pow(center.z-z,2));
	if(radius < d)
	return true;
	else
	return false;
}

bool BoundingSphere::Intersects(Frustum* f)
{
	if(f->CheckSphere(center.x,center.y,center.z,radius))
		return true;
	else
		return false;
}

bool BoundingSphere::Intersects(D3DXVECTOR3* min,D3DXVECTOR3* max)
{
	float dx = max->x - min->x;
	float dy = max->y - min->y;
	float dz = max->z - min->z;
	if(
	Intersects(min->x ,min->y , min->z) ||
	Intersects(min->x+dx , min->y , min->z) ||
	Intersects(min->x ,min->y+dx , min->z+dz) ||
	Intersects(min->x+dx , min->y+dy , min->z) ||
	Intersects(min->x , min->y , min->z) ||
	Intersects(min->x+dx , min->y , min->z) ||
	Intersects(min->x+dx , min->y , min->z) ||
	Intersects(min->x+dx , min->y+dy , min->z))
	return true;
	else
		return false;
}

bool BoundingSphere::Intersects(BoundingSphere* s)
{
	float d =sqrt(pow(center.x-s->center.x,2)+pow(center.y-s->center.y,2)+pow(center.z-s->center.z,2));
	if(radius+s->radius < d)
		return true;
	else
		return false;
}

void BoundingSphere::Transform(D3DXMATRIX* m)
{
	center.x = origCenter.x + m->_41;
	center.y = origCenter.y + m->_42;
	center.z = origCenter.z + m->_43;
}

void BoundingSphere::fromPoints(D3DXVECTOR3* vs,int numVerts)
{
	int i =0;
	radius =0;
	origCenter.x=0;
	origCenter.y=0;
	origCenter.z=0;
	do
	{
		center += vs[i];
		i++;
	}while(i<numVerts);
	origCenter/= (float)numVerts;
	i=0;
	do
	{
        float d =sqrt(pow(origCenter.x-vs[i].x,2)+pow(origCenter.y-vs[i].y,2)+pow(origCenter.z-vs[i].z,2));
		if(radius<d)
			radius=d;
		i++;
	}while(i<numVerts);
}

float BoundingSphere::Volume()
{
	return (float)D3DX_PI * pow(radius,3)*(4/3);
}
