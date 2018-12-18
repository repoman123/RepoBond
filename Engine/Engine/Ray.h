#ifndef RAY_H

#define RAY_H

#include <d3dx10math.h>
#include "helpers.h"
#include <algorithm>
#undef min
#undef max

const float MIN = std::numeric_limits<float>::min();
const float MAX = std::numeric_limits<float>::max();



class Ray
{
public:
	Ray(void);
	~Ray(void);
	void Transform(D3DXMATRIX*);
	void TransformInverse(D3DXMATRIX*);
	void Initialize(D3DXVECTOR3*,D3DXVECTOR3*);
	void SetPosition(float,float,float);
	void SetPosition(D3DXVECTOR3*);
	void SetDirection(float,float,float);
	void SetDirection(D3DXVECTOR3*);
	D3DXVECTOR3* GetPosition();
	void Initialize(float,float,float,float,float,float);
	static bool InterceptsBox(Ray* r,D3DXVECTOR3* min,D3DXVECTOR3* max,float*);
	static bool InterceptsBox(Ray* r,D3DXVECTOR3* min,D3DXVECTOR3* max);
	static bool InterceptsBox(Ray* r,float minx,float miny,float minz,float maxx,float maxy,float maxz,float*);
	static bool InterceptsSphere(Ray* r,D3DXVECTOR3*,float,float*);
	static bool InterceptsTriangle(Ray* ray,D3DXVECTOR3*,D3DXVECTOR3*,D3DXVECTOR3*,float*);
	D3DXVECTOR3 dir;
	D3DXVECTOR3 invdir;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 origDir;
	D3DXVECTOR3 origPos;
};
#endif
