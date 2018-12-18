////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.cpp
////////////////////////////////////////////////////////////////////////////////
#include "D3D10Shaders.h"


Shaders::Shaders()
{
	effect = 0;
	boneTechnique = 0;
	boneLayout = 0;
	texCount = 0;
	m_worldMatrixPtr = 0;
	WVPmatrixPtr=0;
	texturesPtr = 0;
	texIndexPtr=0;
	m_lightDirectionPtr = 0;
	m_ambientColorPtr = 0;
	m_diffuseColorPtr = 0;
	m_cameraPositionPtr = 0;
	m_specularColorPtr = 0;
	m_specularPowerPtr = 0;
	difColorPtr=0;
}

Shaders::Shaders(const Shaders& other)
{
}
Shaders::~Shaders()
{
}


bool Shaders::Initialize(ID3D10Device* device, HWND hwnd)
{
	bool result;
	texs=new TextureArray();

	// Initialize the shader that will be used to draw the triangle.
	result = InitializeShader(device, hwnd, L"../Engine/Shaders.fx");
	if(!result)
	{
		return false;
	}

	return true;
}


void Shaders::Dispose()
{
	// Shutdown the shader effect.
	DisposeShaders();

	return;
}


void Shaders::BoneRender(ID3D10Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
			      unsigned int texIndex, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
			      D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,D3DXMATRIX* boneTransforms,unsigned int boneCount)
{
	// Set the shader parameters that it will use for rendering.
	//error with man happens in set shader parameters
	SetBoneParameters(*worldMatrix, *viewMatrix, *projectionMatrix,texIndex, lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor,
			    specularPower,boneTransforms,boneCount);
	// Now render the prepared buffers with the shader.
	RenderBone(device, indexCount);

	return;
}

void Shaders::LightRender(ID3D10Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
			      unsigned int texIndex, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
			      D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	// Set the shader parameters that it will use for rendering.
	//error with man happens in set shader parameters
	SetLightParameters(*worldMatrix, *viewMatrix, *projectionMatrix,texIndex, lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor,
			    specularPower);
	// Now render the prepared buffers with the shader.
	RenderLight(device, indexCount);

	return;
}

void Shaders::ColorRender(ID3D10Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
			      D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
			      D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,D3DXVECTOR4 color)
{
	// Set the shader parameters that it will use for rendering.
	//error with man happens in set shader parameters
	SetColorParameters(*worldMatrix, *viewMatrix, *projectionMatrix,lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor,
			    specularPower,color);
	// Now render the prepared buffers with the shader.
	RenderColor(device, indexCount);

	return;
}

void Shaders::SimpleColorRender(ID3D10Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix,D3DXVECTOR4 color)
{
	// Set the shader parameters that it will use for rendering.
	//error with man happens in set shader parameters
	SetSimpleColorParams(*worldMatrix, *viewMatrix, *projectionMatrix,color);
	// Now render the prepared buffers with the shader.
	RenderSimpleColor(device, indexCount);

	return;
}

bool Shaders::InitializeShader(ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
										device, NULL, NULL, &effect, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	D3D10_PASS_DESC bonePassDesc;
	// Get a pointer to the technique inside the shader.
	boneTechnique = effect->GetTechniqueByName("BoneTechnique");
	if(!boneTechnique)
	{
		return false;
	}
	const D3D10_INPUT_ELEMENT_DESC bonePolygonLayout[] =
    {
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,           0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",       0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, 48, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
	// Get the description of the first pass described in the shader technique.
    boneTechnique->GetPassByIndex(0)->GetDesc(&bonePassDesc);
	// Create the input layout.
    result = device->CreateInputLayout(bonePolygonLayout, 5, bonePassDesc.pIAInputSignature, bonePassDesc.IAInputSignatureSize, &boneLayout);
	if(FAILED(result))
	{
		return false;
	}

	D3D10_PASS_DESC lightPassDesc;
	// Get a pointer to the technique inside the shader.
	lightTechnique = effect->GetTechniqueByName("LightTechnique");
	if(!lightTechnique)
	{
		return false;
	}
	const D3D10_INPUT_ELEMENT_DESC lightPolygonLayout[] =
    {
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,           0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
	// Get the description of the first pass described in the shader technique.
    lightTechnique->GetPassByIndex(0)->GetDesc(&lightPassDesc);
	// Create the input layout.
    result = device->CreateInputLayout(lightPolygonLayout, 3, lightPassDesc.pIAInputSignature, lightPassDesc.IAInputSignatureSize, &lightLayout);
	if(FAILED(result))
	{
		return false;
	}

	D3D10_PASS_DESC colorPassDesc;
	// Get a pointer to the technique inside the shader.
	colorTechnique = effect->GetTechniqueByName("ColorTechnique");
	if(!colorTechnique)
	{
		return false;
	}
	const D3D10_INPUT_ELEMENT_DESC colorPolygonLayout[] =
    {
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
	// Get the description of the first pass described in the shader technique.
    colorTechnique->GetPassByIndex(0)->GetDesc(&colorPassDesc);
	// Create the input layout.
    result = device->CreateInputLayout(colorPolygonLayout, 2, colorPassDesc.pIAInputSignature, colorPassDesc.IAInputSignatureSize, &colorLayout);
	if(FAILED(result))
	{
		return false;
	}

	D3D10_PASS_DESC simpleColorPassDesc;
	// Get a pointer to the technique inside the shader.
	simpleColorTechnique = effect->GetTechniqueByName("SimpleColorTechnique");
	if(!simpleColorTechnique)
	{
		return false;
	}
	const D3D10_INPUT_ELEMENT_DESC simpleColorPolygonLayout[] =
    {
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,        0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
	// Get the description of the first pass described in the shader technique.
    simpleColorTechnique->GetPassByIndex(0)->GetDesc(&simpleColorPassDesc);
	// Create the input layout.
    result = device->CreateInputLayout(simpleColorPolygonLayout, 1, simpleColorPassDesc.pIAInputSignature, simpleColorPassDesc.IAInputSignatureSize, &simpleColorLayout);
	if(FAILED(result))
	{
		return false;
	}
	D3D10_PASS_DESC spritePassDesc;
	//the mapping none technique
	spriteTechnique = effect->GetTechniqueByName("SpriteTechnique");
	if(!spriteTechnique)
	{
		return false;
	}
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	const D3D10_INPUT_ELEMENT_DESC polygonLayout[] =
    {
		{ "LOCATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
        { "TEXS",       0, DXGI_FORMAT_R32G32B32_FLOAT,    1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
		{ "SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
    };
	// Get the description of the first pass described in the shader technique.
    spriteTechnique->GetPassByIndex(0)->GetDesc(&spritePassDesc);

	// Create the input layout.
    result = device->CreateInputLayout(polygonLayout,3, spritePassDesc.pIAInputSignature, spritePassDesc.IAInputSignatureSize, &spriteLayout);
	if(FAILED(result))
	{
		return false;
	}
	// Get pointers to the three matrices inside the shader so we can update them from this class.
    worldMatrixPtr = effect->GetVariableByName("worldMatrix")->AsMatrix();
	viewMatrixPtr = effect->GetVariableByName("viewMatrix")->AsMatrix();
    orthoMatrixPtr = effect->GetVariableByName("orthoMatrix")->AsMatrix();
	texIndexPtr = effect->GetVariableByName("texIndex")->AsScalar();
	// Get pointers to the three matrices inside the shader so we can update them from this class.
    m_worldMatrixPtr = effect->GetVariableByName("worldMatrix")->AsMatrix();
	WVPmatrixPtr = effect->GetVariableByName("WVPmatrix")->AsMatrix();

	// Get pointer to the texture resource inside the shader.
	texturesPtr = effect->GetVariableBySemantic("shaderTextures")->AsShaderResource();
	texturePtr = effect->GetVariableByName("shaderTexture")->AsShaderResource();
	// Get pointers to the light direction and diffuse color variables inside the shader.
	m_lightDirectionPtr = effect->GetVariableByName("lightDirection")->AsVector();
	m_ambientColorPtr = effect->GetVariableByName("LightAmbColor")->AsVector();
	m_diffuseColorPtr = effect->GetVariableByName("LightDifColor")->AsVector();
	difColorPtr = effect->GetVariableByName("difColor")->AsVector();
	// Get pointers to the specular light components inside the shader.
	m_cameraPositionPtr = effect->GetVariableByName("cameraPosition")->AsVector();
	m_specularColorPtr = effect->GetVariableByName("LightSpeColor")->AsVector();
	m_specularPowerPtr = effect->GetVariableByName("LightSpePower")->AsScalar();
	m_boneTransformsPtr = effect->GetVariableByName("boneTransforms")->AsMatrix();
	return true;
}

void Shaders::DisposeShaders()
{
	// Release the light pointers.
	m_cameraPositionPtr = 0;
	m_specularColorPtr = 0;
	m_specularPowerPtr = 0;
	m_lightDirectionPtr = 0;
	m_ambientColorPtr = 0;
	m_diffuseColorPtr = 0;

	// Release the pointer to the texture in the shader file.
	texturesPtr=0;

	// Release the pointers to the matrices inside the shader.
	m_worldMatrixPtr = 0;

	// Release the pointer to the shader layout.
	if(boneLayout)
	{
		boneLayout->Release();
		boneLayout = 0;
	}

	// Release the pointer to the shader technique.
	boneTechnique = 0;

	// Release the pointer to the shader layout.
	if(lightLayout)
	{
		lightLayout->Release();
		lightLayout = 0;
	}

	// Release the pointer to the shader technique.
	lightTechnique = 0;

	// Release the pointer to the shader layout.
	if(colorLayout)
	{
		colorLayout->Release();
		colorLayout = 0;
	}

	// Release the pointer to the shader technique.
	colorTechnique = 0;

	// Release the pointer to the shader layout.
	if(simpleColorLayout)
	{
		simpleColorLayout->Release();
		simpleColorLayout = 0;
	}

	// Release the pointer to the shader technique.
	simpleColorTechnique = 0;

	// Release the pointer to the shader.
	if(effect)
	{
		effect->Release();
		effect = 0;
	}

	return;
}

void Shaders::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void Shaders::SetBoneParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
					   unsigned int texIndex, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, 
					   D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,D3DXMATRIX* boneTrasforms,unsigned int boneCount)
{
	// Set the world matrix variable inside the shader.
    m_worldMatrixPtr->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	D3DXMATRIX mat = worldMatrix*viewMatrix*projectionMatrix;
	WVPmatrixPtr->SetMatrix((float*)&mat);

	texturePtr->SetResource(textures[texIndex]->GetTexture());

	// Set the direction of the light inside the shader.
	m_lightDirectionPtr->SetFloatVector((float*)&lightDirection);

	// Set the ambient color of the light.
	m_ambientColorPtr->SetFloatVector((float*)&ambientColor);

	// Set the diffuse color of the light inside the shader.
	m_diffuseColorPtr->SetFloatVector((float*)&diffuseColor);

	// Set the position of the camera.
	m_cameraPositionPtr->SetFloatVector((float*)&cameraPosition);

	// Set the specular color of the light.
	m_specularColorPtr->SetFloatVector((float*)&specularColor);

	// Set the specular power of the light.
	m_specularPowerPtr->SetFloat(specularPower);
	m_boneTransformsPtr->SetMatrixArray((float*)boneTrasforms,0,boneCount+1);
	return;
}
void Shaders::SetColorParameters(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,
									D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
									D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,D3DXVECTOR4 color)
{
	WVPmatrixPtr->SetMatrix(world*view*proj);
	// Set the world matrix variable inside the shader.
    m_worldMatrixPtr->SetMatrix((float*)&world);

	// Set the mat color variable inside the shader
	difColorPtr->SetFloatVector((float*)color);

	// Set the view matrix variable inside the shader.
	D3DXMATRIX mat = world*view*proj;
	WVPmatrixPtr->SetMatrix((float*)&mat);

	// Set the direction of the light inside the shader.
	m_lightDirectionPtr->SetFloatVector((float*)&lightDirection);

	// Set the ambient color of the light.
	m_ambientColorPtr->SetFloatVector((float*)&ambientColor);

	// Set the diffuse color of the light inside the shader.
	m_diffuseColorPtr->SetFloatVector((float*)&diffuseColor);

	// Set the position of the camera.
	m_cameraPositionPtr->SetFloatVector((float*)&cameraPosition);

	// Set the specular color of the light.
	m_specularColorPtr->SetFloatVector((float*)&specularColor);

	// Set the specular power of the light.
	m_specularPowerPtr->SetFloat(specularPower);


	return;
}
void Shaders::SetLightParameters(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, 
					   unsigned int texIndex, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, 
					   D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	WVPmatrixPtr->SetMatrix(world*view*proj);
	// Set the world matrix variable inside the shader.
    m_worldMatrixPtr->SetMatrix((float*)&world);

	// Set the view matrix variable inside the shader.
	D3DXMATRIX mat = world*view*proj;
	WVPmatrixPtr->SetMatrix((float*)&mat);

	texturePtr->SetResource(textures[texIndex]->GetTexture());

	// Set the direction of the light inside the shader.
	m_lightDirectionPtr->SetFloatVector((float*)&lightDirection);

	// Set the ambient color of the light.
	m_ambientColorPtr->SetFloatVector((float*)&ambientColor);

	// Set the diffuse color of the light inside the shader.
	m_diffuseColorPtr->SetFloatVector((float*)&diffuseColor);

	// Set the position of the camera.
	m_cameraPositionPtr->SetFloatVector((float*)&cameraPosition);

	// Set the specular color of the light.
	m_specularColorPtr->SetFloatVector((float*)&specularColor);

	// Set the specular power of the light.
	m_specularPowerPtr->SetFloat(specularPower);
	return;
}
void Shaders::SetSimpleColorParams(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,D3DXVECTOR4 color)
{
	// Set the world matrix variable inside the shader.
    m_worldMatrixPtr->SetMatrix((float*)&world);

	// Set the mat color variable inside the shader
	difColorPtr->SetFloatVector((float*)&color);

	// Set the view matrix variable inside the shader.
	D3DXMATRIX mat = world*view*proj;
	WVPmatrixPtr->SetMatrix((float*)&mat);
	return;
}

void Shaders::RenderBone(ID3D10Device* device, int indexCount)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(boneLayout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    boneTechnique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        boneTechnique->GetPassByIndex(i)->Apply(0);
        device->DrawIndexed(indexCount, 0, 0);
    }

	return;
}

void Shaders::RenderLight(ID3D10Device* device, int indexCount)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(lightLayout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    lightTechnique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        lightTechnique->GetPassByIndex(i)->Apply(0);
        device->DrawIndexed(indexCount, 0, 0);
    }

	return;
}

void Shaders::RenderColor(ID3D10Device* device, int indexCount)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(colorLayout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    colorTechnique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        colorTechnique->GetPassByIndex(i)->Apply(0);
        device->DrawIndexed(indexCount, 0, 0);
    }

	return;
}

void Shaders::RenderSimpleColor(ID3D10Device* device, int indexCount)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;

	// Set the input layout.
	device->IASetInputLayout(simpleColorLayout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    simpleColorTechnique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        simpleColorTechnique->GetPassByIndex(i)->Apply(0);
        device->DrawIndexed(indexCount, 0, 0);
    }

	return;
}

void Shaders::SetWorld(D3DXMATRIX* worldMatrix)
{
	// Set the world matrix variable inside the shader.
    worldMatrixPtr->SetMatrix((float*)worldMatrix);
	return;
}

void Shaders::SetView(D3DXMATRIX* viewMatrix)
{
	// Set the world matrix variable inside the shader.
    viewMatrixPtr->SetMatrix((float*)viewMatrix);
	return;
}

void Shaders::SetOrtho(D3DXMATRIX* orthoMatrix)
{
	// Set the world matrix variable inside the shader.
    orthoMatrixPtr->SetMatrix((float*)orthoMatrix);
	return;
}

void Shaders::SetTextures()
{
	this->texturesPtr->SetResource(texs->ppSRV);
}

void Shaders::RenderSprites(ID3D10Device* device,unsigned int spCount)
{
	if(spTexCount!=texs->GetTexCount())
	{
		HRESULT h = texs->UpdateTextureArray(device);
		if(!h==S_OK)
			int test=0;
		SetTextures();
		spTexCount=texs->GetTexCount();
	}
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(spriteLayout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    spriteTechnique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        spriteTechnique->GetPassByIndex(i)->Apply(0);
        device->DrawInstanced(4,spCount,0,0);
    }

	return;
}