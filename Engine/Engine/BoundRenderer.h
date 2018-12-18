#ifndef BOUNDRENDERER_H
#define BOUNDRENDERER_H

#include "Frustum.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "Quad.h"
#include <unordered_map>
#include <D3DX10math.h>
#include "D3D10Shaders.h"
using namespace std;
//#define RENDER_TRANSFORMED_RAY
//#define RENDER_NO_TRI_QUADS

class BoundRenderer
{
public:
	BoundRenderer(void);
	~BoundRenderer(void);

	struct BoxInstance
	{
		BoundingBox* box;
		bool hasColor;
		D3DXVECTOR4 color;
	};
	struct FrustumInstance
	{
		Frustum* frustum;
		bool hasColor;
		D3DXVECTOR4 color;
	};
	struct RayInstance
	{
		Ray* ray;
		bool hasColor;
		D3DXVECTOR4 color;
	};
	bool Initialize(ID3D10Device*);
	unordered_map<unsigned int,BoxInstance*> boxs;
	unordered_map<unsigned int,RayInstance*> rays;
	unordered_map<unsigned int,FrustumInstance*> frustums;
	unsigned int boxCount;
	unsigned int rayCount;
	unsigned int frustumCount;
	void AddBound(BoundingBox*);
	void AddBound(Quad*);
	void AddBound(Ray*);
	void AddBound(Frustum*);
	void AddBound(BoundingBox*,D3DXVECTOR4);
	void AddBound(Quad*,D3DXVECTOR4);
	void AddBound(Ray*,D3DXVECTOR4);
	void AddBound(Frustum*,D3DXVECTOR4);
	bool UpdateBoxs();
	bool UpdateFrustums();
	bool UpdateRays();
	void Render(Shaders*,D3DXMATRIX*,D3DXMATRIX*);
	float rayLength;
private:
	bool InitializeBoxBuffer();
	bool InitializeRayBuffer();
	bool InitializeFrustumBuffer();
	bool UpdateBoxBuffer();
	bool UpdateRayBuffer();
	bool UpdateFrustumBuffer();
	ID3D10Device* dev;
	ID3D10Buffer* boxVertBuf;
	ID3D10Buffer* boxIndBuf;
	ID3D10Buffer* rayVertBuf;
	ID3D10Buffer* frustumVertBuf;
	ID3D10Buffer* frustumIndBuf;
	bool resizeFrustums;
	bool resizeRays;
	bool resizeBoxs;
};

#endif