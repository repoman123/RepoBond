#ifndef MATERIAL_H
#define MATERIAL_H

#include "textureclass.h"


class Material
{
public:

	Material(void);
	~Material(void);

	bool has_color;
	string name;
	unsigned int diffuseTex;
	unsigned int specularTex;
	unsigned int normalTex;
	unsigned int bumpTex;
	unsigned int opacityTex;
	unsigned int reflectionTex;
	unsigned int refractionTex;
	D3DXVECTOR4 color;
	D3DXVECTOR3 specularColor;
	D3DXVECTOR3 ambientColor;
private:
	
};
#endif