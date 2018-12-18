#ifndef TEXTUREARRAY_H
#define TEXTUREARRAY_H

#include <d3d10.h>
#include <D3DX10.h>
#include <DXGI.h>
#include <D3DX10tex.h>
#include <d3dx10math.h>
#include <unordered_map>

using namespace std;


class TextureArray
{
public:
	TextureArray(void);
	~TextureArray(void);
	HRESULT UpdateTextureArray(ID3D10Device* pd3dDevice);
	unsigned int GetTexCount();
	void AddTexture(const wchar_t*);
	void AddTexture(const char*);
	unordered_map<unsigned int,D3DXVECTOR2> texSizes;
	unordered_map<unsigned int,const wchar_t*> texturePaths;
	ID3D10Texture2D* ppTex2D;
	ID3D10ShaderResourceView* ppSRV;

private :
	unsigned int texCount;
};

#endif