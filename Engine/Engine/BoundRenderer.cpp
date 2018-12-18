#include "BoundRenderer.h"


BoundRenderer::BoundRenderer(void)
{
	rayLength=10000;
	boxCount=0;
	rayCount=0;
	frustumCount=0;

}

BoundRenderer::~BoundRenderer(void)
{
}

bool BoundRenderer::Initialize(ID3D10Device* dev)
{
	HRESULT result;
	D3D10_BUFFER_DESC boxIndexBufferDesc;
	D3D10_SUBRESOURCE_DATA boxIndexData;
	// Set up the description of the index buffer.
    boxIndexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    boxIndexBufferDesc.ByteWidth = sizeof(unsigned long) * 19;
    boxIndexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    boxIndexBufferDesc.CPUAccessFlags = 0;
    boxIndexBufferDesc.MiscFlags = 0;
	
	unsigned long* lines = new unsigned long[17];
	//we define the linestrip for the BoundingBox with 8 corners
	//left side
	lines[0]=0;
	lines[1]=3;
	lines[2]=6;
	lines[3]=3;
	lines[4]=2;//
	lines[5]=7;
	lines[6]=2;
	lines[7]=1;//
	lines[8] =4;
	lines[9] =1;
	lines[10] =0;
	lines[11] =5;
	lines[12]=6;
	lines[13]=7;
	lines[14] =4;
	lines[15] =5;
	lines[16] =0;

	this->dev =dev;
	// Give the subresource structure a pointer to the index data.
    boxIndexData.pSysMem = lines;

	// Create the index buffer.
	result = dev->CreateBuffer(&boxIndexBufferDesc, &boxIndexData, &boxIndBuf);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

void BoundRenderer::AddBound(BoundingBox* b)
{
	resizeBoxs=true;
	BoxInstance* bi = new BoxInstance();
	bi->color= D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
	bi->box=b;
	bi->hasColor=false;
	boxs.insert(pair<unsigned int,BoxInstance*>(boxCount,bi));
	boxCount++;
	return;
}

void BoundRenderer::AddBound(Ray* b)
{
	resizeRays=true;
	RayInstance* ri = new RayInstance();
	ri->color= D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
	ri->ray=b;
	ri->hasColor=false;
	rays.insert(pair<unsigned int,RayInstance*>(rayCount,ri));
	rayCount++;
}

void BoundRenderer::AddBound(Frustum* b)
{
	resizeFrustums=true;
	FrustumInstance* fi = new FrustumInstance();
	fi->color= D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
	fi->frustum=b;
	fi->hasColor=false;
	frustums.insert(pair<unsigned int,FrustumInstance*>(frustumCount,fi));
	frustumCount++;
}

#ifndef RENDER_NO_TRI_QUADS
void BoundRenderer::AddBound(Quad* q)
{
	if(q->triCount>0)
	{
		resizeBoxs=true;
		if(q->hasChildren)
		{
			if(q->children[0]->triCount>0)
			AddBound(q->children[0]);
			if(q->children[1]->triCount>0)
			AddBound(q->children[1]);
			if(q->children[2]->triCount>0)
			AddBound(q->children[2]);
			if(q->children[3]->triCount>0)
			AddBound(q->children[3]);
			if(q->children[4]->triCount>0)
			AddBound(q->children[4]);
			if(q->children[5]->triCount>0)
			AddBound(q->children[5]);
			if(q->children[6]->triCount>0)
			AddBound(q->children[6]);
			if(q->children[7]->triCount>0)
			AddBound(q->children[7]);
		}
		else
			AddBound(q->box);
	}
}
#endif
#ifdef RENDER_NO_TRI_QUADS
void BoundRenderer::AddBound(Quad* q)
{
	resizeBoxs=true;
	if(q->hasChildren)
	{
		AddBound(q->children[0]);
		AddBound(q->children[1]);
		AddBound(q->children[2]);
		AddBound(q->children[3]);
		AddBound(q->children[4]);
		AddBound(q->children[5]);
		AddBound(q->children[6]);
		AddBound(q->children[7]);
	}
	else
		AddBound(q->box);
}
#endif

void BoundRenderer::AddBound(BoundingBox* b,D3DXVECTOR4 c)
{
	resizeBoxs=true;
	BoxInstance* bi = new BoxInstance();
	bi->color= c;
	bi->hasColor=true;
	bi->box=b;
	boxs.insert(pair<unsigned int,BoxInstance*>(boxCount,bi));
	boxCount++;
	return;
}

void BoundRenderer::AddBound(Ray* b,D3DXVECTOR4 c)
{
	resizeRays=true;
	RayInstance* ri = new RayInstance();
	ri->color= c;
	ri->hasColor=true;
	ri->ray=b;
	rays.insert(pair<unsigned int,RayInstance*>(rayCount,ri));
	rayCount++;
}

void BoundRenderer::AddBound(Frustum* b,D3DXVECTOR4 c)
{
	resizeFrustums=true;
	FrustumInstance* fi = new FrustumInstance();
	fi->color= c;
	fi->hasColor=true;
	fi->frustum=b;
	frustums.insert(pair<unsigned int,FrustumInstance*>(frustumCount,fi));
	frustumCount++;
}

#ifndef RENDER_NO_TRI_QUADS
void BoundRenderer::AddBound(Quad* q,D3DXVECTOR4 c)
{
	if(q->triCount>0)
	{
		resizeBoxs=true;
		if(q->hasChildren)
		{
			if(q->children[0]->triCount>0)
			AddBound(q->children[0],c);
			if(q->children[1]->triCount>0)
			AddBound(q->children[1],c);
			if(q->children[2]->triCount>0)
			AddBound(q->children[2],c);
			if(q->children[3]->triCount>0)
			AddBound(q->children[3],c);
			if(q->children[4]->triCount>0)
			AddBound(q->children[4],c);
			if(q->children[5]->triCount>0)
			AddBound(q->children[5],c);
			if(q->children[6]->triCount>0)
			AddBound(q->children[6],c);
			if(q->children[7]->triCount>0)
			AddBound(q->children[7],c);
		}
		else
			AddBound(q->box,c);
	}
}
#endif
#ifdef RENDER_NO_TRI_QUADS
void BoundRenderer::AddBound(Quad* q,D3DXVECTOR4 c)
{
	resizeBoxs=true;
	if(q->hasChildren)
	{
		AddBound(q->children[0],c);
		AddBound(q->children[1],c);
		AddBound(q->children[2],c);
		AddBound(q->children[3],c);
		AddBound(q->children[4],c);
		AddBound(q->children[5],c);
		AddBound(q->children[6],c);
		AddBound(q->children[7],c);
	}
	else
		AddBound(q->box,c);
	
}
#endif

void BoundRenderer::Render(Shaders* sh,D3DXMATRIX* view,D3DXMATRIX* proj)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	unsigned int stride=3*sizeof(float);
	unsigned int offset=0;
	if(boxCount>0)
	{
		dev->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

		dev->IASetVertexBuffers(0, 1, &boxVertBuf, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		dev->IASetIndexBuffer(boxIndBuf, DXGI_FORMAT_R32_UINT, 0);
		sh->SetSimpleColorParams(mat,*view,*proj,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
		unsigned int i=0;
		D3DXVECTOR4 c = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
		do
		{

			D3D10_TECHNIQUE_DESC techniqueDesc;
			unsigned int ii;
			// Set the input layout.
			sh->difColorPtr->SetFloatVector((float*)boxs[i]->color);
			dev->IASetInputLayout(sh->simpleColorLayout);

			// Get the description structure of the technique from inside the shader so it can be used for rendering.
			sh->simpleColorTechnique->GetDesc(&techniqueDesc);
		
			// Go through each pass in the technique (should be just one currently) and render the triangles.
			for(ii=0; ii<techniqueDesc.Passes; ++ii)
			{
				sh->simpleColorTechnique->GetPassByIndex(ii)->Apply(0);
				dev->DrawIndexed(17,0,8*i);
			}
			i++;
		}while(i<boxCount);
	}
	//ray rendering
	if(rayCount>0)
	{
		dev->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		dev->IASetVertexBuffers(0, 1, &rayVertBuf, &stride, &offset);

		sh->SetSimpleColorParams(mat,*view,*proj,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
		unsigned int i=0;
		D3DXVECTOR4 c = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
		do
		{

			D3D10_TECHNIQUE_DESC techniqueDesc;
			unsigned int ii;
			// Set the input layout.
			sh->difColorPtr->SetFloatVector((float*)rays[i]->color);
			dev->IASetInputLayout(sh->simpleColorLayout);

			// Get the description structure of the technique from inside the shader so it can be used for rendering.
			sh->simpleColorTechnique->GetDesc(&techniqueDesc);
		
			// Go through each pass in the technique (should be just one currently) and render the triangles.
			for(ii=0; ii<techniqueDesc.Passes; ++ii)
			{
				sh->simpleColorTechnique->GetPassByIndex(ii)->Apply(0);
				dev->Draw(2,2*i);
			}
			i++;
		}while(i<rayCount);
	}
}

bool BoundRenderer::UpdateBoxs()
{
	if(boxCount>0)
	if(resizeBoxs)
		return InitializeBoxBuffer();
	else
		return UpdateBoxBuffer();
	return true;
}

bool BoundRenderer::UpdateFrustums()
{
	if(frustumCount>0)
	if(resizeFrustums)
		return InitializeFrustumBuffer();
	else
		return UpdateFrustumBuffer();
	return true;
}

bool BoundRenderer::UpdateRays()
{
	if(resizeRays)
		return InitializeRayBuffer();
	else
		return UpdateRayBuffer();
}

bool BoundRenderer::InitializeBoxBuffer()
{
	resizeBoxs=false;
	HRESULT result;
	D3D10_BUFFER_DESC vertBufferDesc;
	D3D10_SUBRESOURCE_DATA vertData;
	D3DXVECTOR3* boxVerts = new D3DXVECTOR3[boxCount*8];
	unsigned int i =0;
	do
	{
		D3DXVECTOR3* vs = boxs[i]->box->GetPoints();
		boxVerts[(i*8)]=vs[0];
		boxVerts[(i*8)+1]=vs[1];
		boxVerts[(i*8)+2]=vs[2];
		boxVerts[(i*8)+3]=vs[3];

		boxVerts[(i*8)+4]=vs[4];
		boxVerts[(i*8)+5]=vs[5];
		boxVerts[(i*8)+6]=vs[6];
		boxVerts[(i*8)+7]=vs[7];
		i++;
	}while(i<boxCount);

	// Set up the description of the instance buffer.
	vertBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertBufferDesc.ByteWidth = sizeof(float) *3 * 8 * boxCount;
	vertBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the instance data.
	vertData.pSysMem = boxVerts;
	boxVertBuf=0;
	// Create the instance buffer.
	result = dev->CreateBuffer(&vertBufferDesc, &vertData, &boxVertBuf);
	if(FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete [] boxVerts;
	boxVerts = 0;
	return true;
}

bool BoundRenderer::InitializeRayBuffer()
{
	resizeRays=false;
	HRESULT result;
	D3D10_BUFFER_DESC vertBufferDesc;
	D3D10_SUBRESOURCE_DATA vertData;
	D3DXVECTOR3* rayVerts = new D3DXVECTOR3[rayCount*2];
	unsigned int i =0;
	do
	{
		#ifdef RENDER_TRANSFORMED_RAY
		rayVerts[i*2]=this->rays[i]->ray->pos;
		rayVerts[i*2+1]=this->rays[i]->ray->dir*rayLength;
		#endif
		#ifndef RENDER_TRANSFORMED_RAY
		rayVerts[i*2]=this->rays[i]->ray->origPos;
		rayVerts[i*2+1]=this->rays[i]->ray->origDir*rayLength;
		#endif
		i++;
	}while(i<rayCount);

	// Set up the description of the instance buffer.
	vertBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertBufferDesc.ByteWidth = sizeof(float) *3 * 2 * rayCount;
	vertBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the instance data.
	vertData.pSysMem = rayVerts;
	rayVertBuf=0;
	// Create the instance buffer.
	result = dev->CreateBuffer(&vertBufferDesc, &vertData, &rayVertBuf);
	if(FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete [] rayVerts;
	rayVerts = 0;
	return true;
}

bool BoundRenderer::InitializeFrustumBuffer()
{
	return false;
}

bool BoundRenderer::UpdateBoxBuffer()
{
	HRESULT result;
	void* instPtr;
	D3DXVECTOR3* boxVerts = new D3DXVECTOR3[boxCount*8];
	memset(boxVerts,0,sizeof(D3DXVECTOR3)*8*boxCount);
	unsigned int i =0;
	do
	{
		D3DXVECTOR3* vs = boxs[i]->box->GetPoints();
		boxVerts[(i*8)]=vs[0];
		boxVerts[(i*8)+1]=vs[1];
		boxVerts[(i*8)+2]=vs[2];
		boxVerts[(i*8)+3]=vs[3];

		boxVerts[(i*8)+4]=vs[4];
		boxVerts[(i*8)+5]=vs[5];
		boxVerts[(i*8)+6]=vs[6];
		boxVerts[(i*8)+7]=vs[7];
		i++;
	}while(i<boxCount);
	instPtr=0;
	result = boxVertBuf->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&instPtr);
	if(FAILED(result))
	{
		delete boxVerts;
		return false;
	}
	unsigned int width = sizeof(D3DXVECTOR3) * boxCount*8;
	memcpy(instPtr, (void*)boxVerts,width);
	
	boxVertBuf->Unmap();
	delete boxVerts;
	return true;
}

bool BoundRenderer::UpdateRayBuffer()
{
	resizeRays=false;
	HRESULT result;
	D3DXVECTOR3* rayVerts = new D3DXVECTOR3[rayCount*2];
	memset(rayVerts,0,sizeof(D3DXVECTOR3)*2*rayCount);
	void* instPtr;
	unsigned int i =0;
	do
	{
		#ifdef RENDER_TRANSFORMED_RAY
		rayVerts[i*2]=this->rays[i]->ray->pos;
		rayVerts[i*2+1]=this->rays[i]->ray->dir*rayLength;
		#endif
		#ifndef RENDER_TRANSFORMED_RAY
		rayVerts[i*2]=this->rays[i]->ray->origPos;
		rayVerts[i*2+1]=this->rays[i]->ray->origDir*rayLength;
		#endif
		i++;
	}while(i<rayCount);
	instPtr=0;
	result = rayVertBuf->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&instPtr);
	if(FAILED(result))
	{
		delete rayVerts;
		return false;
	}
	unsigned int width = sizeof(D3DXVECTOR3)*2*rayCount;
	memcpy(instPtr, (void*)rayVerts,width);
	
	rayVertBuf->Unmap();
	delete rayVerts;
	return true;
}

bool BoundRenderer::UpdateFrustumBuffer()
{
	return false;
}
