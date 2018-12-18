#include "Animator.h"


Animator::Animator(void)
{
	needsUpdate=true;
}

Animator::~Animator(void)
{
}

void Animator::Initialize(AnimKey* pos,AnimKey* rot,AnimKey* sc)
{
	firstRot=rot->rotvalue;
	firstPos = pos->value;
	firstSc = sc->value;
}

void Animator::Initialize(D3DXVECTOR3* pos,D3DXQUATERNION* rot,D3DXVECTOR3* scale)
{
	firstRot=rot;
	firstPos = pos;
	firstSc = scale;
}

void Animator::SetFirst(AnimKey* pos,AnimKey* rot,AnimKey* sc)
{
	firstRot = rot->rotvalue;
	firstPos = pos->value;
	firstSc = sc->value;
}

void Animator::SetFirst(D3DXVECTOR3* pos,D3DXQUATERNION* rot,D3DXVECTOR3* scale)
{
	firstRot = rot;
	firstPos = pos;
	firstSc = scale;
}

void Animator::SetNext(AnimKey* pos,AnimKey* rot,AnimKey* sc)
{
	nextRot = rot;
	nextPos = pos;
	nextSc = sc;
}

void Animator::Advance(double time)
{
	this->time += time;
	needsUpdate=true;
}

D3DXMATRIX* Animator::GetTransform()
{
	if(needsUpdate)
	{
	//we calculate the interpolation amount
	pdt = nextPos->time / time;
	rdt = nextRot->time / time;
	sdt = nextSc->time / time;

	//we use d3dx10 lerp functions to interpolate
	D3DXVec3Lerp(cPos,firstPos,nextPos->value,(float)pdt);
	D3DXVec3Lerp(cSc,firstSc,nextSc->value,(float)sdt);
	D3DXQuaternionSlerp(cRot,firstRot,nextRot->rotvalue,(float)rdt);
	needsUpdate=false;
	
	D3DXMatrixTranslation(&tra,cPos->x,cPos->y,cPos->z);
	D3DXMatrixRotationQuaternion(&rot,cRot);
	D3DXMatrixScaling(&scale,cSc->x,cSc->y,cSc->z);
	prev = scale*rot*tra;
	}
	return &prev;
}
