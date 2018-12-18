#include "SpriteBatch.h"


SpriteBatch::SpriteBatch(void)
{
	spriteCount=0;
}

SpriteBatch::~SpriteBatch(void)
{
}

bool SpriteBatch::InitializeBuffers()
{
	resize=false;
	HRESULT result;
	SpriteRenderer::SpriteInstance* instances;
	D3D10_BUFFER_DESC instanceBufferDesc;
	D3D10_SUBRESOURCE_DATA instanceData;
	// Create the instance array.
	instances = new SpriteRenderer::SpriteInstance[spriteCount];
	if(!instances)
	{
		return false;
	}
	unsigned int i =0;
	if(spriteCount!=0)
	do
	{
		Sprite* sp = sprites[i];
		D3DXVECTOR2 v = sprend->ToScreenPos(sp->bound.x,sp->bound.y,sp->bound.w,sp->bound.h);

		instances[i].size.x = (float)sp->bound.w;
		instances[i].size.y = (float)sp->bound.h;

		instances[i].texcoord.x = sp->UV.x*sp->texIndex;
		instances[i].texcoord.y = sp->UV.y*sp->texIndex;
		if(sp->hasTexture)
		instances[i].texcoord.z = (float)sp->texIndex;
		else
		 int test =0;
		instances[i].position.x=v.x;
		instances[i].position.y = v.y;
		instances[i].position.z = sp->Zval;
		i++;
	}while(i<spriteCount);
	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(SpriteRenderer::SpriteInstance) * spriteCount;
	instanceBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;

	// Create the instance buffer.
	result = sprend->device->CreateBuffer(&instanceBufferDesc, &instanceData, &instBuf);
	if(FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete [] instances;
	instances = 0;
	return true;
}

bool SpriteBatch::UpdateBuffers()
{
	HRESULT result;
	void* instPtr;
	instPtr=0;
	SpriteRenderer::SpriteInstance* instances = new SpriteRenderer::SpriteInstance[spriteCount];
	unsigned int i =0;
	do
	{
		Sprite* sp = sprites[i];
		
		instances[i].size.x = (float)sp->bound.w;
		instances[i].size.y = (float)sp->bound.h;

		instances[i].texcoord.x = sp->UV.x;
		instances[i].texcoord.y = sp->UV.y;
		if(sp->hasTexture)
			instances[i].texcoord.z= (float)sp->texIndex;
		else
			int test=0;
		D3DXVECTOR2 v = sprend->ToScreenPos(sp->bound.x,sp->bound.y,(unsigned int)instances[i].size.x,(unsigned int)instances[i].size.y);
		instances[i].position.x=v.x;
		instances[i].position.y = v.y;
		instances[i].position.z = sp->Zval;
		
		i++;
	}while(i<spriteCount);

	result = instBuf->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&instPtr);
	if(FAILED(result))
	{
		delete instances;
		return false;
	}
	unsigned int width = sizeof(SpriteRenderer::SpriteInstance) * spriteCount;
	memcpy(instPtr, (void*)instances,width);
	
	instBuf->Unmap();
	delete instances;
	return true;
}

bool SpriteBatch::Initialize(SpriteRenderer* sprend,Sprite* sp)
{
	this->sprend = sprend;
	bool result= false;
	AddSprite(sp);
	if(result==false)
		return false;
	result = InitializeBuffers();
	if(result==false)
		return false;
	return true;
}

bool SpriteBatch::Initialize(SpriteRenderer* sprend)
{
	this->sprend = sprend;
	bool result;
	result = InitializeBuffers();
	if(result==false)
		return false;
	return true;
}

void SpriteBatch::AddSprite(Sprite* sp)
{
	sprites.insert(pair<int,Sprite*>(spriteCount,sp));
	spriteCount++;
	if(sp->texPath!=0)
	{
	sprend->shaders->texs->AddTexture(sp->texPath);
	sp->texIndex = sprend->shaders->texs->GetTexCount()-1;
	sp->hasTexture=true;
	}
	resize=true;
}

bool SpriteBatch::UpdateSprites()
{
	bool res;
	if(resize)
	res = InitializeBuffers();
	res = UpdateBuffers();
	return res;
}

void SpriteBatch::RenderBuffers()
{
	unsigned int stride=sizeof(SpriteRenderer::SpriteInstance);
	unsigned int offset=0;
	// Set the vertex and instance buffers to active in the input assembler so it can be rendered.
	sprend->device->IASetVertexBuffers(1, 1,&instBuf, &stride, &offset);
}

void SpriteBatch::Render()
{
	RenderBuffers();
	sprend->shaders->RenderSprites(sprend->device,spriteCount);
}

