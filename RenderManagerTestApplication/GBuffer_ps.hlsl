Texture2D Texture0 : register(t0);
SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float3 Normal   : NORMAL0;
  float3 Binormal : BINORMAL0;
  float3 Tangent  : TANGENT0;
  float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
  float4 Albedo		: SV_TARGET0;
	float4 Normal	 	: SV_TARGET1;
	float4 Position	: SV_TARGET2;
	float4 Emissive	: SV_TARGET3;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
  
	float2 uv = input.Texcoord;

	float4 albedo		= Texture0.Sample(SS, uv);
	float4 position	= input.Position;
	float3 normal		= input.Normal.xyz;
	float4 emissive	= float4(1, 1, 0, 1);

	outRT.Albedo = albedo;
	outRT.Normal = float4(normal, 1);
	outRT.Position = position;
	outRT.Emissive = emissive;

	return outRT;
}