////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10.h>
#include <cstring>
#include <string>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	bool Initialize(ID3D10Device*, const wchar_t*);
	bool Initialize(ID3D10Device*, wstring*);
	bool Initialize(ID3D10Device*, const char*);
	void Dispose();
	wstring name;
	string cname;
	const wchar_t* path;
	ID3D10ShaderResourceView* GetTexture();
	unsigned int h,w;
private:
	ID3D10ShaderResourceView* m_texture;
};
#endif