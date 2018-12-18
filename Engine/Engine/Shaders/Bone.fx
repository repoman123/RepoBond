////////////////////////////////////////////////////////////////////////////////
// Filename: Bone.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix boneTransforms[100];
matrix worldMatrix;
matrix WVPmatrix; //world view projection matrix
Texture2D shaderTexture;
float4 ambientColor;
float4 diffuseColor;
float3 lightDirection;

float3 cameraPosition;
float4 specularColor;
float specularPower;

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 weights : WEIGHTS;
	float4 bones : BONES;
	
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
    float3 viewDirection : TEXCOORD1;
};



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType BoneVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

    matrix finalMatrix;
    finalMatrix = input.weights.x * boneTransforms[input.bones.x];
    finalMatrix += input.weights.y * boneTransforms[input.bones.y];
    finalMatrix += input.weights.z * boneTransforms[input.bones.z];
    finalMatrix += input.weights.w * boneTransforms[input.bones.w];    
    
    float4 vAnimatedPos = mul(float4(input.position.xyz,1),finalMatrix);
    float4 vAnimatedNormal = mul(float4(input.normal.xyz,0),finalMatrix);

	// Transform the position from object space to homogeneous projection space
    output.position = mul(vAnimatedPos,WVPmatrix);

    // Transform the normal from object space to world space    
    output.normal = float3(0,0,0);

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
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 BonePixelShader(PixelInputType input) : SV_Target
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
    float4 specular;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	 // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
	
	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
		// Saturate the ambient and diffuse color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir); 

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    // Saturate the final light color.
    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

	// Add the specular component last to the output color.
    color = saturate(color + specular);
    return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 BoneTechnique
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, BoneVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, BonePixelShader()));
        SetGeometryShader(NULL);
    }
}