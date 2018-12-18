#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animations.h"
#include <D3DX10math.h>
#include "AnimKey.h"

class Animator: Animations
{
public:
	Animator(void);
	~Animator(void);
	bool needsUpdate;

	D3DXVECTOR3* firstPos;
	D3DXQUATERNION* firstRot;
	D3DXVECTOR3* firstSc;

	D3DXVECTOR3* cPos;
	D3DXQUATERNION* cRot;
	D3DXVECTOR3* cSc;

	double pdt;
	double rdt;
	double sdt;

	AnimKey* nextRot;
	AnimKey* nextSc;
	AnimKey* nextPos;

	D3DXMATRIX scale;
	D3DXMATRIX rot;
	D3DXMATRIX tra;
	//the previous matrix we return
	D3DXMATRIX prev;

	double time;
	void Initialize(AnimKey* pos,AnimKey* rot,AnimKey* sc);
	void Initialize(D3DXVECTOR3* pos,D3DXQUATERNION* rot,D3DXVECTOR3* scale);
	void SetNext(AnimKey* pos,AnimKey* rot,AnimKey* sc);
	void SetFirst(AnimKey* pos,AnimKey* rot,AnimKey* sc);
	void SetFirst(D3DXVECTOR3* pos,D3DXQUATERNION* rot,D3DXVECTOR3* scale);
	void Advance(double time);
	D3DXMATRIX* GetTransform();
};

#endif