#ifndef D3D11_Shaders
#define D3D11_Shaders

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <D3D10.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <map>
#include "textureclass.h"

class Shaders
{
public:
	Shaders();
	Shaders(const Shaders&);
	~Shaders();
	bool Initialize(ID3D11Device*, HWND);
	unsigned int texCount;
	void Dispose();
	void BoneRender(ID3D11Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float,D3DXMATRIX* boneTrasforms,unsigned int boneCount);
	void LightRender(ID3D11Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float);
	void ColorRender(ID3D11Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float,D3DXVECTOR4);
	void AddTexture(TextureClass*);
	void UpdateTextures();
	map<unsigned int,TextureClass*> textures;
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	void DisposeShaders();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	void SetBoneParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
				 D3DXVECTOR4, float,D3DXMATRIX* boneTrasforms,unsigned int boneCount);
	void SetLightParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
				 D3DXVECTOR4, float);
	void SetColorParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4,
									D3DXVECTOR3, D3DXVECTOR4, float,D3DXVECTOR4);
	void RenderBone(ID3D11Device*, int);
	void RenderLight(ID3D11Device*, int);
	void RenderColor(ID3D11Device*, int);
private:
	
	ID3D11InputLayout* boneLayout;
	ID3D11InputLayout* lightLayout;
	ID3D11InputLayout* colorLayout;
};

#endif
