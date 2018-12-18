#ifndef REGIONS_H
#define REGIONS_H

#include <d3dx10math.h>
#include <map>
#include "Ray.h"
#include "Frustum.h"
#include "BoundingBox.h"
using namespace std ;

class Regions
{
public:

	struct Region
	{
		//coordinates in XYZ
		int x;
		int y;
		int z;
		//lists containing indices to Meshes, Lights,and Cameras
		//within the region
		map<unsigned int,unsigned int> Meshes;
	};

	class Point3D
	{
	public:
		Point3D()
		{

		}
		Point3D(int X,int Y,int Z)
		{
			x=X;
			y=Y;
			z=Z;
		}
		int x;
		int y;
		int z;

		Point3D Point3D::operator-( const Point3D& other )
		{
			return Point3D(x-other.x,y-other.y,z-other.z);
		}
		Point3D Point3D::operator+( const Point3D& other )
		{
			return Point3D(x+other.x,y+other.y,z+other.z);
		}
		Point3D Point3D::operator*( const Point3D& other )
		{
			return Point3D(x*other.x,y*other.y,z*other.z);
		}
		Point3D Point3D::operator*( const int& other )
		{
			return Point3D(x*other,y*other,z*other);
		}
		Point3D Point3D::operator/( const int& other )
		{
			return Point3D(x/other,y/other,z/other);
		}
		Point3D Point3D::operator/( const Point3D& other )
		{
			return Point3D(x/other.x,y/other.y,z/other.z);
		}
	};

	Regions(int x,int y,int z,int h,int w,int d,int regSize);
	~Regions(void);
	bool Intersects(Frustum* f,int x,int y,int z);
	bool Intersects(Ray* r,int x,int y,int z);
	Region* GetRegion(D3DXVECTOR3*);
	Region* GetRegion(Point3D*);
	Region*** rgs;
	Point3D arP;
	int regionSize;
	Point3D ToArCoord(Point3D*);
	Point3D ToArCoord(int x,int y,int z);
	Point3D ToArCoord(D3DXVECTOR3*);
	Region* GetRegions(Point3D min,Point3D max,unsigned int* length);
	void AddMesh(unsigned int,BoundingBox* b);
};

#endif
