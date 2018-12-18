////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Mesh.h"

Mesh::Mesh()
{
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	hasBones=false;
	boneCount=0;
	hasMatrix=false;
	hasAnimation=false;
	hMap = ray;
	type=mesh;
	mat=0;
	box=0;
	render=false;
}
Mesh::~Mesh()
{
}
/*
bool Mesh::Initialize(ID3D10Device* device,aiMesh* mesh)
{
	Initialized=true;
	bool result;
	mVertexCount = mesh->mNumVertices;
	// Initialize the vertex and index buffer that hold the geometry for the mesh.
	result = InitializeBuffers(device,mesh);
	if(!result)
	{
		return false;
	}
	return true;
}

/// <summary>itializes the mesh bones</summary>
bool Mesh::InitializeWithBones(ID3D10Device* device,aiMesh* aimesh)
{
	mVertexCount=aimesh->mNumVertices;
	hasBones=true;
	Initialized = true;
	bool result;
	this;
	// Initialize the vertex and index buffer that hold the geometry for the mesh.
	result = InitializeBoneBuffers(device,aimesh);
	if(!result)
	{
		return false;
	}
	BoneTransforms = new D3DXMATRIX[boneCount+1];
	D3DXMatrixIdentity(&BoneTransforms[0]);
	return true;
}
*/
void Mesh::Dispose()
{
	// Release the vertex and index buffers.
	DisposeBuffers();

	return;
}

int Mesh::GetIndexCount()
{
	return mIndexCount;
}

int Mesh::GetVertexCount()
{
	return mVertexCount;
}

void Mesh::UpdateMesh()
{
	this;
	if(hasAnimation)
	{
		D3DXMATRIX* m = 0;
		m=Base::GetMatrix(this);
		if(hasbox)
			box->Transform(m);
		return;
		m=0;
		delete m;
	}
	if(hasBones==true)
	{
		unsigned int i=1;
		do
		{
			Bone::Update(Bones[i-1]);
			BoneTransforms[i]=*Bone::GetMatrix(Bones[i-1]);
			i++;
		}while(i<boneCount+1);
		return;
	}
}

void Mesh::AddBone(Bone* b)
{
	Bones.insert(pair<unsigned int,Bone*>(boneCount,b));
	boneCount++;
}

bool Mesh::InitializeBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,D3DXVECTOR2* uvs,long* indices)
{
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	hasUVs=true;
	TextureVertex* vertices;
	// Create the vertex array.
	vertices = new TextureVertex[mVertexCount];
	unsigned int i=0;
	do
	{
		vertices[i].position = pos[i];
		vertices[i].texture = uvs[i];
		vertices[i].normal = norms[i];
		i++;
	}while(i<mVertexCount);
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TextureVertex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	
	positions = new D3DXVECTOR3[mIndexCount];
	i=0;
	do
	{
		positions[i] = vertices[indices[i]].position;
		i++;
	}while(i<mIndexCount);
	box = new BoundingBox();
	box->fromPoints(positions,mIndexCount);
	hasbox=true;
	box->Transform(&this->Matrix);
	if(FAILED(result))
	{
		return false;
	}
	delete [] vertices;
	vertices=0;

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
	return true;
};

bool Mesh::InitializeBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,long* indices)
{
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	hasUVs=false;
	ColorVertex* vertices;
	// Create the vertex array.
	vertices = new ColorVertex[mVertexCount];
	unsigned int i=0;
	do
	{
		vertices[i].position = pos[i];
		vertices[i].normal = norms[i];
		i++;
	}while(i<mVertexCount);
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

	positions = new D3DXVECTOR3[mIndexCount];
	i=0;
	do
	{
		positions[i] = vertices[indices[i]].position;
		i++;
	}while(i<mIndexCount);
	box = new BoundingBox();
	box->fromPoints(positions,mIndexCount);
	hasbox=true;
	box->Transform(&this->Matrix);
	if(FAILED(result))
	{
		return false;
	}
	delete [] vertices;
	vertices=0;

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	return true;
};

bool Mesh::InitializeBoneBuffers(ID3D10Device* device,D3DXVECTOR3* pos,D3DXVECTOR3* norms,D3DXVECTOR2* uvs,long* indices)
{
	BoneTransforms = new D3DXMATRIX[boneCount+1];
	D3DXMatrixIdentity(&BoneTransforms[0]);
	this->hasBones=true;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	BoneVertex* vertices;
	// Create the vertex array.
	vertices = new BoneVertex[mVertexCount];
	unsigned int i=0;
	do
	{
		vertices[i].position = pos[i];
		vertices[i].texture = uvs[i];
		vertices[i].normal = norms[i];
		i++;
	}while(i<mVertexCount);


	i=0;
	//we assigne vertex weights and indices to zero by default
	do
	{
		vertices[i].weights.x =0.0f;
		vertices[i].weights.y =0.0f;
		vertices[i].weights.z =0.0f;
		vertices[i].weights.w =0.0f;
		vertices[i].indices.x=0.0f;
		vertices[i].indices.y=0.0f;
		vertices[i].indices.z=0.0f;
		vertices[i].indices.w=0.0f;
		i++;
	}while(i<mVertexCount);
	//we set the bone indices and weights
	i=0;
	unsigned int n=0;
	do
	{
		i=0;
		do
		{
			unsigned int index = Bones[n]->indices[i];
			if(vertices[index].weights.x==0.0f)
			{
			vertices[index].weights.x = Bones[n]->weights[i];
			vertices[index].indices.x=(float)n+1;
			}
			else
			if(vertices[index].weights.y==0.0f)
			{
			vertices[index].weights.y = Bones[n]->weights[i];
			vertices[index].indices.y=(float)n+1;
			}
			else
			if(vertices[index].weights.z==0.0f)
			{
			vertices[index].weights.z = Bones[n]->weights[i];
			vertices[index].indices.z=(float)n+1;
			}
			else
			if(vertices[index].weights.w==0.0f)
			{
			vertices[index].weights.w = Bones[n]->weights[i];
			vertices[index].indices.w=(float)n+1;
			}
			else
				i=0;//the vertex is effected by more than 4 bones not accepted
			i++;
		}while(i!=Bones[n]->numWeights);
		n++;
	}while(n<boneCount);
	n=0;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BoneVertex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

	positions = new D3DXVECTOR3[mIndexCount];
	i=0;
	do
	{
		positions[i] = vertices[indices[i]].position;
		i++;
	}while(i<mIndexCount);
	box = new BoundingBox();
	box->fromPoints(positions,mIndexCount);
	box->Transform(&this->Matrix);
	hasbox=true;

	if(FAILED(result))
	{
		return false;
	}
	delete [] vertices;
	vertices=0;

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	return true;
};
/*
bool Mesh::InitializeBuffers(ID3D10Device* device,aiMesh* mesh)
{
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned int n=0;
	mIndexCount = mesh->mNumFaces*3;
	if(mesh->HasTextureCoords(0))
	{
		hasUVs=true;
		TextureVertex* vertices;
		// Create the vertex array.
		vertices = new TextureVertex[mVertexCount];
		if(!vertices)
		{
			return false;
		}
		// Load the vertex array with data.
		do
		{
			vertices[n].position = D3DXVECTOR3(
				mesh->mVertices[n].x,mesh->mVertices[n].y,mesh->mVertices[n].z);
			vertices[n].texture = D3DXVECTOR2(
				mesh->mTextureCoords[0][n].x,1.0f-mesh->mTextureCoords[0][n].y);
			vertices[n].normal = D3DXVECTOR3(
				mesh->mNormals[n].x,mesh->mNormals[n].y,mesh->mNormals[n].z);
			n++;
		}while(n<mesh->mNumVertices);
		n=0;
		// Create the index array.
		//the size of faces times three since one face should have 3 indices
		indices = new unsigned long[mesh->mNumFaces*3];
		if(!indices)
		{
			return false;
		}
		// Load the index array with data.
		do
		{
			unsigned int n2 = 0;
			//flip winding order
			do
			{
				indices[(n*3)+n2]=mesh->mFaces[n].mIndices[n2];
				n2++;
			}while(n2<mesh->mFaces[n].mNumIndices);
			n++;
		}while(n<mesh->mNumFaces);
		n=0;
		positions = new D3DXVECTOR3[mIndexCount];
		do
		{
			positions[n] = vertices[indices[n]].position;
			n++;
		}while(n<mIndexCount);
		// Set up the description of the vertex buffer.
		vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(TextureVertex) * mVertexCount;
		vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;

		// Now finally create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
		if(FAILED(result))
		{
			return false;
		}
		delete [] vertices;
		vertices=0;
	}
	else
	{
		hasUVs=false;
		ColorVertex* vertices;
		// Create the vertex array.
		vertices = new ColorVertex[mVertexCount];
		if(!vertices)
		{
			return false;
		}
		// Load the vertex array with data.
		do
		{
			vertices[n].position = D3DXVECTOR3(mesh->mVertices[n].x,mesh->mVertices[n].y,mesh->mVertices[n].z);
			vertices[n].normal = D3DXVECTOR3(mesh->mNormals[n].x,mesh->mNormals[n].y,mesh->mNormals[n].z);
			n++;
		}while(n<mesh->mNumVertices);
		n=0;

		// Create the index array.
		//the size of faces times three since one face should have 3 indices
	
		indices = new unsigned long[mesh->mNumFaces*3];
		if(!indices)
		{
			return false;
		}
		// Load the index array with data.
		bool t = false;
		do
		{
			unsigned int n2 = 0;
			unsigned int n3 =2;
			//flip winding order
			do
			{
				indices[(n*3)+n2]=mesh->mFaces[n].mIndices[n3];
				n2++;
				n3--;
			}while(n2<mesh->mFaces[n].mNumIndices);
			t=false;
			n++;
		}while(n<mesh->mNumFaces);
		n=0;
		positions = new D3DXVECTOR3[mIndexCount];
		do
		{
			positions[n] = vertices[indices[n]].position;
			n++;
		}while(n<mIndexCount);
		// Set up the description of the vertex buffer.
		vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * mVertexCount;
		vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;

		// Now finally create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
		if(FAILED(result))
		{
			return false;
		}
		delete [] vertices;
		vertices=0;
	}
	n=0;

	box = new BoundingBox();
	box->fromPoints(positions,mIndexCount);
	hasbox=true;
	/*
	BoundingSphere* s = new BoundingSphere();
	s->fromPoints(positions,mIndexCount);
	float sv = s->Volume();
	float bv = b->Volume();
	if(s->Volume()<b->Volume())
	{
		sphere = s;
		delete b;
		hasbox=false;
	}
	else
	{
		box = b;
		delete s;
		hasbox = true;
	}

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the array now that the index buffer has been created and loaded.
	delete [] indices;
	indices = 0;
	return true;
}

bool Mesh::InitializeBoneBuffers(ID3D10Device* device,aiMesh* mesh)
{
	hasBones=true;
	BoneVertex* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	// Create the vertex array.
	vertices = new BoneVertex[mVertexCount];
	if(!vertices)
	{
		return false;
	}
	// Create the index array.
	//the size of faces times three since one face should have 3 indices
	mIndexCount = mesh->mNumFaces*3;
	indices = new unsigned long[mesh->mNumFaces*3];
	if(!indices)
	{
		return false;
	}
	// Load the vertex array with data.
	unsigned int n=0;
	do
	{
		vertices[n].position = D3DXVECTOR3(mesh->mVertices[n].x,mesh->mVertices[n].y,mesh->mVertices[n].z);
		vertices[n].texture = D3DXVECTOR2(mesh->mTextureCoords[0][n].x,1.0f-mesh->mTextureCoords[0][n].y);
		vertices[n].normal = D3DXVECTOR3(mesh->mNormals[n].x,mesh->mNormals[n].y,mesh->mNormals[n].z);
		n++;
	}while(n<mesh->mNumVertices);
	n=0;
	//we assigne vertex weights and indices to zero by default
	do
	{
		vertices[n].weights.x =0.0f;
		vertices[n].weights.y =0.0f;
		vertices[n].weights.z =0.0f;
		vertices[n].weights.w =0.0f;
		vertices[n].indices.x=0.0f;
		vertices[n].indices.y=0.0f;
		vertices[n].indices.z=0.0f;
		vertices[n].indices.w=0.0f;
		n++;
	}while(n<mVertexCount);
	n=0;
	// Load the index array with data.
		do
		{
			unsigned int n2 = 0;
			//flip winding order
			do
			{
				indices[(n*3)+n2]=mesh->mFaces[n].mIndices[n2];
				n2++;
			}while(n2<mesh->mFaces[n].mNumIndices);
			n++;
		}while(n<mesh->mNumFaces);
		n=0;
	positions = new D3DXVECTOR3[mIndexCount];
	do
	{
		positions[n] = vertices[indices[n]].position;
		n++;
	}while(n<mIndexCount);
	box = new BoundingBox();
	box->fromPoints(positions,mIndexCount);
	hasbox=true;

	//we set the bone indices and weights
	unsigned int i =0;
	n=0;
	do
	{
		i=0;
		do
		{
			unsigned int index = Bones[n]->indices[i];
			if(vertices[index].weights.x==0.0f)
			{
			vertices[index].weights.x = Bones[n]->weights[i];
			vertices[index].indices.x=(float)n+1;
			}
			else
			if(vertices[index].weights.y==0.0f)
			{
			vertices[index].weights.y = Bones[n]->weights[i];
			vertices[index].indices.y=(float)n+1;
			}
			else
			if(vertices[index].weights.z==0.0f)
			{
			vertices[index].weights.z = Bones[n]->weights[i];
			vertices[index].indices.z=(float)n+1;
			}
			else
			if(vertices[index].weights.w==0.0f)
			{
			vertices[index].weights.w = Bones[n]->weights[i];
			vertices[index].indices.w=(float)n+1;
			}
			else
				i=0;//the vertex is effected by more than 4 bones not accepted
			i++;
		}while(i!=Bones[n]->numWeights);
		n++;
	}while(n<boneCount);
	n=0;
	// Set up the description of the vertex buffer.
    vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(BoneVertex) * mVertexCount;
    vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;
	delete [] indices;
	indices = 0;
	return true;
}
*/
void Mesh::DisposeBuffers()
{
	// Release the index buffer.
	if(mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}
	// Release the vertex buffer.
	if(mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	return;
}

void Mesh::RenderBuffers(Mesh* m,ID3D10Device* device)
{
	unsigned int stride;
	unsigned int offset;
	if(m->hasBones==false)
	{
		if(m->hasUVs)
			stride = sizeof(TextureVertex); 
		else
			stride = sizeof(ColorVertex); 
		offset = 0;
	}
	else
	{
		// Set vertex buffer stride and offset.
		stride = sizeof(BoneVertex); 
		offset = 0;
	};
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &m->mVertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    device->IASetIndexBuffer(m->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Mesh::GenQuads()
{
	if(this->hMap==this->ray)
	{
		quad.Initialize(positions,this->mIndexCount,this->box);
	}
}

bool Mesh::Intersects(Mesh* m,BoundingBox* b)
{
	if(BoundingBox::Intersects(m->box,b))
		return true;
	else
		return false;
}

bool Mesh::Intersects(Mesh* m,Ray* r,float* d)
{
	if(m->hMap==Mesh::ray)
	{
	*d = 1.0737418e+008f;
	bool intercepts=false;
	r->TransformInverse(GetMatrix(m));
	if(Quad::Intersects(&m->quad,r,d))
	{	
		return true;
	}
	else
		return false;
	}
	return false;
}

bool Mesh::Intersects(Mesh* m,Ray* r,float dist,float* d)
{
	if(m->hMap==Mesh::ray)
	{
	*d = 1.0737418e+008f;
	bool intercepts=false;
	r->TransformInverse(GetMatrix(m));
	if(Quad::Intersects(&m->quad,r,dist,d))
	{	
		return true;
	}
	else
		return false;
	}
	return false;
}