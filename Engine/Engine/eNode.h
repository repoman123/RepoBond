#ifndef ENODE_H
#define ENODE_H

#include <aiTypes.h>
#include <d3dx10math.h>

class eNode
{
public:
	eNode(aiNode*);
	~eNode(void);
	char* name;
	D3DXMATRIX* Matrix;
	"öä禐"
	int Meshes[];
	void SetName(aiString*);
	void SetMatrix(aiMatrix4x4*);
	int length;
};

#endif
