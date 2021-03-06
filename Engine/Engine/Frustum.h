////////////////////////////////////////////////////////////////////////////////
// Filename: Frustum.h
////////////////////////////////////////////////////////////////////////////////
#ifndef FRUSTUM_H
#define FRUSTUM_H

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: Frustum
////////////////////////////////////////////////////////////////////////////////
class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();
	void ConstructFrustum(float, D3DXMATRIX, D3DXMATRIX);
	bool CheckPoint(float, float, float);
	static bool CheckCube(Frustum* f,float, float, float, float,float,float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);
	D3DXPLANE* m_planes;
};

#endif