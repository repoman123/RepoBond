#ifndef QUAD_H
#define QUAD_H

#include <D3DX10math.h>
#include "BoundingBox.h"
#include "Ray.h"
using namespace std;
#include <unordered_map>
#include <map>

const unsigned int	QUAD_MAX_TRICOUNT = 500;

class Quad
{
public:
	
	Quad(void);
	~Quad(void);
	bool hasChildren;
	D3DXVECTOR3* positions;
	unsigned int triCount;
	unsigned long vertCount;
	void Initialize(D3DXVECTOR3*,unsigned int,BoundingBox*);
	void Initialize();
	void Split(D3DXVECTOR3*);
	void Split();
	static bool Intersects(Quad* q,Ray*,float*);
	static bool Intersects(Quad* q,Ray*,float,float*);
	BoundingBox* box;
	Quad* children[8];
};

#endif
