cbuffer PerObject : register(b0)
{
	matrix transform;
};
struct INPUT
{
float4 position : POSITION;
float2 texCoords : TEXTURE;
float3 normal : NORMAL;

};

struct OUTPUT
{
	float4 screenPos : SV_Position ;
	float3 texCoord : TEXTURE;

};
OUTPUT main(INPUT input ) 
{
	OUTPUT output;
	output.screenPos=mul(transform,input.position);
	
	output.texCoord=input.position.xyz;
	return output;
}