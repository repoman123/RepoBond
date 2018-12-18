////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix boneTransforms[100];
matrix worldMatrix;
matrix viewMatrix;
matrix orthoMatrix;
matrix WVPmatrix; //world view projection matrix
Texture2DArray shaderTextures;
Texture2D shaderTexture;
float4 spriteColor;
float4 LightAmbColor;
float4 LightDifColor;
float3 lightDirection;
float3 cameraPosition;
float4 LightSpeColor;
float LightSpePower;
float4 difColor;
float texIndex;
///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState LinearClampSampling;

BlendState AlphaBlendingOn
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
    SrcBlend = SRC_ALPHA;
};

//////////////
// Bone TYPEDEFS //
//////////////
struct BoneVertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 weights : WEIGHTS;
	float4 bones : BONES;
};

struct LightVertexInputType
{
    float4 position : POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct LightPSInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
    float3 viewDirection : TEXCOORD1;
};
////////////////////////////////////////////////////////////////////////////////
//Bone Vertex Shader
////////////////////////////////////////////////////////////////////////////////
LightPSInputType BoneVertexShader(BoneVertexInputType input)
{
    LightPSInputType output;
    float4 worldPosition;

    matrix finalMatrix;
    finalMatrix = input.weights.x * boneTransforms[input.bones.x];
    finalMatrix += input.weights.y * boneTransforms[input.bones.y];
    finalMatrix += input.weights.z * boneTransforms[input.bones.z];
    finalMatrix += input.weights.w * boneTransforms[input.bones.w];    
    
    float4 vAnimatedPos = mul(float4(input.position.xyz,1),finalMatrix);
    output.normal = mul(float4(input.normal.xyz,0),finalMatrix);

	// Transform the position from object space to homogeneous projection space
    output.position = mul(vAnimatedPos,WVPmatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	input.position.w=0;
	// Calculate the position of the vertex in the world.
    worldPosition = mul(vAnimatedPos, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	return output;
}



////////////////////////////////////////////////////////////////////////////////
// Light Vertex Shader
////////////////////////////////////////////////////////////////////////////////
LightPSInputType LightVertexShader(LightVertexInputType input)
{
    LightPSInputType output;
    float4 worldPosition;
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position,WVPmatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Light Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(LightPSInputType input) : SV_Target
{
	//float3 tex = float3(input.tex.x*texIndex,input.tex.y*texIndex,texIndex);
	float3 textureColor = shaderTexture.Sample(SampleType,input.tex);
	float4 color = float4(textureColor,1.0f);
    return color;
}


////////////////////////////////////////////////////////////////////////////////
// Light Technique
////////////////////////////////////////////////////////////////////////////////
technique10 LightTechnique
{
    pass pass0
    {
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, LightVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, LightPixelShader()));
        SetGeometryShader(NULL);
    }
}
////////////////////////////////////////////////////////////////////////////////
//Bone Technique
////////////////////////////////////////////////////////////////////////////////
technique10 BoneTechnique
{
    pass pass0
    {
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, BoneVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, LightPixelShader()));
	    SetGeometryShader(NULL);
    }
}


//////////////
// Color TYPEDEFS //
//////////////
struct ColorVSInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct ColorPSInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD;
};
////////////////////////////////////////////////////////////////////////////////
// Color Vertex Shader
////////////////////////////////////////////////////////////////////////////////
ColorPSInputType ColorVertexShader(ColorVSInputType input)
{
    ColorPSInputType output;
    float4 worldPosition;
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position,WVPmatrix);
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(ColorPSInputType input) : SV_Target
{
	float3 lightDir;
	float lightIntensity;
	float4 color;
	 float3 reflection;
    float4 specular;

	// Set the default output color to the ambient light value for all pixels.
    color = LightAmbColor;

	 // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
	
	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (LightDifColor * lightIntensity);
		// Saturate the ambient and diffuse color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir); 

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), LightSpePower);
    }

    // Saturate the final light color.
    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * difColor;

	// Add the specular component last to the output color.
    color = saturate(color + specular);
    return color;
}


////////////////////////////////////////////////////////////////////////////////
// Color Technique
////////////////////////////////////////////////////////////////////////////////
technique10 ColorTechnique
{
    pass pass0
    {
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, ColorVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, ColorPixelShader()));
        SetGeometryShader(NULL);
    }
}

//////////////
// Simple Color TYPEDEFS //
//////////////
struct SimpleColorVSInputType
{
    float4 position : POSITION;
};

struct SimpleColorPSInputType
{
    float4 position : SV_POSITION;
};
////////////////////////////////////////////////////////////////////////////////
// Simple Color Vertex Shader
////////////////////////////////////////////////////////////////////////////////
SimpleColorPSInputType SimpleColorVertexShader(SimpleColorVSInputType input)
{
    SimpleColorPSInputType output;
    input.position.w = 1.0f;
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position,WVPmatrix);
 
	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Simple Color Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SimpleColorPixelShader(SimpleColorPSInputType input) : SV_Target
{
	float4 color=difColor;
	color.w = 1.0f;
    return difColor;
}


////////////////////////////////////////////////////////////////////////////////
// Simple Color Technique
////////////////////////////////////////////////////////////////////////////////
technique10 SimpleColorTechnique
{
    pass pass0
    {
	SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, SimpleColorVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, SimpleColorPixelShader()));
        SetGeometryShader(NULL);
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////// 2D SHADERS ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
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
struct spriteVertexInputType
{
	uint index : SV_VertexID;
	float3 location : LOCATION;
	float3 UV : TEXS;
	float2 size : SIZE;
};

struct spritePixelInputType
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
spritePixelInputType SpriteVertexShader(spriteVertexInputType input)
{
	spritePixelInputType output;
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
float4 SpritePixelShader(spritePixelInputType input) : SV_Target
{
	float4 textureColor;
	textureColor = shaderTextures.Sample(SampleType,input.tex);
	textureColor.w = 1.0f;
    return textureColor;
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