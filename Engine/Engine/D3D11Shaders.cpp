////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.cpp
////////////////////////////////////////////////////////////////////////////////
#include "D3D11Shaders.h"


Shaders::Shaders()
{
	boneLayout = 0;
	texCount = 0;
}

Shaders::Shaders(const Shaders& other)
{
}
Shaders::~Shaders()
{
}


bool Shaders::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


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


void Shaders::BoneRender(ID3D11Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
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

void Shaders::LightRender(ID3D11Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
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

void Shaders::ColorRender(ID3D11Device* device, int indexCount, D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projectionMatrix, 
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

bool Shaders::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* filename)
{
	
	return true;
}


void Shaders::DisposeShaders()
{

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
	
	return;
}
void Shaders::SetColorParameters(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,
									D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
									D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,D3DXVECTOR4 color)
{
	
	return;
}
void Shaders::SetLightParameters(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, 
					   unsigned int texIndex, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, 
					   D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{

}

void Shaders::RenderBone(ID3D11Device* device, int indexCount)
{
	return;
}

void Shaders::RenderLight(ID3D11Device* device, int indexCount)
{
	return;
}

void Shaders::RenderColor(ID3D11Device* device, int indexCount)
{
	return;
}

void Shaders::AddTexture(TextureClass* srv)
{
	
}

void Shaders::UpdateTextures()
{

}