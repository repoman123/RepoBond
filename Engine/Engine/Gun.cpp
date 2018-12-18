#include "Gun.h"


Gun::Gun(void)
{
	hasScope=false;
	this->scopeMaxZoom=0.0f;
	this->scopeMinZoom=0.0f;
}

Gun::~Gun(void)
{
}

void Gun::Update()
{
	#ifndef NO_UPDATE
	unsigned int i=0;
	do
	{
		Meshes[i]->UpdateMesh();
		i++;
	}while(i<meshCount);
	Base::Update(root);
	dir.origPos  = Base::MatrixPosition(Base::GetMatrix(objects[barrel]));
	dir.origDir = Base::MatrixPosition(Base::GetMatrix(objects[barreldirection]));
	dir.origDir = dir.origPos - dir.origDir;
	int test=0;
	#endif

}
