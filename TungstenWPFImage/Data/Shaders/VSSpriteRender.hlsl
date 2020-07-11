struct INPUT
{
	float4 pos : POSITION ;
	float2 texcoord : TEXTURE ;
	float4 tint : TINT;
};

cbuffer WINDOW_SIZE
{
	float x;
	float y;
	float2 _pad;
};


struct OUTPUT
{
	float4 pos : SV_POSITION ;
	float2 texcoord : TEXTURE ;
	float4 tint : TINT;
};
OUTPUT main(INPUT input ) 
{
	OUTPUT output;
	output.texcoord=input.texcoord;
	output.pos.x = input.pos.x / x *2-1;
	output.pos.y = input.pos.y / y *-2+1;
	output.pos.zw = input.pos.zw;
	//output.pos = 0;
	output.tint=input.tint;
	return output;
}