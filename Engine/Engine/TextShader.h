
////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTSHADER_H
#define TEXTSHADER_H
//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10math.h>
#include <fstream>
#include "textureclass.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: TextShader
////////////////////////////////////////////////////////////////////////////////
class TextShader
{
public:
	TextShader();
	TextShader(const TextShader&);
	~TextShader();

	bool Initialize(ID3D10Device*, HWND);
	void Dispose();
	void Render(ID3D10Device*,unsigned int l);
	void SetWorld(D3DXMATRIX*);
	void SetView(D3DXMATRIX*);
	void SetOrtho(D3DXMATRIX*);
	//void SetTextures(ID3D10ShaderResourceView**,unsigned int);
	void SetTexture(TextureClass*);
	void SetTextColor(D3DXVECTOR4);
	void SetFontSize(float s);
	float currentFontSize;
private:
	bool InitializeShader(ID3D10Device*, HWND, WCHAR*);
	void DisposeShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void SetShaderParameters(D3DXMATRIX*, D3DXMATRIX, D3DXMATRIX,ID3D10ShaderResourceView*);
	void RenderShader(ID3D10Device*,unsigned int);
private:
	ID3D10Effect* m_effect;
	ID3D10EffectTechnique* m_technique;
	ID3D10InputLayout* m_layout;
	ID3D10EffectVectorVariable* textColorPtr;
	ID3D10EffectVectorVariable* texSPtr;
	ID3D10EffectMatrixVariable* m_viewMatrixPtr;
	ID3D10EffectMatrixVariable* m_orthoMatrixPtr;
	ID3D10EffectScalarVariable* fontSPtr;
	//ID3D10EffectShaderResourceVariable* m_textureArPtr;
	ID3D10EffectShaderResourceVariable* m_texturePtr;
	int w,h;
};
#endif