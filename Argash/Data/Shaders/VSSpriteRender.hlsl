struct INPUT
{
	float4 pos : POSITION ;
	float2 texcoord : TEXTURE ;
	float4 tint : TINT;
};

struct OUTPUT
{
	float4 pos : SV_Position ;
	float2 texcoord : TEXTURE ;
	float4 tint : TINT;
};
OUTPUT main(INPUT input ) 
{
	OUTPUT output;
	output.texcoord=input.texcoord;
	output.pos=input.pos;
	output.tint=input.tint;
	return output;
}