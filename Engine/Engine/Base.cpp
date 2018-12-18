#include "Base.h"


Base::Base(void)
{
	childCount=0;
	parent =0;
	box=0;
	hasMatrix=false;
	name = new string();
	animCount=0;
	hasAnimation=false;
	animCount=0;
	type = base;
	boxUpdate=true;
	hasbox=false;
	currentAnim=0;
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&traMat);
	D3DXMatrixIdentity(&scaleMat);
	rotation = D3DXVECTOR3(0,0,0);
}
Base::~Base(void)
{
}

void Base::AddAnimation(Animation* anim)
{
	hasAnimation=true;
	animations.insert(pair<unsigned int,Animation*>(animCount,anim));
	animCount++;
}

void Base::SetCurrentAnim(unsigned int anim,bool apToCh)
{
	if(apToCh && childCount>0)
	{
		if(animCount > anim)
			currentAnim=anim;
		unsigned int i =0;
		do
		{
			children[i]->SetCurrentAnim(anim,true);
			i++;
		}while(i<childCount);
	}
	if(animCount>anim)
	{
		currentAnim=anim;
	}
}

void Base::SetAnimStart(unsigned int frame,bool apToCh)
{
	if(animCount>0)
	GetCurrentAnim(this)->SetStart(frame);
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->SetAnimStart(frame,true);
			i++;
		}while(i<childCount);
	}
}

void Base::SetAnimEnd(unsigned int frame,bool apToCh)
{
	if(animCount>0)
	GetCurrentAnim(this)->SetEnd(frame);
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->SetAnimEnd(frame,apToCh);
			i++;
		}while(i<childCount);
	}
}

void Base::StopAnimation(bool apToCh,bool apToPr)
{
	if(animCount>0)
	GetCurrentAnim(this)->animate=false;
	if(apToPr && parent!=0)
	{
		parent->StopAnimation(false,true);
	}
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->StopAnimation(apToCh,false);
			i++;
		}while(i<childCount);
	}
}

void Base::StartAnimation(bool apToCh,bool apToPr)
{
	if(animCount>0)
	GetCurrentAnim(this)->animate=true;
	if(apToPr && parent!=0)
	{
		parent->StartAnimation(false,true);
	}
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->StartAnimation(apToCh,false);
			i++;
		}while(i<childCount);
	}
}

void Base::SetAnimPos(unsigned int frame,bool apToCh)
{
	if(animCount>0)
	GetCurrentAnim(this)->SetPosition(frame);
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->SetAnimPos(frame,apToCh);
			i++;
		}while(i<childCount);
	}
}

void Base::SetAnimPos(double time,bool apToCh)
{
	if(animCount>0)
	GetCurrentAnim(this)->SetPosition(time);
	if(apToCh && childCount>0)
	{
		unsigned int i=0;
		do
		{
			children[i]->SetAnimPos(time,apToCh);
			i++;
		}while(i<childCount);
	}
}

Animation* Base::GetCurrentAnim(Base* b)
{
	if(b->animCount>0)
	return b->animations[b->currentAnim];
	else
	return 0;
}

void Base::SetMatrix(D3DXMATRIX* mat)
{
	box->Transform(mat);
	hasMatrix=true;
	Matrix = *mat;
}

void Base::Rotate(float x,float y,float z)
{
	transform=true;
	rotation.x=x;
	rotation.y=y;
	rotation.z=z;
	D3DXMATRIX rx,ry,rz;
	D3DXMatrixRotationX(&rx,x);
	D3DXMatrixRotationY(&ry,y);
	D3DXMatrixRotationZ(&rz,z);
	rotMat = rx*ry*rz;
};

void Base::Rotate(D3DXQUATERNION* q)
{
	transform=true;
	D3DXMatrixRotationQuaternion(&rotMat,q);
};

void Base::Translate(float x,float y,float z)
{
	transform=true;
	translation.x = x;
	translation.y = y;
	translation.z = z;
	D3DXMatrixTranslation(&Matrix,x,y,z);
};

void Base::Scale(float x,float y,float z)
{
	transform=true;
	scale.x =x;
	scale.y = y;
	scale.z = z;
	D3DXMatrixScaling(&scaleMat,x,y,z);
};

void Base::Offset(float x,float y,float z)
{
	transform=true;
	D3DXVECTOR3 p = MatrixPosition(&Matrix);
	translation.x = p.x+x;
	translation.y = p.y+y;
	translation.z = p.z+z;
	D3DXMatrixTranslation(&Matrix,translation.x,translation.y,translation.z);
};

D3DXMATRIX* Base::GetMatrix(Base* b)
{
	return &b->finalMat;
}

void Base::Update(Base* b)
{
	if(b->hasAnimation && b->animCount>0)
		b->Matrix = GetCurrentAnim(b)->GetTransform();
	if(b->transform)
	{
		if(b->parent!=0)
		{
			if(b->parent->hasMatrix)
			{
				D3DXMatrixMultiply(&b->finalMat,&b->scaleMat,&b->rotMat);
				D3DXMatrixMultiply(&b->finalMat,&b->finalMat,&b->Matrix);
				D3DXMatrixMultiply(&b->finalMat,&b->finalMat,Base::GetMatrix(b->parent));
			}
			else
			{
				D3DXMatrixMultiply(&b->finalMat,&b->scaleMat,&b->rotMat);
				D3DXMatrixMultiply(&b->finalMat,&b->finalMat,&b->Matrix);
			}
			if(b->childCount>0)
			{
			unsigned int i=0;
			do
			{
				Base::Update(b->children[i]);
				i++;
			}while(i<b->childCount);
			}
		}
		else
		{
			D3DXMatrixMultiply(&b->finalMat,&b->rotMat,&b->Matrix);
			if(b->childCount>0)
			{
			unsigned int i=0;
			do
			{
				Base::Update(b->children[i]);
				i++;
			}while(i<b->childCount);
			}
		}
	}
	else
	{
		if(b->parent!=0)
		{
			if(b->parent->hasMatrix)
			{
				D3DXMatrixMultiply(&b->finalMat,&b->Matrix,Base::GetMatrix(b->parent));
			}
			else
			{
				b->finalMat= b->Matrix;
			}
			if(b->childCount>0)
			{
			unsigned int i=0;
			do
			{
				Base::Update(b->children[i]);
				i++;
			}while(i<b->childCount);
			}
		}
		else
		{
			b->finalMat= b->Matrix;
			if(b->childCount>0)
			{
			unsigned int i=0;
			do
			{
				Base::Update(b->children[i]);
				i++;
			}while(i<b->childCount);
			}
		}
	}
}

void Base::AddChild(Base* ch)
{
	children.insert(pair<int,Base*>(childCount,ch));
	ch->parent=this;
	ch->hasMatrix=true;
	childCount++;
}

char* Base::ToChars(const wchar_t* wcs,unsigned int l)
{
	char* chs = new char[l];
	unsigned int i=0;
	do
	{
		chs[i]=(char)wcs[i];
		i++;
	}while(i<l);
	chs[l]=0;
	return chs;
}

wchar_t* Base::ToWChars(const char* st,unsigned int length)
{
	wchar_t* ws = new wchar_t[length];
	MultiByteToWideChar(CP_ACP,0,st,-1,ws,length);
	ws[length]=0;
	return ws;
}

D3DXVECTOR3 Base::MatrixPosition(D3DXMATRIX* mat)
{
	D3DXVECTOR3 v = D3DXVECTOR3(mat->_41,mat->_42,mat->_43 );
	return v;
}

D3DXVECTOR3 Base::MatrixScaling(D3DXMATRIX* mat)
{
	D3DXVECTOR3 scale;
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXMatrixDecompose(&scale,&rot,&pos,mat);
	return scale;
}

D3DXQUATERNION Base::MatrixRotation(D3DXMATRIX* mat)
{
	D3DXVECTOR3 scale;
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXMatrixDecompose(&scale,&rot,&pos,mat);
	return rot;
}