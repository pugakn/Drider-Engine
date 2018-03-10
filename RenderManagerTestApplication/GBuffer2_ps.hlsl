Texture2D Position  : register(t0);
Texture2D Normal    : register(t1);
Texture2D Metallic  : register(t2);
Texture2D Roughness : register(t3);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
	float4x4 Bones[200];
};

struct PS_INPUT {
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
	float4 Metallic	 : SV_TARGET0;
	float4 Roughness : SV_TARGET1;
	float4 SSAO      : SV_TARGET2;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	float2 uv = input.Texcoord;
	
	outRT.Metallic  = float4(0.5f, 0.5f, 0.5f, 1.0f);
	outRT.Roughness = float4(0.0f, 0.0f, 0.0f, 1.0f);
	outRT.SSAO      = float4(1.0f, 1.0f, 1.0f, 1.0f);

	outRT.Metallic  = float4(1, 0, 1, 1);
	outRT.Roughness = float4(0, 1, 1, 1);
	outRT.SSAO      = float4(1, 1, 1, 1);
	
	return outRT;
}