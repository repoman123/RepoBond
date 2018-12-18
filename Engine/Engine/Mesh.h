////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef MESHCLASS_H
#define MESHCLASS_H

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "Animation.h"
#include "Base.h"
#include "textureclass.h"
#include "material.h"
#include "Ray.h"
#include "BoundingSphere.h"
#include "Bone.h"
#include <cstring>
#include <string>
#include "Quad.h"
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class Mesh: public Base
{
	
private:
	struct ColorVertex
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR3 normal;
	};
	struct TextureVertex
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR3 normal;
		D3DXVECTOR2 texture;
	};
	struct BoneVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 weights;//the bone vertex weight for max 4 bones
		D3DXVECTOR4 indices;//the bone matrix index for max 4 bones
	};
public:
	enum HMapping
	{
		hmap,
		none,
		ray
	};
	HMapping hMap;
	Mesh();
	Mesh(const Mesh&);
	~Mesh();
	bool hasUVs;
	void Dispose();
	void GenQuads();
	int GetVertexCount();
	int GetIndexCount();
	bool InitializeBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,D3DXVECTOR2* uvs,long* inds);
	bool InitializeBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,long* inds);
	bool InitializeBoneBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,D3DXVECTOR2* uvs,long* inds);
	//bool InitializeBoneBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,long* inds);
	D3DXVECTOR3* positions;//for triangle interception
	Quad quad;
	void UpdateMesh();
	D3DXMATRIX* BoneTransforms;
	bool hasBones;
	unsigned int boneCount;
	unordered_map<unsigned int,Bone*> Bones;
	void AddBone(Bone*);
	Material* mat;
	static bool Intersects(Mesh*,Ray* r,float*);
	static bool Intersects(Mesh*,Ray* r,float,float*);
	static bool Intersects(Mesh*,BoundingBox*);
	static bool Intersects(Mesh*,float*);
	static void RenderBuffers(Mesh* m,ID3D10Device*);
	unsigned int mVertexCount, mIndexCount;
private:
	void DisposeBuffers();
private:
	ID3D10Buffer *mVertexBuffer, *mIndexBuffer;
	
};

#endif