#include "modelclass.h"
Model::Model()
{
	meshCount =0;
	MatCount=0;
	AnimCount=0;
	objCount=0;
	camCount=0;
	animationSpeed=1;
}

Model::Model(const Model& other)
{
}
Model::~Model(void)
{
}

int Model::GetMeshCount()
{

	return meshCount;
}

struct Connection
{
	wchar_t* name;
	wchar_t* parent;
};

bool Model::Intersects(Ray* r,float* d)
{
#ifndef NO_RAYTRACE
	*d = 1.0737418e+008f;
	unsigned int i=0;
	bool intercepts=false;
	float di=1.0737418e+008f;
	do
	{
		if(Mesh::Intersects(Meshes[i],r,&di))
		{
		if(di<*d)
		{
			intercepts = true;
			*d=di;
		}
		}
		
		i++;
	}while(i<meshCount);
	return intercepts;
#endif
	return false;
}

bool Model::Intersects(Ray* r,float* d,unsigned int* mIndex)
{
#ifndef NO_RAYTRACE
	*d = 1.0737418e+008f;
	unsigned int i=0;
	bool intercepts=false;
	float di=1.0737418e+008f;
	do
	{
		if(Mesh::Intersects(Meshes[i],r,&di))
		{
		if(di<*d)
		{
			intercepts = true;
			*d=di;
			*mIndex=i;
		}
		}
		i++;
	}while(i<meshCount);
	
	return intercepts;
#endif
	return false;
}

bool Model::Intersects(Ray* r,float dist,float* d)
{
#ifndef NO_RAYTRACE
	*d = 1.0737418e+008f;
	unsigned int i=0;
	bool intercepts=false;
	float di=1.0737418e+008f;
	do
	{
		if(Mesh::Intersects(Meshes[i],r,dist,&di))
		{
		if(di<*d)
		{
			intercepts = true;
			*d=di;
		}
		}
		
		i++;
	}while(i<meshCount);
	return intercepts;
#endif
	return false;
}

void Model::AddMaterial(Material* mat)
{
	Materials.insert(pair<unsigned int,Material*>(MatCount,mat));
	MatCount++;
}

void Model::UpdateAnimation(unsigned int fps)
{
#ifndef NO_ANIM
	this;
	if(this->animationSpeed!=0.0f)
	{
		double d = 60.0/(double)fps*(double)animationSpeed;
		unsigned int i =0;
		if(AnimCount!=0)
		{
		do
		{
			Animations[i]->Advance(d);
			i++;
		}while(i<AnimCount);
		}
	}
#endif
}

void Model::Update()
{
#ifndef NO_UPDATE
	Base::Update(root);
	unsigned int i=0;
	do
	{
		Meshes[i]->UpdateMesh();
		i++;
	}while(i<meshCount);
#endif
}

int Model::GetIndex(const char* name)
{
	unsigned int i =0;
	do
	{
		const char* name2 = objects[i]->name->data();
		if(strcmp(name2,name)==0)
			return i;
		i++;
	}while(i<objCount);
	return -1;
}

bool Model::CreateHeightMap()
{
	wstring s;
	s.assign(filePath.data());
	long l = (long)s.length();
	s.erase(l-3,3);
	s.append(L"/");
	s.append(L"HeightMap.txt");
	bool result=false;
	HeightMap* map=new HeightMap();
	result =map->Load(s.data());
	this;
	if(result==false)
	{
	Ray r;
	r.SetDirection(0,0,-1);
	BoundingBox b;
	b.min=D3DXVECTOR3(0,0,0);
	b.max=D3DXVECTOR3(0,0,0);
	b.origMin=D3DXVECTOR3(0,0,0);
	b.origMax=D3DXVECTOR3(0,0,0);
	unsigned int i=0;
	do
	{
		b.Merge(Meshes[i]->box);
		i++;
	}while(i<meshCount);
	map->Initialize(&b.max,b.max.x-b.min.x,b.max.y-b.min.y);
	i=0;
	unsigned int ii=0;
	do
	{
		ii=0;
		do
		{
			r.SetPosition(floor(b.max.x-i*HMapScale),floor(b.max.y-ii*HMapScale),ceil(b.max.z));
			float* d = new float();
			if(this->Intersects(&r,d))
			map->data[i][ii]=*d;
			else
				map->data[i][ii]=0;
			ii++;
		}while(ii<map->height);
		i++;
	}while(i<map->width);
	map->Save(s.data());
	}
	hmap=map;	
	return true;
};