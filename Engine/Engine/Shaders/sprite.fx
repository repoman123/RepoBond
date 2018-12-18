////////////////////////////////////////////////////////////////////////////////
// Filename: sprite.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix worldMatrix;
matrix viewMatrix;
matrix orthoMatrix;
Texture2DArray shaderTextures : register(ps,t0);
float2 texS;
bool useColor;
float4 spriteColor;
uint mapping;
///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

/////////////////////
// BLENDING STATES //
/////////////////////
BlendState AlphaBlendingOn
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};

/////////////
// HELPERS //
/////////////
matrix Scaling(float x,float y,float z)
{
return matrix
(
   x,0.0f,0.0f,0.0f,
0.0f,   y,0.0f,0.0f,
0.0f,0.0f,   z,0.0f,
0.0f,0.0f,0.0f,1.0f
);
}

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	uint index : SV_VertexID;
	float3 location : LOCATION;
	float3 UV : TEXS;
	float2 size : SIZE;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SpriteVertexShader(VertexInputType input)
{
	PixelInputType output;
    //using the position we assigne the UV:s
	//top left = 0.0f/1.0f
	output.tex = input.UV;
	float4 pos = float4(0.0f,1.0f,0.0f,1.0f);

	//mapping stretch
	if(input.index == 0)
	{
		//top right = 1.0f/1.0f
		output.tex.x = 1.0f;
		output.tex.y = 0.0f;
		pos.x = 1.0f;
		pos.y = 1.0f;
	}
	else
	if(input.index == 3)
	{
		//bottom left = 0.0f/0.0f
		output.tex.x = 0.0f;
		output.tex.y = 1.0f;
		pos.y=0.0f;
	}
	else
	if(input.index == 1)
	{
		//bottom right = 1.0f/0.0f
		output.tex.x = 1.0f;
		output.tex.y = 1.0f;
		pos.x=1.0f;
		pos.y=0.0f;
	}

	//we scale the position according to size
	//by creating a scaling matrix from the custom Scaling function
	output.position = mul(pos,Scaling(input.size.x,input.size.y,0.0f));

	//we change the position of the quad according to the position parameter
	output.position.x += input.location.x;
	output.position.y += input.location.y;
	output.position.z += input.location.z;

	//then we finally multiply by the view and ortho matrices
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, orthoMatrix);
	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SpritePixelShader(PixelInputType input) : SV_Target
{
	//float4 textureColor;
	//textureColor = shaderTextures.Sample(SampleType,input.tex);
	//textureColor.w = 0.5f;
    //return textureColor;
	return float4(1.0f,0.0f,0.0f,1.0f);
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 SpriteTechnique
{
    pass pass0
    {
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, SpritePixelShader()));
        SetGeometryShader(NULL);
    }
}