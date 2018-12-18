#include "Regions.h"


Regions::Regions(int X,int Y,int Z,int dimX,int dimY,int dimZ,int regSize)
{
	// begin memory allocation
	rgs = new Regions::Region**[dimX];
	for(int x = 0; x < dimX; ++x) 
	{
		rgs[x] = new Regions::Region*[dimY];
		for(int y = 0; y < dimY; ++y) 
		{
			rgs[x][y] = new Regions::Region[dimZ];
		}
	}
	for(int x = 0; x < dimX; ++x) 
	{
		for(int y = 0; y < dimY; ++y) 
		{
			for(int z = 0; z < dimZ; ++z) 
			{
				rgs[x][y][z].x = (X+x)*regSize;
				rgs[x][y][z].y = (Y+y)*regSize;
				rgs[x][y][z].z = (Z+z)*regSize;
			}
		}
	}
	arP=Point3D(X,Y,Z);
	this->regionSize = regSize;
}

Regions::~Regions(void)
{
}

bool Regions::Intercepts(Ray* r,int x,int y,int z)
{
	return false;
}

bool Regions::Intercepts(Frustum* f,int x,int y,int z)
{
	return f->CheckCube((float)rgs[x][y][z].x,(float)rgs[x][y][z].y,(float)rgs[x][y][z].z,
						(float)rgs[x][y][z].x+regionSize,(float)rgs[x][y][z].y+regionSize,(float)rgs[x][y][z].z+regionSize);
}

void Regions::AddMesh(unsigned int index,BoundingBox* b)
{
	unsigned int l=0;
	Point3D min = ToArCoord(&b->min);
	Point3D max = ToArCoord(&b->max);
	Region minr = rgs[min.x][min.y][min.z];
	Region maxr = rgs[max.x][max.y][max.z];
	Region* r = GetRegions(min,max,&l);
	r->Meshes.insert(pair<unsigned int,unsigned int>(r->Meshes.size(),index));
}

Regions::Region* Regions::GetRegion(D3DXVECTOR3* p)
{
	Point3D pi = ToArCoord((int)floor(p->x+0.5f),(int)floor(p->y+0.5f),(int)floor((int)p->z+0.5f));
	return &rgs[pi.x][pi.y][pi.z];
}

Regions::Region* Regions::GetRegion(Point3D* p)
{
	Point3D pi = ToArCoord(p);
	return &rgs[pi.x][pi.y][pi.z];
}

Regions::Region* Regions::GetRegions(Point3D min,Point3D max,unsigned int* length)
{
	Point3D pd = max-min;
	pd.x+=1;
	pd.y+=1;
	pd.z+=1;
	Region* regs = new Region[(pd.x+1)*(pd.y+1)*(pd.z+1)];
	unsigned int i=0;
	unsigned int x = 0;
	unsigned int y =0;
	unsigned int z = 0;
	do
	{
		x++;
	}while(x<(unsigned int)pd.x);
	(*length)=(pd.x+1)*(pd.y+1)*(pd.z+1);
	return regs;
}

Regions::Point3D Regions::ToArCoord(Point3D* p)
{
	Point3D p1 = (*p)/regionSize;
	Point3D p2 = p1-arP;
	return p2;
}

Regions::Point3D Regions::ToArCoord(int x,int y,int z)
{
	Point3D p = Point3D(x,y,z);
	Point3D p1 = p/regionSize;
	Point3D p2 = p1-arP;
	return p2;
}

Regions::Point3D Regions::ToArCoord(D3DXVECTOR3* p)
{
	Point3D p1 = Point3D((int)floor(p->x+0.5f),(int)floor(p->y+0.5f),(int)floor(p->z+0.5f));
	return ToArCoord(&p1);
}