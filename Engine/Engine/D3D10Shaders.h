////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D3D10_Shaders
#define D3D10_Shaders


//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10math.h>
#include <fstream>
#include <map>
#include "TextureArray.h"
#include "textureclass.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: Shaders
////////////////////////////////////////////////////////////////////////////////
class Shaders
{
public:
	Shaders();
	Shaders(const Shaders&);
	~Shaders();
	bool Initialize(ID3D10Device*, HWND);
	void Dispose();
	void BoneRender(ID3D10Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float,D3DXMATRIX* boneTrasforms,unsigned int boneCount);
	void LightRender(ID3D10Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float);
	void ColorRender(ID3D10Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
		    D3DXVECTOR4, float,D3DXVECTOR4);
	void SimpleColorRender(ID3D10Device*, int, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*,D3DXVECTOR4);

	////2D STUFF
	void RenderSprites(ID3D10Device*,unsigned int spCount);
	void SetWorld(D3DXMATRIX*);
	void SetView(D3DXMATRIX*);
	void SetOrtho(D3DXMATRIX*);
	void SetTextures();
	TextureArray* texs;
	TextureClass* texture;
	unordered_map<unsigned int,TextureClass*> textures;
	unsigned int spTexCount;
	unsigned int texCount;

	bool InitializeShader(ID3D10Device*, HWND, WCHAR*);
	void DisposeShaders();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	void SetBoneParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
				 D3DXVECTOR4, float,D3DXMATRIX* boneTrasforms,unsigned int boneCount);
	void SetLightParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,unsigned int, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, 
				 D3DXVECTOR4, float);
	void SetColorParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4,
									D3DXVECTOR3, D3DXVECTOR4, float,D3DXVECTOR4);
	void SetSimpleColorParams(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,D3DXVECTOR4);
	void RenderBone(ID3D10Device*, int);
	void RenderLight(ID3D10Device*, int);
	void RenderColor(ID3D10Device*, int);
	void RenderSimpleColor(ID3D10Device*, int);

	ID3D10Effect* effect;
	ID3D10EffectTechnique* boneTechnique;
	ID3D10EffectTechnique* lightTechnique;
	ID3D10EffectTechnique* colorTechnique;
	ID3D10EffectTechnique* simpleColorTechnique;
	ID3D10EffectTechnique* spriteTechnique;
	ID3D10InputLayout* boneLayout;
	ID3D10InputLayout* lightLayout;
	ID3D10InputLayout* colorLayout;
	ID3D10InputLayout* simpleColorLayout;
	ID3D10InputLayout* spriteLayout;
	ID3D10EffectMatrixVariable* m_worldMatrixPtr;
	ID3D10EffectMatrixVariable* m_boneTransformsPtr;
	ID3D10EffectMatrixVariable* WVPmatrixPtr;
	ID3D10EffectMatrixVariable* worldMatrixPtr;
	ID3D10EffectMatrixVariable* viewMatrixPtr;
	ID3D10EffectMatrixVariable* orthoMatrixPtr;
	ID3D10EffectShaderResourceVariable* texturesPtr;
	ID3D10EffectShaderResourceVariable* texturePtr;
	ID3D10EffectVectorVariable* m_lightDirectionPtr;
	ID3D10EffectVectorVariable* m_ambientColorPtr;
	ID3D10EffectVectorVariable* m_diffuseColorPtr;
	ID3D10EffectVectorVariable* difColorPtr;
	ID3D10EffectVectorVariable* m_cameraPositionPtr;
	ID3D10EffectVectorVariable* m_specularColorPtr;
	ID3D10EffectScalarVariable* m_specularPowerPtr;
	ID3D10EffectScalarVariable* texIndexPtr;
	
};

#endif