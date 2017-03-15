sampler sample : register(s0);
Texture2D tex : register (t0);
cbuffer ParallelLight : register(b0)
{
	float3 pDirection;
	float3 pDiffuse;
	float3 pAmbient;
};
cbuffer PointLight : register(b1)
{
	float3 direction;
	float3 diffuse;
	float3 ambient;
};
float3 Parallel(float3 color,float3 normal)
{
	float3 litColor=pAmbient*color;
	float3 lightVector=-pDirection;
	float diffuseFactor=dot(lightVector,normal);
	if(diffuseFactor>0.0f)
	{
		litColor+=pDiffuse*color;
	}
	return litColor;
}
struct Input
{
float4 position : SV_POSITION;
float2 texCoords : TEXTURE;
float3 normal : NORMAL;
};
float4 main(Input input) : SV_TARGET
{
	float4 sampled=tex.Sample(sample,input.texCoords);
	sampled.xyz=Parallel(float3(sampled.xyz),input.normal);
	sampled.w=1.0f;
	return sampled;
}