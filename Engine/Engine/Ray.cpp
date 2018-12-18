#include "Ray.h"

Ray::Ray(void)
{
}
Ray::~Ray(void)
{
}

void Ray::Initialize(D3DXVECTOR3* position,D3DXVECTOR3* direction)
{
	origPos = *position;
	origDir = *direction;
	pos = *position;
	dir = *direction;
}

void Ray::Initialize(float px,float py,float pz,float dx,float dy,float dz)
{
	origPos.x = px;
	origPos.y = py;
	origPos.z = pz;
	origDir.x = dx;
	origDir.y = dy;
	origDir.z = dz;
	dir.x = dx;
	dir.y = dy;
	dir.z = dz;
	pos.x = px;
	pos.y = py;
	pos.z = pz;
}

void Ray::SetPosition(float x,float y,float z)
{
	origPos.x = x;
	origPos.y = y;
	origPos.z = z;
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Ray::SetPosition(D3DXVECTOR3* v)
{
	origPos=*v;
	pos=*v;
}

void Ray::SetDirection(float x,float y,float z)
{
	origDir.x=x;
	origDir.y=y;
	origDir.z=z;
	dir.x =x;
	dir.y =y;
	dir.z =z;
	invdir.x = 1.0f / dir.x;
	invdir.x = 1.0f / dir.y;
	invdir.x = 1.0f / dir.z;
}

void Ray::SetDirection(D3DXVECTOR3* v)
{
	origDir=*v;
	dir = *v;
	invdir.x = 1.0f / dir.x;
	invdir.x = 1.0f / dir.y;
	invdir.x = 1.0f / dir.z;
}

void Ray::Transform(D3DXMATRIX* m)
{
	D3DXVec3TransformCoord(&pos,&origPos,m);
	D3DXVec3TransformNormal(&dir,&origDir,m);
	//D3DXVec3Normalize(&dir,&dir);
	invdir.x = 1.0f / dir.x;
	invdir.y = 1.0f / dir.y;
	invdir.z = 1.0f / dir.z;
}

void Ray::TransformInverse(D3DXMATRIX* m)
{
	D3DXMATRIX inv;
	D3DXMatrixInverse(&inv,NULL,m);
	D3DXVec3TransformNormal(&dir,&origDir,&inv);
	D3DXVec3TransformCoord(&pos,&origPos,&inv);
	//D3DXVec3Normalize(&dir,&dir);
	invdir.x = 1.0f / dir.x;
	invdir.y = 1.0f / dir.y;
	invdir.z = 1.0f / dir.z;
}

bool Ray::InterceptsTriangle(Ray* r,D3DXVECTOR3* v1,D3DXVECTOR3* v2,D3DXVECTOR3* v3,float* distance)
{
		//Source: Fast Minimum Storage Ray / Triangle Intersection
        //Reference: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

        //Compute vectors along two edges of the triangle->
        D3DXVECTOR3 edge1, edge2;
        //Edge 1
        edge1.x = v2->x - v1->x;
        edge1.y = v2->y - v1->y;
        edge1.z = v2->z - v1->z;

        //Edge2
        edge2.x = v3->x - v1->x;
        edge2.y = v3->y - v1->y;
        edge2.z = v3->z - v1->z;

        //Cross product of ray direction and edge2 - first part of determinant->
        D3DXVECTOR3 directioncrossedge2;

		D3DXVec3Cross(&directioncrossedge2,&r->dir,&edge2);
        //Compute the determinant->
        float determinant;
        //Dot product of edge1 and the first part of determinant->
        determinant = (edge1.x * directioncrossedge2.x) + (edge1.y * directioncrossedge2.y) + (edge1.z * directioncrossedge2.z);

        //If the ray is parallel to the triangle plane, there is no collision->
        //This also means that we are not culling, the ray may hit both the
        //back and the front of the triangle->
        if (determinant < EPSILON && determinant > -EPSILON)
        {
            *distance = 0.0f;
            return false;
        }

        float inversedeterminant = 1.0f / determinant;

        //Calculate the U parameter of the intersection point->
        D3DXVECTOR3 distanceVector;
        distanceVector.x = r->pos.x - v1->x;
        distanceVector.y = r->pos.y - v1->y;
        distanceVector.z = r->pos.z - v1->z;

        float triangleU;
        triangleU = (distanceVector.x * directioncrossedge2.x) + (distanceVector.y * directioncrossedge2.y) + (distanceVector.z * directioncrossedge2.z);
        triangleU *= inversedeterminant;

        //Make sure it is inside the triangle->
        if (triangleU < 0.0f || triangleU > 1.0f)
        {
            *distance = 0.0f;
            return false;
        }

        //Calculate the V parameter of the intersection point->
        D3DXVECTOR3 distancecrossedge1;
		D3DXVec3Cross(&distancecrossedge1,&distanceVector,&edge1);
        float triangleV;
        triangleV = ((r->dir.x * distancecrossedge1.x) + (r->dir.y * distancecrossedge1.y)) + (r->dir.z * distancecrossedge1.z);
        triangleV *= inversedeterminant;

        //Make sure it is inside the triangle->
        if (triangleV < 0.0f || triangleU + triangleV > 1.0f)
        {
            *distance = 0.0f;
            return false;
        }

        //Compute the distance along the ray to the triangle->
        float raydistance;
        raydistance = D3DXVec3Dot(&edge2,&distancecrossedge1);
		raydistance *= inversedeterminant;
		if(raydistance>0)
		*distance = raydistance;
		else
		{
			*distance = 0.0f;
			return false;
		}
        return true;

}

bool Ray::InterceptsBox(Ray* r,D3DXVECTOR3* min,D3DXVECTOR3* max,float* distance)
{
	//Source: Real-Time Collision Detection by Christer Ericson
    //Reference: Page 179

    *distance = 0.0f;
    float tmax=MAX;

    if (abs(r->dir.x) < EPSILON)
    {
        if (r->pos.x < min->x || r->pos.x > max->x)
        {
            *distance = 0.0f;
            return false;
        }
    }
    else
    {
        float t1 = (min->x - r->pos.x) * r->invdir.x;
        float t2 = (max->x - r->pos.x) * r->invdir.x;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        *distance = std::max(t1, *distance);
        tmax = std::min(t2, tmax);

        if (*distance > tmax)
        {
            *distance = 0.0f;
            return false;
        }
    }

    if (abs(r->dir.y) < EPSILON)
    {
        if (r->pos.y < min->y || r->pos.y > max->y)
        {
            *distance = 0.0f;
            return false;
        }
    }
    else
    {
        float t1 = (min->y - r->pos.y) * r->invdir.y;
        float t2 = (max->y - r->pos.y) * r->invdir.y;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        *distance = std::max(t1, *distance);
        tmax = std::min(t2, tmax);

        if (*distance > tmax)
        {
            *distance = 0.0f;
            return false;
        }
    }

    if (abs(r->dir.z) < EPSILON)
    {
        if (r->pos.z < min->z || r->pos.z > max->z)
        {
            *distance = 0.0f;
            return false;
        }
    }
    else
    {
        float t1 = (min->z - r->pos.z) * r->invdir.z;
        float t2 = (max->z - r->pos.z) * r->invdir.z;

        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
        }

        *distance = std::max(t1, *distance);
        tmax = std::min(t2, tmax);

        if (*distance > tmax)
        {
            *distance = 0.0f;
            return false;
        }
    }
    return true;

}

bool Ray::InterceptsBox(Ray* r,D3DXVECTOR3* min,D3DXVECTOR3* max)
{
	float tx1 = (min->x - r->pos.x)*r->invdir.x;
	float tx2 = (max->x - r->pos.x)*r->invdir.x;
 
	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);
 
	float ty1 = (min->y - r->pos.y)*r->invdir.y;
	float ty2 = (max->y - r->pos.y)*r->invdir.y;
	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));
 
  return tmax >= tmin;
}

bool Ray::InterceptsBox(Ray* r,float minx,float miny,float minz,float maxx,float maxy,float maxz,float* distance)
{
	return InterceptsBox(r,&D3DXVECTOR3(minx,miny,minz),&D3DXVECTOR3(maxx,maxy,maxz),distance);
}

bool Ray::InterceptsSphere(Ray* ray,D3DXVECTOR3* center,float r,float* distance)
{
	//Source: Real-Time Collision Detection by Christer Ericson
    //Reference: Page 177
	//SlimDX
    D3DXVECTOR3 m;
    D3DXVec3Subtract(&m,&ray->pos,center);
    float b = D3DXVec3Dot(&m,&ray->dir);
    float c = D3DXVec3Dot(&m, &m) - (r * r);

    if (c > 0.0f && b > 0.0f)
    {
        *distance = 0.0f;
        return false;
    }

    float discriminant = b * b - c;

    if (discriminant < 0.0f)
    {
        *distance = 0.0f;
        return false;
    }

    *distance = -b - (float)sqrt(discriminant);

    if (*distance < 0.0f)
        *distance = 0.0f;

    return true;

}
