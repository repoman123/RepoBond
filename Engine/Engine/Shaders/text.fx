////////////////////////////////////////////////////////////////////////////////
// Filename: sprite.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix viewMatrix;
matrix orthoMatrix;
Texture2D shaderTexture : register(ps,t2);
float4 textColor;
float2 texS;//texture texel size (x=1/w,1/h)
float fontSize;
///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
/////////////////////
// BLENDING STATES //
/////////////////////
BlendState AlphaBlendingState
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	uint index : SV_VertexID;
	float3 location : LOCATION;
	float2 charSize : CHARSIZE;
	float2 charUV : UV;
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

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SpriteVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	//using the position we assigne the UV:s
	
	//top left = 0.0f/1.0f -> index 3
	output.tex = input.charUV;
	float4 pos = float4(0.0f,1.0f,0.0f,1.0f);

	if(input.index==0)
	{
		//top right = 1.0f/1.0f
		pos.x = 1.0f;
		pos.y = 1.0f;
		output.tex.x += texS.x * input.charSize.x;
	}
	else
	if(input.index==3)
	{
		//bottom left = 0.0f/0.0f
		output.tex.y += texS.y * input.charSize.y;
		pos.y = 0.0f;
	}
	else
	if(input.index==1)
	{
		//bottom right = 1.0f/0.0f
		output.tex.x += texS.x * input.charSize.x;
		output.tex.y += texS.y * input.charSize.y;
		pos.x=1.0f;
		pos.y=0.0f;
	}

	// Change the position vector to be 4 units for proper matrix calculations.
	

	//we scale the position according to size
	//by creating a scaling matrix from the custom Scaling function
	output.position = mul(pos,Scaling(input.charSize.x*fontSize,input.charSize.y*fontSize,0.0f));

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
		// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	if(textureColor.x >0.8f & textureColor.y >0.8f & textureColor.z >0.8f)
	{
		//we return white transparent
		return float4(1.0f,1.0f,1.0f,0.0f);
	}
	else
	{
		float4 color= saturate((textureColor)+textColor);
		return color;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 SpriteTechnique
{
    pass pass0
    {
		SetBlendState(AlphaBlendingState, float4(1.0f, 1.0f, 1.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, SpritePixelShader()));
        SetGeometryShader(NULL);
    }
}