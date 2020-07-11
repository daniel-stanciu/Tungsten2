TextureCube cubeTex : register(t0); 
SamplerState sample	: register(s0);
float4 tint;
struct INPUT
{
	float4 screenPos : SV_Position ;
	float3 texCoord : TEXTURE;

};

float4 main(INPUT input) : SV_TARGET
{
	
	float4 color=cubeTex.Sample(sample,input.texCoord);
	color=color*tint;
	return color;
	//return float4(1.0f,1.0f,1.0f,1.0f);
}