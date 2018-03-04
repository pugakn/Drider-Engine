Texture2D Texture0 : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 Bones[200];
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
	float2 uvs		 : TEXCOORD0;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float2 uv = input.uvs;
	float3 n = input.hnormal.xyz;
	float4 color = Texture0.Sample(SS, uv);
    return color;
}