#ifndef BONE_H
#define BONE_H

#include "Base.h"

class Bone
{
public:
	Bone(void);
	~Bone(void);
	Base* base;
	static D3DXMATRIX* GetMatrix(Bone*);
	std::unordered_map<unsigned int,D3DXMATRIX> offsetMatrices;
	void AddOffset(D3DXMATRIX);
	static D3DXMATRIX* GetOffset(Bone*);
	static void Update(Bone*);
	unsigned int offsetIndex;
	unsigned int offsetCount;
	//void SetWeights(aiVertexWeight*,unsigned int);
	void SetCurrentAnim(unsigned int,bool apToCh);
	unsigned int numWeights;
	unsigned int* indices;
	D3DXMATRIX finalMat;
	float* weights;
};

#endif
