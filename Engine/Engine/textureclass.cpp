////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}
TextureClass::TextureClass(const TextureClass& other)
{
}
TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D10Device* device, const wchar_t* filename)
{
	HRESULT result;
	// Load the texture in.
	path = filename;
	name.assign(filename);
	//92 code for '\'
	unsigned int ind =0;
	ind = name.find_last_of(92);
	if(ind==4294967295)//max value
		ind = name.find_last_of('/');
	name.erase(0,ind+1);
	result = D3DX10CreateShaderResourceViewFromFile(device,filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}
    ID3D10Resource* resource;
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	m_texture->GetResource(&resource);
	m_texture->GetDesc(&viewDesc);
	D3D10_TEXTURE2D_DESC desc2D;
    ((ID3D10Texture2D*)resource)->GetDesc(&desc2D);
    w = desc2D.Width;
    h = desc2D.Height;
	return true;
}

bool TextureClass::Initialize(ID3D10Device* device, wstring* str)
{
	return Initialize(device,str->data());
}

bool TextureClass::Initialize(ID3D10Device* device, const char* filename)
{
	unsigned int len = strlen(filename);
	wchar_t* ws = new wchar_t[len];
	MultiByteToWideChar(CP_ACP,0,filename,-1,ws,len);
	ws[len]=0;
	cname.assign(filename);
	return Initialize(device,ws);
}

void TextureClass::Dispose()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D10ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}