Texture2D AlbedoTex   : register(t0);
Texture2D NormalTex   : register(t1);
Texture2D EmissiveTex : register(t2);

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
	float4 Position	: SV_TARGET1;
	float4 Normal	 	: SV_TARGET2;
	float4 Emissive	: SV_TARGET3;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
  
	float2 uv = input.Texcoord;

	float4 albedo		= AlbedoTex.Sample(SS, uv);
	float4 position	= input.Position;
	float3 normal		= NormalTex.Sample(SS, uv);
	float4 emissive	= EmissiveTex.Sample(SS, uv); 

	outRT.Albedo	 = albedo;
	outRT.Position = position;
	outRT.Normal	 = float4(normal, 1);
	outRT.Emissive = emissive;

	outRT.Albedo	 = float4(1, 0 ,0 , 1);
	outRT.Position = float4(0, 1 ,0 , 1);
	outRT.Normal	 = float4(0, 0 ,1 , 1);
	outRT.Emissive = float4(1, 1 ,0 , 1);

	return outRT;
}