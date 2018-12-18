#include "Bone.h"


Bone::Bone(void)
{
	indices =0;
	weights =0;
	offsetCount=0;
	offsetIndex =0;
}

Bone::~Bone(void)
{
}

void Bone::AddOffset(D3DXMATRIX mat)
{
	offsetMatrices.insert(pair<unsigned int,D3DXMATRIX>(offsetCount,mat));
	offsetCount++;
}

void Bone::SetCurrentAnim(unsigned int anim,bool apToCh)
{
	base->SetCurrentAnim(anim,apToCh);
}

D3DXMATRIX* Bone::GetOffset(Bone* b)
{
	return &b->offsetMatrices[b->offsetIndex];
}

D3DXMATRIX* Bone::GetMatrix(Bone* b)
{
	return &b->finalMat;
}

void Bone::Update(Bone* b)
{
	if(b->base->parent!=0)
	{
		if(b->base->parent->hasMatrix)
		{
			D3DXMatrixMultiply(&b->finalMat,GetOffset(b),&b->base->Matrix);
			D3DXMatrixMultiply(&b->finalMat,&b->finalMat,Base::GetMatrix(b->base->parent));
		}
		else
		{
			D3DXMatrixMultiply(&b->finalMat,GetOffset(b),&b->base->Matrix);
		}
	}
	else
	{
		D3DXMatrixMultiply(&b->finalMat,GetOffset(b),&b->base->Matrix);
	}
}