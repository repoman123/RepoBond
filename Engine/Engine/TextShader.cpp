////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextShader.h"


TextShader::TextShader()
{
	m_effect = 0;
	m_technique = 0;
	m_layout = 0;
	m_viewMatrixPtr = 0;
	m_orthoMatrixPtr = 0;
	//m_textureArPtr = 0;
	m_texturePtr = 0;
	textColorPtr=0;
}

TextShader::TextShader(const TextShader& other)
{
}
TextShader::~TextShader()
{
}

bool TextShader::Initialize(ID3D10Device* device, HWND hwnd)
{
	bool result;
	// Initialize the shader that will be used to draw the triangle.
	result = InitializeShader(device, hwnd, L"../Engine/Shaders/text.fx");
	if(!result)
	{
		return false;
	}
	SetTextColor(D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
	return true;
}

void TextShader::Dispose()
{
	// Shutdown the shader effect.
	DisposeShader();

	return;
}

void TextShader::Render(ID3D10Device* device,unsigned int l)
{
	RenderShader(device,l);
	return;
}

bool TextShader::InitializeShader(ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	unsigned int numElements;
    D3D10_PASS_DESC passDesc;


	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
										device, NULL, NULL, &m_effect, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Get a pointer to the technique inside the shader.
	m_technique = m_effect->GetTechniqueByName("SpriteTechnique");
	if(!m_technique)
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	const D3D10_INPUT_ELEMENT_DESC polygonLayout[] =
    {
		{ "LOCATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
		{ "CHARSIZE", 0, DXGI_FORMAT_R32G32_FLOAT,    1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
        { "UV",       0, DXGI_FORMAT_R32G32_FLOAT,    1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
    };

	// Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Get the description of the first pass described in the shader technique.
    m_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Get pointers to the three matrices inside the shader so we can update them from this class.
	m_viewMatrixPtr = m_effect->GetVariableByName("viewMatrix")->AsMatrix();
    m_orthoMatrixPtr = m_effect->GetVariableByName("orthoMatrix")->AsMatrix();

	// Get pointer to the texture resource inside the shader.
	m_texturePtr = m_effect->GetVariableByName("shaderTexture")->AsShaderResource();
	textColorPtr = m_effect->GetVariableByName("textColor")->AsVector();
	texSPtr = m_effect->GetVariableByName("texS")->AsVector();
	fontSPtr = m_effect->GetVariableByName("fontSize")->AsScalar();;
	return true;
}

void TextShader::DisposeShader()
{
	// Release the pointer to the texture in the shader file.
	m_texturePtr=0;

	// Release the pointers to the matrices inside the shader.
	m_viewMatrixPtr = 0;
	m_orthoMatrixPtr = 0;

	// Release the pointer to the shader layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pointer to the shader technique.
	m_technique = 0;

	// Release the pointer to the shader.
	if(m_effect)
	{
		m_effect->Release();
		m_effect = 0;
	}

	return;
}

void TextShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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

void TextShader::SetView(D3DXMATRIX* viewMatrix)
{
	// Set the world matrix variable inside the shader.
    m_viewMatrixPtr->SetMatrix((float*)viewMatrix);
	return;
}

void TextShader::SetOrtho(D3DXMATRIX* orthoMatrix)
{
	// Set the world matrix variable inside the shader.
    m_orthoMatrixPtr->SetMatrix((float*)orthoMatrix);
	return;
}

void TextShader::SetTextColor(D3DXVECTOR4 color)
{
	//set the color
	textColorPtr->SetFloatVector((float*)color);
	return;
}

void TextShader::SetFontSize(float s)
{
	if(s!=currentFontSize)
	{
	fontSPtr->SetFloat(s);
	currentFontSize=s;
	}
}
//void TextShader::SetTextures(ID3D10ShaderResourceView** texs,unsigned int l)
//{
	// Set the texture variable inside the shader.
    //m_textureArPtr->SetResourceArray(texs,0,l);
	//return;
//}


void TextShader::SetTexture(TextureClass* tex)
{
	// Set the texture variable inside the shader.
    m_texturePtr->SetResource(tex->GetTexture());
	//set the texture texel size variable
	D3DXVECTOR2 v = D3DXVECTOR2(1.0f/(float)tex->w,1.0f/(float)tex->h);
	texSPtr->SetFloatVector((float*)v);
}

void TextShader::RenderShader(ID3D10Device* device,unsigned int l)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(m_layout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    m_technique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        m_technique->GetPassByIndex(i)->Apply(0);
        device->DrawInstanced(4,l,0,0);
    }

	return;
}