Texture2D Texture0 : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
    float4x4 Bones[200];
}

struct VS_OUTPUT{
	float4 Position : SV_POSITION;
  	float3 Normal   : NORMAL0;
  	float3 Binormal : BINORMAL0;
  	float3 Tangent  : TANGENT0;
  	float2 Texcoord : TEXCOORD0;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float2 uv = input.Texcoord;
	float3 n = input.Normal.xyz;
	float4 color = Texture0.Sample(SS, uv);
    return color;
}