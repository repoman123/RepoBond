
////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef SPRITESHADER_H
#define SPRITESHADER_H
//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10math.h>
#include "TextureArray.h"
#include <fstream>

using namespace std;
#include <map>

////////////////////////////////////////////////////////////////////////////////
// Class name: SpriteShader
////////////////////////////////////////////////////////////////////////////////
class SpriteShader
{
public:
	SpriteShader();
	SpriteShader(const SpriteShader&);
	~SpriteShader();
	enum Mapping
	{
		none,
		strech,
	};
	bool Initialize(ID3D10Device*, HWND);
	Mapping currentMapping;
	void Dispose();
	void RenderSprites(ID3D10Device*,unsigned int spCount);
	void SetWorld(D3DXMATRIX*);
	void SetView(D3DXMATRIX*);
	void SetOrtho(D3DXMATRIX*);
	void SetTextures();
	TextureArray* texs;
	unsigned int texCount;
private:
	bool InitializeShader(ID3D10Device*, HWND, WCHAR*);
	void DisposeShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void SetShaderParameters(D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*);
private:
	ID3D10Effect* m_effect;
	ID3D10EffectTechnique* m_technique;
	ID3D10InputLayout* m_layout;
	ID3D10EffectVectorVariable* ColorPtr;
	ID3D10EffectScalarVariable* useColorPtr;
	ID3D10EffectScalarVariable* mappingPtr;
	ID3D10EffectMatrixVariable* m_worldMatrixPtr;
	ID3D10EffectMatrixVariable* m_viewMatrixPtr;
	ID3D10EffectMatrixVariable* m_orthoMatrixPtr;
	ID3D10EffectShaderResourceVariable* m_texturesPtr;
};
#endif