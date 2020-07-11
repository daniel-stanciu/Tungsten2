Texture2D spriteTex : register(t0); 
SamplerState sample	: register(s0);

struct INPUT
{
	float4 pos : SV_POSITION ;
	float2 texcoord : TEXTURE ;
	float4 tint : TINT;
};

cbuffer TEXTURE_SIZE
{
	float x;
	float y;
	float2 _pad;
};

float4 main(INPUT input) : SV_TARGET
{
	float width;
	float height;
	float levels;
	spriteTex.GetDimensions(0,width,height,levels);
	float4 color=spriteTex.Sample(sample,float2(input.texcoord.x/width,input.texcoord.y/height));
	color=color*input.tint;
	return color;
}