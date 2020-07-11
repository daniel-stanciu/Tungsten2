cbuffer PerFrame : register(b0)
{
	float4x4 viewMatrix ;
}
cbuffer PerObject : register(b1)
{
	float4x4 worldMatrix;
}
struct Input
{
float4 position : POSITION;
float2 texCoords : TEXTURE;
float3 normal : NORMAL;
};
struct Output
{
float4 position : SV_POSITION;
float2 texCoords : TEXTURE;
float3 normal : NORMAL;
};
Output main(Input input ) 
{
	Output output;
	float4x4 transform=mul(viewMatrix,worldMatrix);
	output.position= mul(transform,input.position);
	output.texCoords=input.texCoords;
	output.normal=mul(worldMatrix,input.normal);
	output.normal=normalize(output.normal);
	return output;
}